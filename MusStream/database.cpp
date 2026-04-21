#include "database.h"

database::database(QObject *parent)
    : QObject{parent}
{

}
/**
 * The database is an SQLite databse. The file associated with it is `data.db`
 * @brief database::createDatabase. Used to init the database
 * @return bool value true -> success, false -> failure
 *
 */
bool database::createDatabase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    // 3. Tenter d'ouvrir la base
    if (!db.open()) {
        qDebug() << "Erreur lors de l'ouverture :" << db.lastError().text();
        return false;
    }

    qDebug() << "Fichier .db créé et ouvert avec succès !";

    // 4. Créer une table
    QSqlQuery query;

    // 1. Désactiver les clés étrangères (Optionnel mais présent dans ton SQL)
    query.exec("PRAGMA foreign_keys = off;");

    // 2. OUVRIR la transaction (C'est ce qui manquait !)
    if (!query.exec("BEGIN TRANSACTION;")) {
        qDebug() << "Erreur début transaction:" << query.lastError().text();
        return false;
    }

    // 3. Création Table Musics
    if (!query.exec("CREATE TABLE IF NOT EXISTS musics (ID INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT (255) NOT NULL, artist TEXT (255), album TEXT (255), year REAL (4) DEFAULT (0), url TEXT (256) NOT NULL, url_cover TEXT (256), cache_cover TEXT, mime_cover TEXT (100) DEFAULT \"image/png\", lyrics TEXT (256), tags TEXT (2500) NOT NULL, fk_source INTEGER CONSTRAINT fk_source REFERENCES sources (ID) ON DELETE CASCADE, score INTEGER NOT NULL DEFAULT (0));")) {
        qDebug() << "Erreur musics:" << query.lastError().text();
        query.exec("ROLLBACK;"); // On annule tout en cas d'erreur
        return false;
    }

    // 4. Création Table Playlists
    if (!query.exec("CREATE TABLE IF NOT EXISTS playlists (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT (255) UNIQUE NOT NULL, \"table\" TEXT (255) NOT NULL UNIQUE);")) {
        qDebug() << "Erreur playlists:" << query.lastError().text();
        query.exec("ROLLBACK;");
        return false;
    }

    // 5. Création Table Sources
    if (!query.exec("CREATE TABLE IF NOT EXISTS sources (ID INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT (255) NOT NULL, url TEXT (256) NOT NULL, version REAL NOT NULL, favicon TEXT (256));")) {
        qDebug() << "Erreur sources:" << query.lastError().text();
        query.exec("ROLLBACK;");
        return false;
    }

    // 6. VALIDER la transaction
    if (!query.exec("COMMIT;")) {
        qDebug() << "Erreur commit:" << query.lastError().text();
        return false;
    }

    // 7. Réactiver les clés étrangères
    query.exec("PRAGMA foreign_keys = on;");

    return true;
}

int database::insertSourceInfos(const QString &name, const QString &url, int version, QString favicon) {
    QSqlQuery query;
    query.prepare("INSERT INTO sources (name, url, version) VALUES (:name, :url, :version)");
    query.bindValue(":name", name);
    query.bindValue(":url", url);
    query.bindValue(":version", version);

    if (query.exec()) {
        // Récupère l'ID généré par l'AUTOINCREMENT
        qDebug() << "ID: " << query.lastInsertId().toInt();
        return query.lastInsertId().toInt();
    } else {
        qDebug() << "Erreur insertion source :" << query.lastError().text();
        return -1;
    }
}
/**
 * This function is used to insert every titles in the database. Here are the fields:
 * | name        | type    | constraints                     | Accept null | Description                          | Default value |
 *|-------------|---------|---------------------------------|-------------|--------------------------------------|---------------|
 *| ID          | INTEGER | [object Object]                 | FALSE       | Primary key of the table             |               |
 *| name        | TEXT    | [object Object]                 | FALSE       | Title of the song                    |               |
 *| artist      | TEXT    |                                 | TRUE        | Name of the artist                   |               |
 *| album       | TEXT    |                                 | TRUE        | Album                                |               |
 *| year        | REAL    | [object Object]                 | TRUE        | Year of creation                     | 0             |
 *| url         | TEXT    | [object Object]                 | FALSE       | URL of the music                     |               |
 *| url_cover   | TEXT    |                                 | TRUE        | URL of the cover                     |               |
 *| cache_cover | TEXT    |                                 | TRUE        | Cache of the cover encoded in base64 |               |
 *| mime_cover  | TEXT    | [object Object]                 | TRUE        | Type mime of the cached cover        | image/png     |
 *| lyrics      | TEXT    |                                 | TRUE        | URL of the .lrc lyrics files         |               |
 *| tags        | TEXT    | [object Object]                 | FALSE       | Type of music (comma separated)      |               |
 *| fk_source   | INTEGER |                                 | TRUE        | foreing key with the SOURCES table   |               |
 *| score       | INTEGER | [object Object],[object Object] | FALSE       | Notation for the algorythme          | 0             |

 * @brief database::addSourceToDatabase
 * @param
 *      const int &fk_source,
        const QString &name,
        const QString &url,
        const QString &tags,
        const QString &artistL,
        const QString &album,
        const int &year,
        const QString &url_cover,
        const QString &cache_cover,
        const QString &mime_cover,
        const QString &lyrics
 * @return Boolean value
 * *True* for a success
 * *False* for a failure
 */
