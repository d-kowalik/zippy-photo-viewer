#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include <QObject>

#include <memory>

#include <quazip.h>

namespace Zip
{
class Archive : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QStringList fileNames READ fileNames NOTIFY fileNamesChanged)
    Q_PROPERTY(QString password WRITE setPassword)
private:
    QString _path = "";
    QString _newPath;
    std::unique_ptr<QuaZip> _zip;
    int _count = 0;
    QStringList _fileNames;
    QStringList _supportedFiles;
    bool _passwordRequired = false;
    QString _password;
public:
    Archive(QString path);

    QImage getImage(const QString& id);

    inline int getCount() const { return _count; }
    inline const QStringList& fileNames() const { return _fileNames; }

    inline const QString path() const { return _path; }
    void setPath(const QString& newPath);
    void setPassword(const QString& password);

    bool initFile(const QString& path);

private:
    bool isSupported(const QString& fileName) const;

signals:
    void pathChanged();
    void fileNamesChanged();
    void failedToOpen();
    void passwordRequired();
    void passwordCorrect();
    void passwordIncorrect();
};
}

#endif // ARCHIVE_HPP
