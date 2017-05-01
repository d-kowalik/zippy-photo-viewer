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
    QSharedPointer<ZipItem> _root;
    QSharedPointer<ZipItem> _currentFolder;
    QSharedPointer<Zip::Archive> _archive;

    int _currentImageIndex;

    QString _currentFile = "";

    enum ItemModelRole { SOURCE, NAME, TYPE };

public:
    ZipItemModel(QSharedPointer<Zip::Archive> archive, QObject* parent=nullptr);

    inline const int count() const { return _currentFolder->count(); }
    inline const int imageCount() const { return _currentFolder->getFiles().count(); }
    inline const int folderCount() const { return _currentFolder->getFolders().count(); }
    inline const int currentImageIndex() const { return _currentImageIndex; }
    void setCurrentImageIndex(int index);
    void setCurrentImageIndexRaw(int index);


    inline const QString& currentFile() const { return _currentFile; }
    inline const QString currentFileFullPath() const { return makeSource(currentFile()); }
    void setCurrentFile(const QString& file);
    inline const QString& currentFolderName() const { return _currentFolder->getName(); }
    void setCurrentFolderName(const QString& name);
    const QString& parentFolderName() const;

    inline QSharedPointer<ZipItem> getCurrentFolder() const { return _currentFolder; }
    inline QSharedPointer<ZipItem> getRoot() const { return _root; }

    inline int toFileIndex(int index) const { return index - folderCount(); }

    // Super implementation
    inline int rowCount(const QModelIndex &parent) const override { return count(); }
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    const QString makeSource(const QString& name) const;

private:
    void resetFileInfo();

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
    void decrementCurrentImageIndex();
    void incrementCurrentImageIndex();
    void goToNextImage();
    void goToPreviousImage();
};

#endif // ZIPITEMMODEL_H
