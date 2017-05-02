#ifndef MODELROLEMANAGER_H
#define MODELROLEMANAGER_H

#include <QSharedPointer>
#include <QModelIndex>
#include <QString>
#include <QHash>
#include <QVariant>

#include "zipitem.hpp"

class ModelRoleManager
{
private:
    enum ItemRole { NAME, SOURCE, TYPE };

    QHash<ItemRole, QString> _roleNames;

public:
    ModelRoleManager();

    QVariant data(QSharedPointer<ZipItem> folder, const QModelIndex &fullIndex, int role) const;

    QHash<int, QByteArray> roleNames() const;
};

#endif // MODELROLEMANAGER_H
