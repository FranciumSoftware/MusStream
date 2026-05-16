#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include "external/QSimpleUpdater/include/QSimpleUpdater.h"
#include "QProcess"

class updater: public QObject
{
public:
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ currentVersion WRITE setCurrentVersion NOTIFY currentVersionChanged)
    Q_PROPERTY(bool updateAvailable READ updateAvailable NOTIFY updateAvailableChanged)
    Q_PROPERTY(bool isDownloading READ isDownloading NOTIFY isDownloadingChanged)
    Q_PROPERTY(int downloadProgress READ downloadProgress NOTIFY downloadProgressChanged)
    Q_PROPERTY(QString latestVersion READ latestVersion NOTIFY latestVersionChanged)
    Q_PROPERTY(QString changelog READ changelog NOTIFY changelogChanged)

public:
    explicit updater(QObject *parent = nullptr);
    ~updater();

    // Méthodes exposées à QML
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void downloadUpdate();
    Q_INVOKABLE void installUpdate();

    // Getters/Setters
    QString currentVersion() const;
    void setCurrentVersion(const QString &version);

    bool updateAvailable() const;
    bool isDownloading() const;
    int downloadProgress() const;
    QString latestVersion() const;
    QString changelog() const;

signals:
    void currentVersionChanged();
    void updateAvailableChanged();
    void isDownloadingChanged();
    void downloadProgressChanged();
    void latestVersionChanged();
    void changelogChanged();
    void updateError(const QString &error);

private slots:
    void handleUpdateAvailable();
    void handleUpdateDownloaded();
    void handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void handleUpdateError(const QString &error);

private:
    QSimpleUpdater *m_updater;
    QString m_currentVersion;
    bool m_updateAvailable;
    bool m_isDownloading;
    int m_downloadProgress;
    QString m_latestVersion;
    QString m_changelog;
};

#endif // UPDATER_H
