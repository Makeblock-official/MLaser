#include "hexloadbase.h"
#include "qdebug.h"

HexLoadBase::HexLoadBase(QObject *parent) :
    QObject(parent)
{
    download = new QProcess();
}

void HexLoadBase::HexLoad(QString filename ,QString a)
{
    QStringList po;
    QString arg = tr("-Uflash:w:%1:i").arg(filename);
    //UNO para
    //po<<"-C./avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega328p"<<"-carduino"<<comName<<"-b115200"<<"-D"<<arg;
#if define Q_OS_DARWIN || define Q_OS_DARWIN64
    QString comName = "-P"+a;
    po<<"-C./avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega2560"<<"-cwiring"<<comName<<"-b115200"<<"-D"<<arg;
    download->start("./avrdude.exe",po );
#else
    QString comName = "-P/dev/cu."+a;
    po<<"-C./avrdude.linux.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega2560"<<"-cwiring"<<comName<<"-b115200"<<"-D"<<arg;
    download->start("./avrdude",po );
#endif
//    qDebug()<<po;
    download->waitForFinished();
    emit Sig_FinishHexLoad();
}
