#include "zipimageprovider.hpp"

ZipImageProvider::ZipImageProvider(QSharedPointer<Zip::Archive> archive)
    : QQuickImageProvider(QQuickImageProvider::Image), archive(archive)
{
}

QImage ZipImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = archive->getImage(id);

    if (requestedSize.isValid())
    {
        image = image.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        size->setWidth(image.width());
        size->setHeight(image.height());
    }

    return image;
}

const QStringList ZipImageProvider::getFileNames() const
{
    return archive->fileNames();
}
