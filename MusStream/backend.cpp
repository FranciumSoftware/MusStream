#include "backend.h"
#include "database.h"

backEnd::backEnd(QObject *parent)
    : QObject{parent}
{
    m_sourceManager = new SourceManager(this);
    
    connect(m_sourceManager, &SourceManager::ready, this, [this]() {
        QJsonArray musics = m_sourceManager->getMusicsData();
        qDebug() << "Backend: Les données sont prêtes!" << musics.size() << "musiques";

        // On informe l'UI que c'est un succès !
        emit sourceAddedSuccess();
    });
}
/**
 * @brief Call every neccessary functions at the launch of the app
 * @param No params needed
 * @return Return the result:
 * 0 -> Success
 * 1 -> Database initialisation error
 */
int backEnd::init(){
    database database;
    if (!database.createDatabase()) {
        return 1;
    }
    return 0;
}
/**
* @brief Function called if the AddSource button is checked
* @param QString URL (the url of the source)
* @return bool if success -> true else -> false
*/
bool backEnd::addSource(QString url) {
    m_sourceManager->addSource(url);
    // Les données seront traitées automatiquement via le signal musicsReady
    return true;
}
/**
 * @brief Function that call database::getAddedSources exchange between UI and backend
 * @return The content of the table sources
 */
QList<QMap<QString, QVariant>> backEnd::getSources() {
    database database;
    QList<QMap<QString, QVariant>> result = database.getAddedSources() ;

    return result;
}
/**
 * @brief backEnd::deleteSource call database::deleteSource(const int id)
 * @param id
 * @return true -> success; false -> failure
 */
bool backEnd::deleteSource(const int id) {
    database database;
    if(!database.deleteSource(id)) {
        qWarning() << "Error when calling function deleteSource";
        return false;
    }

    return true;
}
void backEnd::searchMusic(const QString search) {
    qDebug() << "Calling function";
    database database;
    QList<QMap<QString, QVariant>> result = database.searchMusic(search);

    QVariantList variantList;
    for (const auto &map : result) {
        QVariantMap variantMap;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
            variantMap[it.key()] = it.value();
        }
        variantList.append(variantMap);
    }

    m_searchResults = variantList;
    emit searchResultsChanged();
}
// Store actual musics infos to show it in GUI
void backEnd::setInfosTo(const QString name, const QString artist, const QString cover) {
    private_name = name;
    private_artist = artist;
    private_cover = cover;
}