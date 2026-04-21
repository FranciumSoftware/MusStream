#ifndef BACKEND_H
#define BACKEND_H

#include "sourcemanager.h"

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
public:
    explicit backEnd(QObject *parent = nullptr);
    Q_INVOKABLE int init();
    Q_INVOKABLE bool addSource(QString url);
    Q_INVOKABLE QList<QMap<QString, QVariant>> getSources();
    Q_INVOKABLE bool deleteSource(const int id);
    Q_INVOKABLE void searchMusic(const QString search);
    Q_INVOKABLE void setInfosTo(const QString name, const QString artist, const QString cover);
    Q_INVOKABLE QString getName() {return private_name;}
    Q_INVOKABLE QString getArtist() {return private_artist;}
    Q_INVOKABLE QString getCover() {return private_cover;}

    QVariantList getSearchResults() const { return m_searchResults; }


private:
    SourceManager *m_sourceManager;
    QVariantList m_searchResults;
    QString private_artist = "Artist";
    QString private_name = "Title";
    QString private_cover = "";
signals:
    void sourceAddedSuccess();
    void sourceAddedError(QString message);
    void searchResultsChanged();
};

#endif // BACKEND_H
