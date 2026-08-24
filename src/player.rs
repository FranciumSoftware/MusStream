use lofty::file::AudioFile;
use std::error::Error;
use std::io::{Read, Seek, SeekFrom};
use std::sync::{Arc, Mutex, OnceLock};
use std::time::Duration;
use stream_download::storage::temp::TempStorageProvider;
use stream_download::{Settings, StreamDownload};
pub static TOTAL_DURATION: OnceLock<Duration> = OnceLock::new();

pub fn current_player() -> Option<Arc<rodio::Player>> {
    crate::PLAYER
        .get()
        .and_then(|state| state.lock().ok()?.as_ref().cloned())
}

pub fn stop() {
    let player = crate::PLAYER
        .get()
        .and_then(|state| state.lock().ok()?.take());
    if let Some(player) = player {
        player.stop();
    }
}
struct HttpRangeReader {
    client: reqwest::blocking::Client,
    url: String,
    total_len: u64,
    pos: u64,
}
fn sniff_is_mp4_container<R: Read + Seek>(reader: &mut R) -> std::io::Result<bool> {
    let mut header = [0u8; 12];
    reader.read_exact(&mut header)?;
    reader.seek(SeekFrom::Start(0))?; // on remet le curseur au début pour le decoder
    Ok(&header[4..8] == b"ftyp")
}
impl std::io::Read for HttpRangeReader {
    fn read(&mut self, buf: &mut [u8]) -> std::io::Result<usize> {
        if self.pos >= self.total_len {
            return Ok(0);
        }
        let end = (self.pos + buf.len() as u64 - 1).min(self.total_len - 1);
        let resp = self
            .client
            .get(&self.url)
            .header("Range", format!("bytes={}-{}", self.pos, end))
            .send()
            .map_err(std::io::Error::other)?;
        let bytes = resp.bytes().map_err(std::io::Error::other)?;
        let n = bytes.len();
        buf[..n].copy_from_slice(&bytes);
        self.pos += n as u64;
        Ok(n)
    }
}

impl std::io::Seek for HttpRangeReader {
    fn seek(&mut self, pos: std::io::SeekFrom) -> std::io::Result<u64> {
        self.pos = match pos {
            std::io::SeekFrom::Start(p) => p,
            std::io::SeekFrom::End(offset) => (self.total_len as i64 + offset).max(0) as u64,
            std::io::SeekFrom::Current(offset) => (self.pos as i64 + offset).max(0) as u64,
        };
        Ok(self.pos)
    }
}

fn fetch_duration_via_range(
    url: &str,
) -> Result<Option<std::time::Duration>, Box<dyn Error + Send + Sync>> {
    let client = reqwest::blocking::Client::new();
    let head = client.head(url).send()?;
    let total_len = head
        .headers()
        .get(reqwest::header::CONTENT_LENGTH)
        .and_then(|v| v.to_str().ok())
        .and_then(|v| v.parse::<u64>().ok())
        .ok_or("Content-Length absent : impossible de faire des Range requests")?;

    let reader = HttpRangeReader {
        client,
        url: url.to_string(),
        total_len,
        pos: 0,
    };

    let tagged_file = lofty::probe::Probe::new(reader).guess_file_type()?.read()?;

    Ok(Some(tagged_file.properties().duration()))
}

pub async fn play(url: &str) -> Result<(), Box<dyn Error + Send + Sync>> {
    eprintln!("[1/6] Chargement du flux HTTPS : {}...", url);

    // Récupération de la durée en parallèle via lofty + Range requests HTTP.
    // Ne bloque pas le démarrage de la lecture : le résultat arrive dès
    // que possible et met à jour TOTAL_DURATION quand il est prêt.
    let url_for_duration = url.to_string();
    tokio::task::spawn_blocking(move || match fetch_duration_via_range(&url_for_duration) {
        Ok(Some(duration)) => {
            eprintln!("Durée totale récupérée via lofty : {:?}", duration);
            let _ = TOTAL_DURATION.set(duration);
        }
        Ok(None) => eprintln!("lofty n'a pas trouvé de durée dans les métadonnées."),
        Err(e) => eprintln!("Erreur récupération durée via lofty : {e}"),
    });

    eprintln!("[2/6] URL parsée, ouverture du flux (StreamDownload::new_http)...");
    let reader = StreamDownload::new_http(
        url.parse()?,
        TempStorageProvider::new(),
        Settings::default(),
    )
    .await?;
    eprintln!("[3/6] Flux ouvert.");

    tokio::task::spawn_blocking(move || -> Result<(), Box<dyn Error + Send + Sync>> {
        let mut reader = reader;
        if sniff_is_mp4_container(&mut reader)? {
            return Err("Conteneur MP4/M4A temporairement non supporté (bug rodio #846)".into());
        }
        eprintln!("[4/6] Ouverture du périphérique audio par défaut...");
        let sink_handle = rodio::DeviceSinkBuilder::open_default_sink()
            .map_err(|e| std::format!("Impossible d'ouvrir le périphérique audio : {:?}", e))?;
        eprintln!(
            "[4/6] Périphérique audio ouvert : {:?}",
            sink_handle.config()
        );
        let player = Arc::new(rodio::Player::connect_new(&sink_handle.mixer()));
        eprintln!("[5/6] Player connecté. Décodage de la source...");

        // with_seekable(false) : on ne fait plus confiance à total_duration()
        // de rodio en streaming, la durée vient désormais de lofty ci-dessus.
        let source = rodio::Decoder::builder()
            .with_data(reader)
            .with_seekable(false)
            .build()?;
        eprintln!("[6/6] Source décodée, lancement de la lecture.");

        let state = crate::PLAYER.get_or_init(|| Mutex::new(None));
        if let Ok(mut current_player) = state.lock() {
            *current_player = Some(player.clone());
        }
        player.append(source);
        player.sleep_until_end();
        eprintln!("Lecture terminée.");
        Ok(())
    })
    .await??;
    Ok(())
}
