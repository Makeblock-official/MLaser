#include "hexloadbase.h"
#include "qdebug.h"
#include "QCoreApplication"

HexLoadBase::HexLoadBase(QObject *parent) :
    QObject(parent)
{
    download = new QProcess();
}

void HexLoadBase::HexLoad(QString filename ,QString a)
{
    QString path = QCoreApplication::applicationDirPath();
    QStringList po;
    QString arg = tr("-Uflash:w:%1/%2:i").arg(path).arg(filename);
    //UNO para
    //po<<"-C./avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega328p"<<"-carduino"<<comName<<"-b115200"<<"-D"<<arg;
#if defined Q_OS_WIN
    QString comName = "-P"+a;
    po<<"-C"  + path + "/avrdude.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega2560"<<"-cwiring"<<comName<<"-b115200"<<"-D"<<arg;
    download->start(path + "/avrdude.exe",po );
#else
    QString comName = "-P/dev/cu."+a;
    po<<"-C" +  path + "/avrdude.linux.conf"<<"-v"<<"-v"<<"-v"<<"-v"<<"-patmega2560"<<"-cwiring"<<comName<<"-b115200"<<"-D"<<arg;
    download->start(path + "/avrdude",po );
#endif
//    qDebug()<<po;
    download->waitForFinished();
    emit Sig_FinishHexLoad();
}
