#include "zipunzip.h"
//#include "quazip/quazip.h"
//#include "quazip/quazipfile.h"
#include <QDir>
#include "kzip.h"

ZipUnzip::ZipUnzip(QObject *parent) :
    QObject(parent)
{
}


bool ZipUnzip::zip(const QString &dirPath, const QString &zipPath)
{
    KZip archive(zipPath);
    if(!archive.open(QIODevice::WriteOnly)) {
        qWarning("Cannot create " + zipPath.toLatin1());
        return false;
    }
    //archive.writeDir(dirPath);
    archive.addLocalDirectory(dirPath, "");

    archive.close();
    return true;
}

bool ZipUnzip::unzip(const QString &zipPath, const QString &dirPath)
{
    KZip archive(zipPath);
// Open the archive
    if (!archive.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open " + zipPath.toLatin1());
        qWarning("Is it a valid zip file?");
        return false;
    }

    // Take the root folder from the archive and create a KArchiveDirectory object.
    // KArchiveDirectory represents a directory in a KArchive.
    const KArchiveDirectory *root = archive.directory();

    // We can extract all contents from a KArchiveDirectory to a destination.
    // recursive true will also extract subdirectories.
    QString destination = dirPath;
    bool recursive = true;
    root->copyTo(destination, recursive);

    archive.close();
    return true;
}
