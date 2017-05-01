#include "zipitem.hpp"

ZipItem::ZipItem(const QString& name, QSharedPointer<ZipItem> parent, bool isRoot)
    : _isRoot(isRoot), _name(name), _parent(parent)
{
    if (_parent != nullptr)
    {
        _fullPath = _parent->fullPath() + _name + "/";
    }
    else
    {
        _fullPath = _name + (_isRoot ? "" : "/");
    }
    qDebug() << "Full path: " << _fullPath;
}

void ZipItem::addFile(const QString& name)
{
    _files.push_back(name);
}

void ZipItem::addFolder(QSharedPointer<ZipItem> folder)
{
    _folders.insert(folder->getName(), folder);
}
