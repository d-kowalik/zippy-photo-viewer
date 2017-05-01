#ifndef ZIPITEMROLEMANAGER_H
#define ZIPITEMROLEMANAGER_H

#include <QHash>
#include <QString>
#include <QSharedPointer>
#include <QVariant>

#include "zipitemmodel.hpp"

class ZipItemRoleManager
{
private:
    enum ZipItemRole { SOURCE, NAME, TYPE };

    QSharedPointer<ZipItemModel> _model;
    QHash<ZipItemRole, QString> _roles;

public:
    ZipItemRoleManager(QSharedPointer<ZipItemModel> model) : _model(model)
    {
        _roles = QHash<ZipItemRole, QString> {
            { SOURCE, "source" },
            { NAME, "name" },
            { TYPE, "type" }
        };
    }

    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> result;
        result.reserve(_roles.size());
        for (const auto& k : _roles.keys())
        {
            const QString& s = _roles.value(k);
            result.insert(k, QByteArray(s.toStdString().data(), s.size()));
        }
        return result;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        bool isFolder = index.row() < _model->_currentFolder->getFolders().count();
        if (role == ZipItemRole::SOURCE)
        {
            if (isFolder)
            {
                auto& folderName = _model->_currentFolder->getFolders().keys()[index.row()];
                return QVariant(folderName);
            }

            return QVariant(_model->makeSource(_model->_currentFolder->getFile(_model->toFileIndex(index.row()))));
        }
        else if (role == ZipItemRole::TYPE)
        {
            if (isFolder)
            {
                return QVariant("Folder");
            }
            return QVariant("Thumbnail");
        }
        else if (role == ZipItemRole::NAME)
        {
            if (isFolder)
            {
                auto& folderName = _model->_currentFolder->getFolders().keys()[index.row()];
                return QVariant(folderName);
            }

            return QVariant(_model->_currentFolder->getFile(_model->toFileIndex(index.row())));
        }

        qDebug() << "Error! No such role!";
        return QVariant();
    }

};

#endif // ZIPITEMROLEMANAGER_H
