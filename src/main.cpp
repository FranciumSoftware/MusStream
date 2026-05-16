#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    backEnd backend;
    QQmlApplicationEngine engine;
    QIcon appIcon("qrc:/images/Assets/Logo.png");
    app.setWindowIcon(appIcon);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [&](QObject *obj, const QUrl &objUrl) {
            if (!obj) {
                QCoreApplication::exit(-1);
            }
            QQuickWindow *window = qobject_cast<QQuickWindow*>(obj);
            if (window) {
                window->setIcon(QIcon("qrc:/images/Assets/Logo.png"));
            }
        },
        Qt::QueuedConnection
        );
    QMediaPlayer mediaPlayer;
    QAudioOutput audioOutput;
    mediaPlayer.setAudioOutput(&audioOutput);

    engine.rootContext()->setContextProperty("globalMediaPlayer", &mediaPlayer);
    engine.rootContext()->setContextProperty("cppManager", &backend);
    engine.loadFromModule("MusStream", "Main");

    return app.exec();
}
