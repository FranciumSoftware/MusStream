#include "updatechecker.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}

void UpdateChecker::checkForUpdates()
{
    qDebug() << "Calling function in check updater class";
    // Remplace par l'URL raw de ton fichier version.json
    QUrl url("https://raw.githubusercontent.com/FranciumSoftware/MusStream/refs/heads/develop/version.json");
    QNetworkReply *reply = m_manager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit checkFailed(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (doc.isNull() || !doc.isObject()) {
            emit checkFailed("Réponse invalide");
            return;
        }

        QJsonObject obj = doc.object();
        QString remoteVersion = obj["version"].toString();
        QString downloadUrl   = obj["download_url"].toString();
        QString changelog     = obj["changelog"].toString();

        if (isNewerVersion(remoteVersion, CURRENT_VERSION)) {
            qDebug() << "An update is avalaible !";
            emit updateAvailable(remoteVersion, downloadUrl, changelog);
        } else {
            emit noUpdateAvailable();
        }
    });
}

// Compare "MAJOR.MINOR.PATCH" — retourne true si remote > local
bool UpdateChecker::isNewerVersion(const QString &remote, const QString &local)
{
    auto split = [](const QString &v) {
        QStringList parts = v.split('.');
        while (parts.size() < 3) parts.append("0");
        return parts;
    };

    QStringList r = split(remote);
    QStringList l = split(local);

    for (int i = 0; i < 3; ++i) {
        int rv = r[i].toInt();
        int lv = l[i].toInt();
        if (rv > lv) return true;
        if (rv < lv) return false;
    }
    return false;
}