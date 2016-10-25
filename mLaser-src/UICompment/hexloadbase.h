#ifndef HEXLOADBASE_H
#define HEXLOADBASE_H

#include <QObject>
#include <QProcess>

class HexLoadBase : public QObject
{
    Q_OBJECT
public:
    explicit HexLoadBase(QObject *parent = 0);
public slots:
    void HexLoad(QString ,QString);
private:
    QProcess *download;
signals:
    void Sig_FinishHexLoad();
};

#endif // HEXLOADBASE_H
