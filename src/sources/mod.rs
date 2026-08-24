use crate::db;
use serde::Deserialize;

#[derive(Deserialize)]
#[serde(rename_all = "snake_case")]
pub struct Source {
    pub name: String,
    pub version: String,
    pub url: String,
    pub description: String,
    pub musics: Vec<Music>,
}

#[derive(Deserialize)]
#[serde(rename_all = "snake_case")]
pub struct Music {
    pub name: String,
    pub artist: String,
    pub album: String,
    pub url: String,
    pub url_cover: String,
    pub year: String,
    pub tags: String,
}
pub struct SourceUi {
    pub id: i32,
    pub name: String,
    pub url: String,
}
/**
 * @param url The URL of the source to download
 * @returns The source as a string
 */
pub async fn download_source(url: &str) -> Result<String, reqwest::Error> {
    let body = reqwest::get(url).await?.text().await?;
    Ok(body)
}
/**
 * @param url The URL of the source to parse
 * @returns The parsed source
 */
pub async fn parse_source(url: &str) -> Result<Source, Box<dyn std::error::Error + Send + Sync>> {
    let json = download_source(url).await?;
    let source: Source = serde_json::from_str(&json)?;
    Ok(source)
}
/**
 * @param url The URL of the source to save
 * @returns The ID of the saved source
 */
pub async fn save_source(url: &str) -> Result<(), sqlite::Error> {
    let source = parse_source(url).await.unwrap();
    let db = db::init::connect();
    let source_id = db::musics::save_source_metadata(&db, &source)?;

    for music in source.musics {
        let _ = db::musics::save_music(&db, music, &source_id);
    }
    Ok(())
}
/**
 * @returns The list of all saved sources
 */
pub fn get_sources() -> Result<Vec<SourceUi>, sqlite::Error> {
    let db = db::init::connect();
    db::musics::get_sources(&db)
}
