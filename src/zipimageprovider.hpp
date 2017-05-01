#ifndef ZIPIMAGEPROVIDER_H
#define ZIPIMAGEPROVIDER_H

#include <QQuickImageProvider>

#include <memory>

#include "zip/archive.hpp"

class ZipImageProvider : public QQuickImageProvider
{
public:
    QSharedPointer<Zip::Archive> archive;

    ZipImageProvider(QSharedPointer<Zip::Archive> archive);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    const QStringList getFileNames() const;
};

#endif // ZIPIMAGEPROVIDER_H