bool database::addSourceToDatabase(
        const int &fk_source,
        const QString &name,
        const QString &url,
        const QString &tags,
        const QString &artist,
        const QString &album,
        const int &year,
        const QString &url_cover,
        const QString &lyrics
    )
{
    QSqlQuery query;
    const QString request = "INSERT INTO musics (name, artist, album, year, url, url_cover, lyrics, tags, fk_source) "
                            "VALUES (:name, :artist, :album, :year, :url, :url_cover, :lyrics, :tags, :fk_source)";

    query.prepare(request);

    // Bind parameters to the query
    query.bindValue(":name", name);          // Replace `name` with your variable (track name)
    query.bindValue(":artist", artist);      // Replace `artist` with your variable (artist name)
    query.bindValue(":album", album);        // Replace `album` with your variable (album name)
    query.bindValue(":year", year);          // Replace `year` with your variable (release year, integer)
    query.bindValue(":url", url);            // Replace `url` with your variable (track URL)
    query.bindValue(":url_cover", url_cover);// Replace `url_cover` with your variable (cover art URL)
    query.bindValue(":lyrics", lyrics);      // Replace `lyrics` with your variable (lyrics text)
    query.bindValue(":tags", tags);          // Replace `tags` with your variable (tags, e.g., genre)
    query.bindValue(":fk_source", fk_source);// Replace `fk_source` with your variable (foreign key for source, integer)

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Error during insertion:" << query.lastError().text();
        return false;
    }

    return true;
}
int database::getSourceId() {
    const QString request = "SELECT * FROM sources ORDER BY id DESC LIMIT 1;";
    QSqlQuery query;
    if (!query.exec()) {
        qWarning() << "A fatal error occured: " << query.lastError().text();
        return 0;
    }
    return query.value("ID").toInt();
}
QList<QMap<QString, QVariant>> database::getAddedSources() {
    QSqlQuery query;
    const QString request = "SELECT * FROM sources";

    if (!query.exec(request)) {
        qCritical() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return {}; // Retourne une liste vide en cas d'erreur
    }

    QList<QMap<QString, QVariant>> results;

    // Parcourir chaque ligne du résultat
    while (query.next()) {
        QMap<QString, QVariant> row;

        // Parcourir chaque colonne de la ligne
        for (int i = 0; i < query.record().count(); ++i) {
            QString columnName = query.record().fieldName(i);
            QVariant value = query.value(i);
            row[columnName] = value;
        }

        results.append(row);
    }

    return results;
}
bool database::deleteSource(const int id) {
    qDebug() << "ID: " << id;
    QSqlQuery query;
    const QString request = "DELETE from 'sources' WHERE id = :id";
    query.prepare(request);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Unable to delete the source: " << query.lastError().text();
        return false;
    }
    qDebug() << "Status: " << query.numRowsAffected() ;
    return true;
}

QList<QMap<QString, QVariant>> database::searchMusic(QString search) {
    QSqlQuery query;
    search = "%"+search+"%";
    const QString request = "SELECT * FROM musics WHERE name LIKE :search" ;
    query.prepare(request);
    query.bindValue(":search", search);
    if (!query.exec()) {
        qWarning() << "Unable to find the music. More details: " << query.lastError().text();
        return {};
    }

    QList<QMap<QString, QVariant>> results;
    while (query.next()) {
        QMap<QString, QVariant> row;

        for (int i = 0; i < query.record().count(); ++i) {
            QString columnName = query.record().fieldName(i);
            QVariant value = query.value(i);
            row[columnName] = value;
        }

        results.append(row);

    }
    return results;
}
