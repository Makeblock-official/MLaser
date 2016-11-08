#include <QSettings>
#include <QDebug>
#include "svg2gcodep.h"
#include "QCoreApplication"

Svg2GcodeP::Svg2GcodeP(QWidget *parent) :
    QWidget(parent)
{

}

//---the firmare is marlin, here we use gcode to control the
//---machine.So,we can setting the gcoder's header to change
//---the XY's speed,power and other parament.
void Svg2GcodeP::SetGcoderHeader(QString str)
{
    setPrintHead(str.toLatin1().data());
}

void Svg2GcodeP::SetCloseLine(bool a)
{
    if(a)//文字
    {
        setCloseLines(1);
        qDebug()<<tr("这是文字！");
    }
    else    //非文字
    {
        setCloseLines(0);
    }
}
//set the print power of the laser from 0 to 255
void Svg2GcodeP::SetLaser(QString p)
{
    setPrintLaster(p.toLatin1().data());
}

//set print width
void Svg2GcodeP::SetgGcodeWidth(int w)
{
    setPrintWidth(w);
}

//the interface of svg to gcode.
void Svg2GcodeP::SvgToGcode(QString inputfile,QString outfile)
{
	QString path = QCoreApplication::applicationDirPath();
    QString name_config = "/Config.ini";
	QString name_meterial = "/material.ini";
	QString config_allPath = QString("%1%2").arg(path).arg(name_config);
	QString meterial_allPath = QString("%1%2").arg(path).arg(name_meterial);
    QSettings* psetting = new QSettings(config_allPath,QSettings::IniFormat);
    psetting->beginGroup("laser");
    QString type = psetting->value("material").toString();
    psetting->endGroup();

    QSettings* tpsetting = new QSettings(meterial_allPath,QSettings::IniFormat);
    tpsetting->beginGroup(type);
    int laserTravelSpeed =  tpsetting->value("laserTravelSpeed").toInt();
    int laserSpeed =  tpsetting->value("laserSpeed").toInt();
    QString laserPower = QString::number(2.4*(tpsetting->value("laserPowerHigh").toDouble()));
    tpsetting->endGroup();

    laserPower = "M4 P"+laserPower+"\n";
    SetSpeed(laserSpeed,laserTravelSpeed);
    SetLaser(laserPower);
//    SetGcoderHeader(tr("G0 F%1 \n").arg(laserspeed));
    svgToGcode(inputfile.toLatin1().data(),outfile.toLatin1().data(),1);
}

void Svg2GcodeP::SetSpeed(float s,float travel)
{
    setPrintSpeed(s);
    setTravelSpeed(travel);
}
