#include "zipitemmodel.hpp"

ZipItemModel::ZipItemModel(QSharedPointer<Zip::Archive> archive, QObject* parent)
    : QAbstractListModel(parent), _archive(archive), _roleManager{ModelRoleManager()}
{
    reloadFolderStructure();
}

void ZipItemModel::setCurrentImageIndex(int index)
{
    auto newIndex = toFileIndex(index);
    _currentImageIndex = newIndex;
    emit currentImageIndexChanged();
}

void ZipItemModel::setCurrentImageIndexRaw(int index)
{
    _currentImageIndex = index;
    emit currentImageIndexChanged();
}

void ZipItemModel::setCurrentFile(const QString& file)
{
    _currentFile = file;
    emit currentFileFullPathChanged();
    emit currentFileChanged();
}

void ZipItemModel::setCurrentFolderName(const QString& name)
{
    auto newFolder = _currentFolder->getFolder(name);
    if (newFolder.isNull())
    {
        newFolder = _currentFolder->getParent();
    }
    _currentFolder = newFolder;

    emit currentFolderNameChanged();
    refresh();
}

const QString& ZipItemModel::parentFolderName() const
{
    if (_currentFolder->getParent() != nullptr)
    {
        return _currentFolder->getParent()->getName();
    }
    return _currentFolder->getName();
}

QVariant ZipItemModel::data(const QModelIndex &index, int role) const
{
    return _roleManager.data(_currentFolder, index, role);
}

QHash<int, QByteArray> ZipItemModel::roleNames() const
{
    return _roleManager.roleNames();
}

const QString ZipItemModel::makeSource(const QString& name) const
{
    return makeSource(_currentFolder, name);
}

const QString ZipItemModel::makeSource(QSharedPointer<ZipItem> folder, const QString &name)
{
    return folder->fullPath() + name;
}

void ZipItemModel::reloadFolderStructure()
{
    _root = ZipItem::createRoot();
    _currentFolder = _root;

    for (const auto& name : _archive->fileNames())
    {
        if (name.contains("/"))
        {
            QStringList parts = name.split("/");
            QSharedPointer<ZipItem> root = _root;
            for (int i = 0; i < parts.length(); i++)
            {
                auto s = parts[i];
                if (i == parts.length() - 1)
                {
                    root->addFile(s);
                    // root->addFile(name);
                }
                else
                {
                    if (root->contains(s))
                    {
                        root = root->getFolder(s);
                    }
                    else
                    {
                        auto newRoot = QSharedPointer<ZipItem>::create(s, root);
                        root->addFolder(newRoot);
                        root = newRoot;
                    }
                }
            }
        }
        else
        {
            _root->addFile(name);
        }
    }
    resetFileInfo();
}

void ZipItemModel::refresh()
{
    this->beginResetModel();
    this->resetInternalData();
    resetFileInfo();
    this->endResetModel();
}

void ZipItemModel::resetFileInfo()
{
    setCurrentImageIndex(folderCount());
    if (this->imageCount() != 0)
        setCurrentFile(_currentFolder->getFile(0));
    else
        setCurrentFile("");
    emit imageCountChanged();
}

void ZipItemModel::decrementCurrentImageIndex()
{
    auto newIndex = currentImageIndex();
    if (newIndex == 0)
        newIndex = imageCount() - 1;
    else
        newIndex--;
    setCurrentImageIndexRaw(newIndex);
}

void ZipItemModel::incrementCurrentImageIndex()
{
    auto newIndex = currentImageIndex();
    if (newIndex == imageCount() - 1)
        newIndex = 0;
    else
        newIndex++;
    setCurrentImageIndexRaw(newIndex);
}

void ZipItemModel::goToNextImage()
{
    incrementCurrentImageIndex();
    setCurrentFile(_currentFolder->getFile(_currentImageIndex));
}

void ZipItemModel::goToPreviousImage()
{
    decrementCurrentImageIndex();
    setCurrentFile(_currentFolder->getFile(_currentImageIndex));
}
