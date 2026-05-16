#include "updater.h"
#include <QDebug>

updater::updater(QObject *parent)
    : QObject(parent),
    m_updater(new QSimpleUpdater(this)),
    m_currentVersion("1.0.0"),
    m_updateAvailable(false),
    m_isDownloading(false),
    m_downloadProgress(0)
{
    // Configure QSimpleUpdater
    m_updater->setModuleVersion(m_currentVersion);
    m_updater->setNotifyOnFinish(false); // On gère nous-mêmes les notifications
    m_updater->setNotifyOnUpdateAvailable(false);
    m_updater->setDownloaderEnabled(true);

    // URL du fichier version.json
    m_updater->setUrl("https://raw.githubusercontent.com/ton-utilisateur/MuseStream/main/version.json");

    // Connecte les signaux de QSimpleUpdater
    connect(m_updater, &QSimpleUpdater::updateAvailable, this, &updater::handleUpdateAvailable);
    connect(m_updater, &QSimpleUpdater::updateDownloaded, this, &updater::handleUpdateDownloaded);
    connect(m_updater, &QSimpleUpdater::downloadProgress, this, &updater::handleDownloadProgress);
    connect(m_updater, &QSimpleUpdater::errorOccurred, this, &updater::handleUpdateError);
}

updater::~updater()
{
    delete m_updater;
}

// Méthodes exposées à QML
void updater::checkForUpdates()
{
    m_isDownloading = false;
    emit isDownloadingChanged();
    m_updater->checkForUpdates();
}

void updater::downloadUpdate()
{
    if (m_updateAvailable) {
        m_isDownloading = true;
        emit isDownloadingChanged();
        m_updater->downloadUpdate();
    }
}

void updater::installUpdate()
{
    // Logique pour installer la mise à jour (extraire, remplacer les fichiers, etc.)
    QString downloadPath = m_updater->getDownloadedFilePath();
    qDebug() << "Installation de la mise à jour depuis :" << downloadPath;

    // Exemple : Extraire et remplacer les fichiers (à adapter)
    QProcess *extractProcess = new QProcess(this);
    QString extractDir = QCoreApplication::applicationDirPath() + "/update_temp";
    QDir().mkdir(extractDir);

    // Utilise unzip (Linux/macOS) ou 7z (Windows)
    extractProcess->start("unzip", {downloadPath, "-d", extractDir});

    connect(extractProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, extractDir, downloadPath](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitCode == 0) {
                    // Copie les fichiers extraits
                    QDir dir(extractDir);
                    for (const QString &file : dir.entryList(QDir::Files)) {
                        QString destPath = QCoreApplication::applicationDirPath() + "/" + file;
                        if (QFile::exists(destPath)) {
                            QFile::remove(destPath);
                        }
                        QFile::copy(extractDir + "/" + file, destPath);
                        QFile::setPermissions(destPath, QFile::Permissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner));
                    }

                    // Nettoyage
                    QDir().rmdir(extractDir);
                    QFile::remove(downloadPath);

                    emit changelogChanged(); // Notifie QML que l'installation est terminée
                } else {
                    emit updateError("Échec de l'extraction de l'archive.");
                }
            });
}

// Getters/Setters
QString updater::currentVersion() const
{
    return m_currentVersion;
}

void updater::setCurrentVersion(const QString &version)
{
    if (m_currentVersion != version) {
        m_currentVersion = version;
        emit currentVersionChanged();
    }
}

bool updater::updateAvailable() const
{
    return m_updateAvailable;
}

bool updater::isDownloading() const
{
    return m_isDownloading;
}

int updater::downloadProgress() const
{
    return m_downloadProgress;
}

QString updater::latestVersion() const
{
    return m_latestVersion;
}

QString updater::changelog() const
{
    return m_changelog;
}

// Handlers pour QSimpleUpdater
void updater::handleUpdateAvailable()
{
    m_latestVersion = m_updater->getLatestVersion();
    m_changelog = m_updater->getChangelog();
    m_updateAvailable = true;
    emit updateAvailableChanged();
    emit latestVersionChanged();
    emit changelogChanged();
}

void updater::handleUpdateDownloaded()
{
    m_isDownloading = false;
    emit isDownloadingChanged();
    emit changelogChanged(); // Notifie que le téléchargement est terminé
}

void updater::handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_downloadProgress = (bytesReceived * 100) / bytesTotal;
    emit downloadProgressChanged();
}

void updater::handleUpdateError(const QString &error)
{
    m_isDownloading = false;
    emit isDownloadingChanged();
    emit updateError(error);
}