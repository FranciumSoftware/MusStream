#ifndef BACKEND_H
#define BACKEND_H

#include "sourcemanager.h"
#include "updatechecker.h"

#include <QObject>
/**
 * @class backEnd
 * @brief Link between the backend and the front end
 *
 * Every functions you want to link with the user interface must be called in this class.
 */
class backEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList searchResults READ getSearchResults NOTIFY searchResultsChanged)
    Q_PROPERTY(QVariantList randomTitles READ get10randomMusics NOTIFY randomMusicsChanged)
public:
    explicit backEnd(QObject *parent = nullptr);
    Q_INVOKABLE int init();
    Q_INVOKABLE bool addSource(QString url);
    Q_INVOKABLE QList<QMap<QString, QVariant>> getSources();
    Q_INVOKABLE bool deleteSource(const int id);
    Q_INVOKABLE void searchMusic(const QString search);
    Q_INVOKABLE void randomMusics();
    Q_INVOKABLE void setInfosTo(const QString name, const QString artist, const QString cover);
    Q_INVOKABLE QString getName() {return private_name;}
    Q_INVOKABLE QString getArtist() {return private_artist;}
    Q_INVOKABLE QString getCover() {return private_cover;}
    Q_INVOKABLE void checkForUpdates();

    QVariantList getSearchResults() const { return m_searchResults; }
    QVariantList get10randomMusics() const { return m_randomMusics; }


private:
    SourceManager *m_sourceManager;
    QVariantList m_searchResults;
    QVariantList m_randomMusics;
    QString private_artist = "Artist";
    QString private_name = "Title";
    QString private_cover = "";
    UpdateChecker *m_updateChecker;
signals:
    void sourceAddedSuccess();
    void sourceAddedError(QString message);
    void searchResultsChanged();
    void randomMusicsChanged();
    void updateAvailable(const QString &version, const QString &url, const QString &changelog);
};

#endif // BACKEND_H
