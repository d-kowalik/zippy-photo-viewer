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

#include <quazipfile.h>

#include "zip/archive.hpp"
#include "zipimageprovider.hpp"
#include "zipitemmodel.hpp"

#include "borderless-window/MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QGuiApplication application(argc, argv);
    // Background color
    HBRUSH windowBackground = CreateSolidBrush( RGB( 63, 81, 181 ) );

    // Create window
    BorderlessWindow window( &application, windowBackground, 100, 100, 1024, 768 );
    window.setMinimumSize( 800, 600 );

    // Launch
    application.exec();
}
