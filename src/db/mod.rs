pub mod init;
pub mod musics;
use sqlite::Connection;

pub struct DbManager {
    path: String,
    connection: Connection,
}
pub struct MusicsDisplayed {
    pub id: i32,
    pub title: String,
    pub artist: String,
    pub album: String,
    pub url: String,
    pub url_cover: String,
}

impl DbManager {
    /**
     * Create a new DbManager instance
     * @param path The path to the .sqlite or .db file
     */
    pub fn new(path: &str) -> Self {
        let connection = Connection::open(path).unwrap();
        Self {
            path: path.to_string(),
            connection,
        }
    }
    /**
     * Initialises database by executing the query given at the beginning
     * @param query The sql query to execute
     */
    pub fn init(&self, query: &str) {
        self.connection.execute(query).unwrap();
    }
}
