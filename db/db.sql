--
-- Fichier généré avec Letos v4.0.0 le Sat Aug 15 14:42:06 2026
--
-- Encodage texte utilisé : UTF-8
--
PRAGMA foreign_keys = off; BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS "cache" (id INTEGER PRIMARY KEY AUTOINCREMENT REFERENCES playlists (cover), type TEXT (50) NOT NULL, data TEXT (25000) NOT NULL);

-- Tableau : musics
CREATE TABLE IF NOT EXISTS "musics" (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT (255) NOT NULL, artist TEXT (255) DEFAULT Unknown, album TEXT (255), year INTEGER (4), url TEXT (256) NOT NULL, cover TEXT (255), lyrics TEXT (256), tags TEXT (255), fk_source INTEGER REFERENCES sources (id) ON DELETE CASCADE, fk_cover_cache INTEGER REFERENCES cache (id) ON DELETE SET NULL ON UPDATE NO ACTION);

-- Tableau : playlists
CREATE TABLE IF NOT EXISTS "playlists" (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, "table" TEXT NOT NULL, cover TEXT (2500) REFERENCES cache (id) ON DELETE SET NULL ON UPDATE NO ACTION);

-- Tableau : sources
CREATE TABLE IF NOT EXISTS "sources" (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT (255) NOT NULL, url TEXT (255) NOT NULL, version INTEGER NOT NULL, favicon TEXT (255));

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
