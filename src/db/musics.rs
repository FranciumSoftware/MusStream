use super::DbManager;
use crate::{
    db::MusicsDisplayed,
    sources::{Music, Source, SourceUi},
};
use sqlite::State;

/**
 * Saves a music to the database
 * @param db - the database manager type DbManager
 * @param music - the music to save
 * @param source_id - the id of the source the music belongs to
 */
pub fn save_music(db: &DbManager, music: Music, source_id: &i64) -> Result<(), sqlite::Error> {
    let mut stmt = db.connection.prepare("INSERT INTO musics (title, artist, album, url, cover, year, tags, fk_source) VALUES (?, ?, ?, ?, ?, ?, ?, ?)").unwrap();
    stmt.bind((1, music.name.as_str()))?;
    stmt.bind((2, music.artist.as_str()))?;
    stmt.bind((3, music.album.as_str()))?;
    stmt.bind((4, music.url.as_str()))?;
    stmt.bind((5, music.url_cover.as_str()))?;
    stmt.bind((6, music.year.as_str()))?;
    stmt.bind((7, music.tags.as_str()))?;
    stmt.bind((8, source_id.to_string().as_str()))?;
    stmt.next()?;
    Ok(())
}
/**
 * Saves the metadata of a source to the database
 * @param db - the database manager type DbManager
 * @param source - the source to save
 */
pub fn save_source_metadata(db: &DbManager, source: &Source) -> Result<i64, sqlite::Error> {
    let mut stmt = db
        .connection
        .prepare("INSERT INTO sources (name, url, version) VALUES (?, ?, ?)")
        .unwrap();
    stmt.bind((1, source.name.as_str()))?;
    stmt.bind((2, source.url.as_str()))?;
    stmt.bind((3, source.version.as_str()))?;
    stmt.next()?;
    let mut id_stmt = db.connection.prepare("SELECT last_insert_rowid()")?;
    id_stmt.next()?;
    let last_insert_id: i64 = id_stmt.read(0)?;
    Ok(last_insert_id)
}
/**
 * Retrieves all sources from the database
 */
pub fn get_sources(db: &DbManager) -> Result<Vec<SourceUi>, sqlite::Error> {
    let query = "SELECT * FROM sources";
    let mut sources = Vec::new();
    db.connection.iterate(query, |pairs| {
        let mut id = 0;
        let mut name = String::new();
        let mut url = String::new();
        for &(column_name, value) in pairs.iter() {
            let val = value.unwrap_or_default();
            match column_name {
                "id" => id = val.parse().unwrap_or_default(),
                "name" => name = val.parse().unwrap_or_default(),
                "url" => url = val.parse().unwrap_or_default(),
                _ => {}
            }
        }
        sources.push(SourceUi { id, name, url });

        true
    })?;

    Ok(sources)
}

/**
 * Searches for musics in the database that match the given term
 * @param db - the database manager type DbManager
 * @param term - the term to search for
 * @returns a vector of musics that match the given term
 */
pub fn search_musics(db: &DbManager, term: &str) -> Result<Vec<MusicsDisplayed>, sqlite::Error> {
    let query = "SELECT id, title, artist, album, url, cover FROM musics WHERE title like ? OR artist like ? OR album like ?";
    let mut stmt = db.connection.prepare(query)?;
    let term = format!("%{}%", term.to_string());
    stmt.bind((1, term.as_str()))?;
    stmt.bind((2, term.as_str()))?;
    stmt.bind((3, term.as_str()))?;
    let mut musics = Vec::new();
    while let Ok(State::Row) = stmt.next() {
        let id: i64 = stmt.read(0)?;
        let title = stmt.read(1)?;
        let artist = stmt.read(2)?;
        let album = stmt.read(3)?;
        let url = stmt.read(4)?;
        let url_cover = stmt.read(5)?;
        musics.push(MusicsDisplayed {
            id: id as i32,
            title,
            artist,
            album,
            url,
            url_cover,
        });
    }
    Ok(musics)
}
