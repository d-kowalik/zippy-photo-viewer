#ifndef ZIPITEMMODEL_H
#define ZIPITEMMODEL_H

#include <QDebug>
#include <QQuickTextureFactory>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QHash>
#include <QObject>

#include <memory>

#include "zip/archive.hpp"
#include "zipitem.hpp"
#include "modelrolemanager.hpp"
#include "foldermanager.hpp"

class ZipItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentFolderName READ currentFolderName WRITE setCurrentFolderName NOTIFY currentFolderNameChanged)
    Q_PROPERTY(QString parentFolderName READ parentFolderName)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int imageCount READ imageCount NOTIFY imageCountChanged)
    Q_PROPERTY(int currentImageIndex READ currentImageIndex WRITE setCurrentImageIndex NOTIFY currentImageIndexChanged)
    Q_PROPERTY(QString currentFile READ currentFile WRITE setCurrentFile NOTIFY currentFileChanged)
    Q_PROPERTY(QString currentFileFullPath READ currentFileFullPath NOTIFY currentFileFullPathChanged)

private:
    ModelRoleManager _roleManager;
    FolderManager _folderManager;

    enum ItemModelRole { SOURCE, NAME, TYPE };

public:
    ZipItemModel(QSharedPointer<Zip::Archive> archive, QObject* parent=nullptr);

    inline const int count() const { return _folderManager.count(); }
    inline const int imageCount() const { return _folderManager.filesCount(); }
    inline const int folderCount() const { return _folderManager.foldersCount(); }
    inline const int currentImageIndex() const { return _folderManager.currentFileIndex(); }
    void setCurrentImageIndex(int index);
    void setCurrentImageIndexRaw(int index);


    inline const QString& currentFile() const { return _folderManager.currentFile(); }
    inline const QString currentFileFullPath() const { return _folderManager.currentFileFullPath(); }
    void setCurrentFile(const QString& file);
    inline const QString& currentFolderName() const { return _folderManager.currentFolderName(); }
    void setCurrentFolderName(const QString& name);
    const QString& parentFolderName() const;

    inline QSharedPointer<ZipItem> getCurrentFolder() const { return _folderManager.currentFolder(); }
    inline QSharedPointer<ZipItem> getRoot() const { return _folderManager.root(); }

    // Super implementation
    inline int rowCount(const QModelIndex &parent) const override { return count(); }
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void currentFolderNameChanged();
    void currentImageIndexChanged();
    void currentFileChanged();
    void folderStructureChanged();
    void imageCountChanged();
    void currentFileFullPathChanged();

public slots:
    void reloadFolderStructure();
    void refresh();
    void goToNextImage();
    void goToPreviousImage();
};

#endif // ZIPITEMMODEL_H
