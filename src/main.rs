#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // Needed to hide the console window on Windows

use slint::*;

slint::include_modules!();
mod db;
mod interactivity;
use image;
mod sources;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex, OnceLock};
use std::time::Duration;

use crate::db::MusicsDisplayed as DbMusicsDisplayed;

// Global reference to the current player, so we can control it from UI callbacks,
// independent of the thread that is blocked in sleep_until_end().
static PLAYER: OnceLock<Mutex<Option<Arc<rodio::Player>>>> = OnceLock::new();
static IS_PAUSED: AtomicBool = AtomicBool::new(false);

mod player;
#[tokio::main]
async fn main() -> Result<(), slint::PlatformError> {
    let _ = db::init::auto_init();

    let ui = MainWindow::new()?;

    interactivity::sources::show_sources(&ui);

    // Pause/play toggle
    // on the shared Player, if it exists (the music has had time to start).
    ui.on_pause(|| {
        let Some(player) = player::current_player() else {
            eprintln!("Pause demandée mais aucune lecture en cours.");
            return;
        };
        let was_paused = IS_PAUSED.fetch_xor(true, Ordering::SeqCst);
        if was_paused {
            player.play();
        } else {
            player.pause();
        }
    });

    let ui_weak_for_source = ui.as_weak();
    ui.on_add_source(move |url| {
        println!("Ajout de la source : {}", url);
        let ui_weak = ui_weak_for_source.clone();

        tokio::spawn(async move {
            let result =
                tokio::time::timeout(Duration::from_secs(30), sources::save_source(&url)).await;

            match result {
                Ok(Ok(())) => {
                    let _ = ui_weak.upgrade_in_event_loop(|ui| {
                        interactivity::sources::show_sources(&ui);
                    });
                }
                Ok(Err(err)) => eprintln!("Error while saving source: {err}"),
                Err(_) => eprintln!("Error: HTTP request timed out (3s)! Server does not respond."),
            }
        });
    });

    let ui_weak = ui.as_weak();
    tokio::spawn(async move {
        loop {
            // Waiting for 100ms between each update
            tokio::time::sleep(Duration::from_millis(100)).await;

            // Extracting properly the current time and total duration from Rodio and our OnceLock
            let current_time =
                player::current_player().map(|player| player.get_pos().as_secs_f32());
            let total_time = player::TOTAL_DURATION
                .get()
                .map(|total| total.as_secs_f32());

            // Returning the current time and total duration to the main thread of Slint
            let _ = ui_weak.upgrade_in_event_loop(move |ui| {
                if let Some(current) = current_time {
                    if !ui.get_is_changing() {
                        ui.set_currenttime(current);
                    }
                }
                if let Some(total) = total_time {
                    ui.set_maxtime(total);
                }
            });
        }
    });

    let ui_weak_for_position = ui.as_weak();
    ui.on_position_changed(move |value| {
        if let Some(player) = player::current_player() {
            let _result = player.try_seek(std::time::Duration::from_secs_f32(value));
        } else {
            eprintln!("[debug] position_changed: no player available");
        }
        let _ = ui_weak_for_position.upgrade_in_event_loop(move |ui| {
            ui.set_currenttime(value);
            ui.set_is_changing(false);
        });
    });

    let ui_weak_for_search = ui.as_weak();

    // Handles the search field
    ui.on_search(move |text| {
        println!("Search: {}", text);
        let ui_weak = ui_weak_for_search.clone();
        let term = text.to_string(); // Requested term search

        tokio::task::spawn_blocking(move || {
            let db = db::init::connect();
            // Calling search_musics on the database
            let musics = match db::musics::search_musics(&db, &term) {
                Ok(musics) => musics,
                Err(e) => {
                    eprintln!("Error: {}", e);
                    return;
                }
            };
            // Collecting cover URLs for the search results
            let cover_urls: Vec<(i32, String)> = musics
                .iter()
                .map(|music| (music.id, music.url_cover.clone()))
                .collect();

            let _ = ui_weak.upgrade_in_event_loop(move |ui| {
                let music_vec: Vec<MusicsDisplayed> = musics
                    .into_iter()
                    .map(|m: DbMusicsDisplayed| MusicsDisplayed {
                        id: m.id,
                        title: m.title.into(),
                        artist: m.artist.into(),
                        album: m.album.into(),
                        url: m.url.into(),
                        cover_url: m.url_cover.into(),
                        cover: slint::Image::default(), // Transmitting Image bytes to show it in the list
                    })
                    .collect();
                ui.set_musics(ModelRc::new(VecModel::from(music_vec))); // Transmitting results to the ui
                // Transforming cover URLs into Image bytes and updating the ui
                for (index, (music_id, cover_url)) in cover_urls.into_iter().enumerate() {
                    let ui_weak = ui.as_weak();
                    tokio::spawn(async move {
                        let Some((raw, width, height)) = load_image_bytes_async(cover_url).await
                        else {
                            return;
                        };
                        let _ = ui_weak.upgrade_in_event_loop(move |ui| {
                            let model = ui.get_musics();
                            let Some(mut music) = model.row_data(index) else {
                                return;
                            };
                            if music.id != music_id {
                                return;
                            }
                            let buffer =
                                slint::SharedPixelBuffer::clone_from_slice(&raw, width, height);
                            music.cover = slint::Image::from_rgba8(buffer);
                            model.set_row_data(index, music);
                        });
                    });
                }
            });
        });
    });
    let ui_weak_play = ui.as_weak();
    let ui_weak_play2 = ui.as_weak();
    // Handles the play music button
    ui.on_play_music(move |url, artist, title, album, cover_url| {
        player::stop();
        IS_PAUSED.store(false, Ordering::SeqCst);
        let ui_weak_play = ui_weak_play.clone();
        let ui_weak_play2 = ui_weak_play2.clone();
        let _handle = tokio::spawn(async move {
            if let Err(e) = player::play(&url).await {
                eprintln!("Erreur de lecture audio : {e}");
                let msg = e.to_string();
                let _ = slint::invoke_from_event_loop(move || {
                    if let Some(ui) = ui_weak_play.upgrade() {
                        ui.invoke_show_warning("Une erreur est survenue".into(), msg.into());
                        ui.set_maxtime(
                            player::TOTAL_DURATION
                                .get()
                                .map(|d| d.as_secs_f32())
                                .unwrap_or(0.0),
                        );
                    }
                });
            }
        });
        std::thread::spawn(move || {
            let ui_weak_play = ui_weak_play2.clone();
            if let Some((raw, w, h)) = load_image_bytes(cover_url.to_string()) {
                let _ = ui_weak_play.upgrade_in_event_loop(move |app| {
                    let buffer = slint::SharedPixelBuffer::clone_from_slice(&raw, w, h);
                    app.set_cover(slint::Image::from_rgba8(buffer));
                    app.set_title_music(title);
                    app.set_artist(artist);
                    app.set_album(album);
                });
            }
        });
    });

    let _ = ui.run();
    player::stop();

    Ok(())
}

fn load_image_bytes(url: String) -> Option<(Vec<u8>, u32, u32)> {
    println!("Loading image from {}", url);
    let bytes = reqwest::blocking::get(&url).ok()?.bytes().ok()?;
    let dynamic_img = image::load_from_memory(&bytes).ok()?;
    let rgba8 = dynamic_img.to_rgba8();
    let (w, h) = (rgba8.width(), rgba8.height());
    println!("Image loaded successfully");
    Some((rgba8.into_raw(), w, h))
}

async fn load_image_bytes_async(url: String) -> Option<(Vec<u8>, u32, u32)> {
    println!("Loading image from {}", url);
    let bytes = reqwest::get(&url).await.ok()?.bytes().await.ok()?;
    let dynamic_img = image::load_from_memory(&bytes).ok()?;
    let rgba8 = dynamic_img.to_rgba8();
    let (w, h) = (rgba8.width(), rgba8.height());
    println!("Image loaded successfully");
    Some((rgba8.into_raw(), w, h))
}
