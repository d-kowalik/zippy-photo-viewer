#include "zipitemmodel.hpp"

ZipItemModel::ZipItemModel(QSharedPointer<Zip::Archive> archive, QObject* parent)
    : QAbstractListModel(parent), _roleManager{ModelRoleManager()}, _folderManager(FolderManager(archive))
{

}

void ZipItemModel::setCurrentImageIndex(int index)
{
    _folderManager.setCurrentFileIndex(index);
    emit currentImageIndexChanged();
}

void ZipItemModel::setCurrentImageIndexRaw(int index)
{
    _folderManager.setCurrentFileIndexRaw(index);
    emit currentImageIndexChanged();
}

void ZipItemModel::setCurrentFile(const QString& file)
{
    _folderManager.setCurrentFile(file);
    emit currentFileFullPathChanged();
    emit currentFileChanged();
}

void ZipItemModel::setCurrentFolderName(const QString& name)
{
    _folderManager.setCurrentFolderName(name);

    emit currentFolderNameChanged();
    refresh();
}

const QString& ZipItemModel::parentFolderName() const
{
    return _folderManager.parentFolderName();
}

QVariant ZipItemModel::data(const QModelIndex &index, int role) const
{
    return _roleManager.data(_folderManager.currentFolder(), index, role);
}

QHash<int, QByteArray> ZipItemModel::roleNames() const
{
    return _roleManager.roleNames();
}

void ZipItemModel::reloadFolderStructure()
{
    _folderManager.reloadFolderStructure();
}

void ZipItemModel::refresh()
{
    this->beginResetModel();
    this->resetInternalData();
    _folderManager.resetFileInfo();
    this->endResetModel();
}
