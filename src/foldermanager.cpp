#include "foldermanager.hpp"

FolderManager::FolderManager(QSharedPointer<Zip::Archive> archive) : _archive(archive)
{
    reloadFolderStructure();
}

void FolderManager::setCurrentFileIndex(int index)
{
    auto newIndex = toFileIndex(index);
    _currentFileIndex = newIndex;
}

void FolderManager::setCurrentFolderName(const QString &name)
{
    auto newFolder = _currentFolder->getFolder(name);
    if (newFolder.isNull())
    {
        newFolder = _currentFolder->getParent();
    }
    _currentFolder = newFolder;
}

const QString& FolderManager::parentFolderName() const
{
    if (_currentFolder->getParent() != nullptr)
    {
        return _currentFolder->getParent()->getName();
    }
    return _currentFolder->getName();
}

void FolderManager::reloadFolderStructure()
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

void FolderManager::resetFileInfo()
{
    setCurrentFileIndex(foldersCount());
    if (this->filesCount() != 0)
        setCurrentFile(_currentFolder->getFile(0));
    else
        setCurrentFile("");
}

