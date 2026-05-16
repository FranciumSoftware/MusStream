#include "sourcemanager.h"
#include "database.h"
#include <QNetworkReply>
#include <QDebug>

SourceManager::SourceManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}
/**
 * @brief SourceManager::getSourceData
 * @param QString `url` the url of the source. It must be in JSON format.
 */
void SourceManager::getSourceData(const QString &url) {
    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseData = reply->readAll();

            getSourcesInfo(responseData);

            // STEP 1: extract sources info
            database db;
            int newId = db.insertSourceInfos(source_name, source_url, version);

            // STEP 2: add source infos to database
            if (newId != -1) {

                // STEP 3: uniquely if the step 2 is a success, we parse the musics array
                m_musicData = parseJson(responseData);

                // STEP 4: we add the musics
                addToDatabase(m_musicData, newId);

                emit ready();
            } else {
                qDebug() << "Error: unable to add the musics";
            }
        }
        reply->deleteLater();
    });
}
/**
 * Extract the ` musics ` JSON array from the response.
 * @brief SourceManager::parseJson
 * @param ` QString data ` the JSON downloaded previuosly by getSourceData void function.
 * @return QJsonArray
 */
QJsonArray SourceManager::parseJson(const QString &data) {
    
    // 1. Convert String to JSON doc
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

    // 2. Verify if JSON is valid
    if (doc.isNull()) {
        qDebug() << "Le JSON est invalide !";
        return QJsonArray();
    }
    
    qDebug() << "JSON valide";

    // 3. Access to root object
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        
        if (obj.contains("musics")) {
            if (obj["musics"].isArray()) {
                QJsonArray musics = obj["musics"].toArray();
                return musics;
            } else {
                return QJsonArray();
            }
        } else {
            return QJsonArray();
        }
    }
    return QJsonArray();
}

void SourceManager::getSourcesInfo(const QString &json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        if (obj["name"].isUndefined() || obj["version"].isUndefined()) {
            return;
        }
        source_name = obj["name"].toString();
        source_url = obj["url"].toString();
        version = obj["version"].toInt();
    }
}



bool SourceManager::addToDatabase(const QJsonArray &data, const int fk_source) {

    database db;
    for (int i = 0; i < data.size(); ++i) {
        QJsonObject music = data[i].toObject();
        QString name = music["name"].toString();
        qDebug() << "Name: "<< name;
        QString artist = music["artist"].toString();
        QString album = music["album"].toString();
        int year = music["year"].toInt(); // Assure que la clé "year" existe dans `music`
        QString url = music["url"].toString();
        QString url_cover = music["url_cover"].toString(); // Remplace par la clé correcte si différente
        QString lyrics = music["lyrics"].toString(); // Remplace par la clé correcte si différente
        QString tags = music["tags"].toString(); // Remplace par la clé correcte si différente
        db.addSourceToDatabase(fk_source, name, url, tags, artist, album, year, url_cover, lyrics);
    }
    
    return true;
}

/**
 * Call every necessary functions to add a source
 * @brief SourceManager::addSource
 * @param QString url
 * @return
 * | | |
 * |-------|-------|
 * | True | Success|
 * |False | Failure |
 */
bool SourceManager::addSource(const QString &url) {
    getSourceData(url);
    return true; // On retourne true pour dire que le processus est lancé
}