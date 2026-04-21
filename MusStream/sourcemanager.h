#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "database.h"

class SourceManager : public QObject
{
    Q_OBJECT // 2. Macro indispensable pour les signaux

public:
    explicit SourceManager(QObject *parent = nullptr);

    // On change le retour en void car la réponse arrive plus tard (asynchrone)
    void getSourceData(const QString &url);
    QJsonArray parseJson(const QString &data);
    bool addSource(const QString &url);
    bool addToDatabase(const QJsonArray &data, const int fk_source);  // Surcharge pour compatibilité
    bool addToDatabase(const QJsonArray &data, database &db);  // Version interne
    
    // Getter pour accéder aux données parsées
    QJsonArray getMusicsData() const { return m_musicData; }
    void getSourcesInfo(const QString &json);


signals:
    void dataReceived(const QString &data); // Le signal pour transmettre la réponse brute
    void ready();  // Signal simple pour notifier que les données sont parsées et prêtes

private:
    QNetworkAccessManager *manager;
    QJsonArray m_musicData;  // Stockage des données parsées
    QString source_name;
    QString source_url;
    int version;
    int m_fk_source;  // Stockage de l'ID de la source

};

#endif