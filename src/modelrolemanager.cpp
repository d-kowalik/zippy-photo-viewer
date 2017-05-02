#include "modelrolemanager.hpp"

#include "foldermanager.hpp"

ModelRoleManager::ModelRoleManager()
{
    _roleNames = QHash<ItemRole, QString>{
        { NAME, "name" },
        { SOURCE, "source" },
        { TYPE, "type" }
    };

}

QVariant ModelRoleManager::data(QSharedPointer<ZipItem> folder, const QModelIndex &fullIndex, int role) const
{
    auto index = fullIndex.row();
    bool isFolder = index < folder->childrenCount();

    if (role == ItemRole::SOURCE)
    {
        if (isFolder)
        {
            auto& folderName = folder->getFolderName(index);
            return QVariant(folderName);
        }

        return QVariant(FolderManager::makeSource(folder, folder->getFile(FolderManager::toFileIndex(folder, index))));
    }
    else if (role == ItemRole::TYPE)
    {
        if (isFolder)
        {
            return QVariant("Folder");
        }
        return QVariant("Thumbnail");
    }
    else if (role == ItemRole::NAME)
    {
        if (isFolder)
        {
            auto& folderName = folder->getFolderName(index);
            return QVariant(folderName);
        }

        return QVariant(folder->getFile(FolderManager::toFileIndex(folder, index)));
    }

    qDebug() << "Error! No such role!";
    return QVariant();
}

QHash<int, QByteArray> ModelRoleManager::roleNames() const
{
    QHash<int, QByteArray> result;
    for (const auto& k : _roleNames.keys())
        result.insert(static_cast<int>(k), QByteArray(_roleNames.value(k).toStdString().data(), _roleNames.value(k).size()));
    return result;
}
