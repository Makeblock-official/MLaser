#ifndef ZIPUNZIP_H
#define ZIPUNZIP_H

#include <QObject>

class ZipUnzip : public QObject
{
    Q_OBJECT
public:
    explicit ZipUnzip(QObject *parent = 0);

    bool zip(const QString &dirPath, const QString &zipPath);
    bool unzip(const QString &zipPath, const QString &dirPath);
};

#endif // ZIPUNZIP_H
