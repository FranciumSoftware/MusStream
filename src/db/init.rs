use super::DbManager;
const SCHEMA: &str = include_str!("../../db/db.sql");

use std::env::home_dir;
use std::fs;
use std::path::PathBuf;

/**
 * Initializes the database in the user's home directory at the app launch
 * @returns a DbManager instance
 */
fn init_db(dir: &str) -> DbManager {
    // make sure the folder exists
    let mut db_path = PathBuf::from(dir);
    fs::create_dir_all(&db_path).expect("failed to create db directory");

    // use file name data.sqlite inside that folder
    db_path.push("data.sqlite");
    let db_file = db_path.to_string_lossy();

    // Connection::open will create the file if it doesn't exist
    let db = DbManager::new(&db_file);
    db.init(SCHEMA);
    db
}
/**
 * Returns the user's home directory path on every OSes
 * @returns the user's home directory path as a string
 */
fn get_user_path() -> String {
    match home_dir() {
        Some(path) => path.to_string_lossy().into_owned(),
        None => "".to_string(),
    }
}
/**
 * Automatically initializes the database in the user's home directory
 * @returns a DbManager instance
 */
pub fn auto_init() -> DbManager {
    let path = get_user_path() + "/.MusStream";
    init_db(&path)
}
/**
 * Connects to the database in the user's home directory. To use after auto init.
 * @returns a DbManager instance
 */
pub fn connect() -> DbManager {
    let path = get_user_path() + "/.MusStream/data.sqlite";
    DbManager::new(&path)
}
