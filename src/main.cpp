#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickImageProvider>
#include <QQmlContext>
#include <QQuickTextureFactory>
#include <QQuickImageResponse>
#include <QQuickTextureFactory>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QHash>
#include <QObject>
#include <QQuickWindow>
#include <QAbstractNativeEventFilter>

#include <iostream>

#include <quazipfile.h>

#include "zip/archive.hpp"
#include "zipimageprovider.hpp"
#include "zipitemmodel.hpp"

#include "borderless-window/MainWindow.h"

int main(int argc, char *argv[])
{
    std::cout << "TEST!" << std::endl;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication application(argc, argv);


    QString zipFile = "";
    if (QGuiApplication::arguments().length() > 1) {
        zipFile = QGuiApplication::arguments()[1];
    }
    auto archive = QSharedPointer<Zip::Archive>::create(zipFile);
    ZipImageProvider* provider = new ZipImageProvider(archive);
    ZipItemModel* model = new ZipItemModel(archive);

    QMainPanel* panel = new QMainPanel();
    panel->setSize(1024, 768);
    panel->rootContext()->setContextProperty("archive", archive.data());
    panel->rootContext()->setContextProperty("myModel", model);
    panel->engine()->addImageProvider(QLatin1String("zipimageprovider"), provider);
    panel->setSource(QUrl("qrc:/main.qml"));


    // Background color
    HBRUSH windowBackground = CreateSolidBrush( RGB( 63, 81, 181 ) );
    // Create window
    BorderlessWindow window( &application, panel, windowBackground, 100, 100, 1024, 768 );
    window.setMinimumSize( 800, 600 );

    // Launch
    application.exec();
}
