#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>

class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QObject *parent = nullptr);
    void checkForUpdates();

    // Version actuelle de l'app, définie à la compilation
    static constexpr const char* CURRENT_VERSION = "0.1.0";

signals:
    void updateAvailable(const QString &newVersion, const QString &downloadUrl, const QString &changelog);
    void noUpdateAvailable();
    void checkFailed(const QString &error);

private:
    QNetworkAccessManager *m_manager;
    bool isNewerVersion(const QString &remote, const QString &local);
};

#endif