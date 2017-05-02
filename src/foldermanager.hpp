#ifndef FOLDERMANAGER_HPP
#define FOLDERMANAGER_HPP

#include <QSharedPointer>

#include "zipitem.hpp"

class FolderManager
{
private:
    QSharedPointer<ZipItem> _root;
    QSharedPointer<ZipItem> _currentFolder;
    QSharedPointer<Zip::Archive> _archive;

    int _currentFileIndex;

    QString _currentFile = "";
public:
    FolderManager();

    inline const int count() const { return _currentFolder->count(); }
    inline const int filesCount() const { return _currentFolder->filesCount(); }
    inline const int foldersCount() const { return _currentFolder->childrenCount(); }
    inline const int currentFileIndex() const { return _currentFileIndex; }
    void setCurrentFileIndex(int index);
    inline void setCurrentFileIndexRaw(int index) { _currentFileIndex = index; }

    inline const QString& currentFile() const { return _currentFile; }
    inline const QString currentFileFullPath() const { return makeSource(currentFile()); }
    inline void setCurrentFile(const QString& file) { _currentFile = file; }
    inline const QString& currentFolderName() const { return _currentFolder->getName(); }
    void setCurrentFolderName(const QString& name);
    const QString& parentFolderName() const;

    inline QSharedPointer<ZipItem> currentFolder() const { return _currentFolder; }
    inline QSharedPointer<ZipItem> root() const { return _root; }

    inline int toFileIndex(int index) const { return toFileIndex(_currentFolder, index); }
    static inline int toFileIndex(QSharedPointer<ZipItem> folder, int index) { return index - folder->childrenCount(); }
    inline const QString makeSource(const QString& name) const { return makeSource(_currentFolder, name); }
    static inline const QString makeSource(QSharedPointer<ZipItem> folder, const QString& name) { return folder->fullPath() + name; }

    void reloadFolderStructure();
    void resetFileInfo();
};

#endif // FOLDERMANAGER_HPP
