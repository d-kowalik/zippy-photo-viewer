#ifndef ZIPITEM_HPP
#define ZIPITEM_HPP

#include <QDebug>
#include <QQuickTextureFactory>
#include <QSharedPointer>
#include <QHash>
#include <QObject>

#include "zip/archive.hpp"

class ZipItem : public QObject
{
    Q_OBJECT
private:
    const bool _isRoot;
    QString _name;
    QString _fullPath;

    QSharedPointer<ZipItem> _parent;

    QStringList _files;
    QHash<QString, QSharedPointer<ZipItem>> _folders;

public:
    ZipItem(const QString& name, QSharedPointer<ZipItem> parent=nullptr, bool isRoot=false);

    void addFile(const QString& name);
    void addFolder(QSharedPointer<ZipItem> folder);

    inline const int count() const { return _files.size() + _folders.size(); }

    inline const bool isRoot() const { return _isRoot; }
    inline const bool contains(const QString& name) const { return _folders.contains(name); }

    inline const QString& getName() const { return _name; }
    inline const QString& fullPath() const { return _fullPath; }
    inline const QString& getFile(int index) const { return _files[index]; }

    inline const QSharedPointer<ZipItem> getFolder(const QString& name) const { return _folders[name]; }
    inline const QSharedPointer<ZipItem> getParent() const { return _parent; }

    inline const QStringList& getFiles() const { return _files; }
    inline const QHash<QString, QSharedPointer<ZipItem>> getFolders() const { return _folders; }


    static inline QSharedPointer<ZipItem> createRoot() { return QSharedPointer<ZipItem>::create("/", nullptr, true); }
};

#endif // ZIPITEM_HPP
