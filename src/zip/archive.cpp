#include "archive.hpp"

#include <QImage>
#include <QDebug>

#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>

namespace Zip
{
Archive::Archive(QString path)
{
    _supportedFiles = QStringList{ "jpg", "png" };
    setPath(path);
}

QImage Archive::getImage(const QString& id)
{
    if (id == "") return QImage();
    if (id == _currentImageId) return _currentImageCache;

    _zip->setCurrentFile(id);

    QuaZipFile file(&*_zip);

    bool result;
    if (_passwordRequired)
        result = file.open(QIODevice::ReadOnly, _password.toStdString().data());
    else
        result = file.open(QIODevice::ReadOnly);

    if (result)
    {
    }
    else
    {
        qDebug() << "File " << file.getActualFileName() << " failed to open.";
    }

    auto bytes = file.readAll();
    QuaZipFileInfo info;
    file.getFileInfo(&info);
    qDebug() << "Flags: " << info.flags;
    QImage image = QImage::fromData(bytes);

    _currentImageCache = image;
    _currentImageId = id;

    return image;
}

void Archive::setPassword(const QString& password)
{
    _password = password;

    auto zip = std::make_unique<QuaZip>(_newPath);
    bool result = zip->open(QuaZip::mdUnzip);
    if (!result)
    {
        emit failedToOpen();
        return;
    }

    for (bool f = zip->goToFirstFile(); f; f = zip->goToNextFile())
    {
        QString fileName = zip->getCurrentFileName();
        if (isSupported(fileName))
        {
            QuaZipFile file(&*zip);
            file.open(QIODevice::ReadOnly, _password.toStdString().data());
            if (file.readAll().size() > 0) {
                _passwordRequired = true;
                emit passwordCorrect();
            }
            else
                emit passwordIncorrect();
            return;
        }
    }
}

bool Archive::initFile(const QString& path)
{
    auto zip = std::make_unique<QuaZip>(path);
    bool result = zip->open(QuaZip::mdUnzip);
    if (!result)
    {
        emit failedToOpen();
        return false;
    }

    _fileNames = QStringList();
    for (bool f = zip->goToFirstFile(); f; f = zip->goToNextFile())
    {
        QuaZipFileInfo info;
        zip->getCurrentFileInfo(&info);

        if (info.flags == 1 && !_passwordRequired) {
            emit passwordRequired();
            return false;
        }

        QString fileName = zip->getCurrentFileName();
        if (isSupported(fileName))
        {
            qDebug() << "File name in init: " << fileName;
            _count++;
            _fileNames.append(fileName);
        }
    }

    _zip = std::move(zip);
    return true;
}

void Archive::setPath(const QString& newPath)
{
    if (_newPath != newPath)
    {
        _passwordRequired = false;
    }
    _newPath = newPath;
    if (initFile(newPath))
    {
        _path = newPath;
        emit pathChanged();
    }
}

bool Archive::isSupported(const QString& fileName) const
{
    return _supportedFiles.contains(fileName.split(".").last());
}
}
