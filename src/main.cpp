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
    QString zipFile = "";
    if (argc > 1)
    {
        zipFile = argv[1];
    }


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

// OPENING PASSWORD PROTECTED FILES
//    QuaZip zip("C:/Users/d10dd/Desktop/Test3.zip");
//    bool res = zip.open(QuaZip::mdUnzip);
//    qDebug() << res;
//    zip.goToFirstFile();

//    QuaZipFile file(&zip);
//    QuaZipFileInfo info{};
//    file.getFileInfo(&info);
//    qDebug() << info.flags;
//    bool res2 = file.open(QIODevice::ReadOnly, "password");
//    qDebug() << res2;
//    auto data = file.readAll();
//    qDebug() << data.size();
//    file.close();
//    zip.close();

//    QFile file2("C:/Users/d10dd/Desktop/result.jpg");
//    file2.open(QIODevice::WriteOnly);
//    file2.write(data);
//    file2.close();
