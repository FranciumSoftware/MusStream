#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QJsonArray>

/**
 * @name database
 * @brief The class for databases operations.
 */
class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = nullptr);
    int insertSourceInfos(const QString &name, const QString &url, const int version, QString favicon = "");
    bool createDatabase();
    bool addSourceToDatabase(
            const int &fk_source,
            const QString &name,
            const QString &url,
            const QString &tags,
            const QString &artist = NULL,
            const QString &album = NULL,
            const int &year = NULL,
            const QString &url_cover = NULL,
            const QString &lyrics = NULL
        );
    int getSourceId();
    QList<QMap<QString, QVariant>> getAddedSources();
    bool deleteSource(const int id);
    QList<QMap<QString, QVariant>> searchMusic(QString search);
private:
    QSqlDatabase db;
signals:

};

#endif // DATABASE_H
