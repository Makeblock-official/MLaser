#include "hexloadbase.h"
#include "QDebug"
HexLoadBase::HexLoadBase(QObject *parent) :
    QObject(parent)
{
    download = new QProcess();
}

void HexLoadBase::HexLoad(QString filename ,QString a)
{
    QStringList po;
    QString arg = tr("-Uflash:w:%1:i").arg(filename);
    QString comName = "-P"+a;
    //UNO para
    //po<<"-C./avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega328p"<<"-carduino"<<comName<<"-b115200"<<"-D"<<arg;
    po<<"-C./avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega2560"<<"-cwiring"<<comName<<"-b115200"<<"-D"<<arg;
    download->start("./avrdude.exe",po);
//    qDebug()<<po;
    download->waitForFinished();
   // qDebug()<<"aaaa"<<download->waitForFinished();
    emit Sig_FinishHexLoad();
}
