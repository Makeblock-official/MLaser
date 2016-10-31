#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include "gsendmodel.h"

GSendModel::GSendModel(QObject *parent) :
    QObject(parent)
{
    layers=0;
    g_cmds=1;
    totaltimes=0;
    currentcmds = 0;
    print_file = "test.gcode";
    target_gcode = new QFile("test.gcode");
    in =new  QTextStream(target_gcode);
}

GSendModel::~GSendModel()
{
    target_gcode->close();
}
void GSendModel::SetTargetFile(QString file)
{
    layers=0;
    g_cmds=1;
    currentcmds = 0;
    totaltimes=0;
    print_file = file;
    target_gcode->close();
    target_gcode->setFileName(print_file);

    //获取文件的bounding
    QVector3D boundsMin( 99999, 99999, 99999);
    QVector3D boundsMax(-99999,-99999,-99999);
    QVector3D _bounds(0,0,0);
    QRegExp regXY(QString("G1 [\s]* *X([0-9]*\.[0-9]*) *Y([0-9]*\.[0-9]*)"));
    QRegExp reg0XY(QString("G0 [\s]* *X([0-9]*\.[0-9]*) *Y([0-9]*\.[0-9]*)"));
    float currentZ = 0.0;
    float currentX = 0.0;
    float currentY = 0.0;
//    QString testreg = "G1 X11.4376 Y1.68076 ";
//    if(testreg.contains(regXY))
//    {
//        qDebug()<<"FFFFFFFFFFFFFFFuck;";
//    }

    if(!target_gcode->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the g-code file!"<<print_file;
    }
    in->setDevice(target_gcode);
    quint64 pos = in->device()->pos();
    QRegExp regLt(QString(";Layer count: *([0-9]*)"));
    QRegExp regL(QString(";LAYER: *([0-9]*)"));
    QRegExp regT(QString(";TIME: *([0-9]*)"));
    while(!(in->atEnd()))
    {
        QString yi = in->readLine();
        if(yi.startsWith("G"))
        {
            g_cmds++;
        }
        if(yi.contains(regLt))
        {
            if((yi.indexOf(regLt))>=0)
            {
                qDebug()<<"total layers is :"<<regLt.cap(1);
            }
        }
        if(yi.contains(regT))
        {
            if((yi.indexOf(regT)))
            {
                qDebug()<<"total time is :"<<regT.cap(1);
            }
        }

        if(yi.contains(regXY))
        {
            int pos = yi.indexOf(regXY);
            if(pos>=0)
            {
                currentX = regXY.cap(1).toFloat();
                currentY = regXY.cap(2).toFloat();
                boundsMin.setX( qMin(boundsMin.x(),currentX));
                boundsMax.setX( qMax(boundsMax.x(), currentX));
                boundsMin.setY(qMin(boundsMin.y(),currentY));
                boundsMax.setY(qMax(boundsMax.y(), currentY));
            }

        }
        else if(yi.contains(reg0XY))
        {
            int pos = yi.indexOf(regXY);
            if(pos>=0)
            {
                currentX = reg0XY.cap(1).toFloat();
                currentY = reg0XY.cap(2).toFloat();
                qDebug()<<"all we get :s"<<reg0XY.cap(1);
                boundsMin.setX( qMin(boundsMin.x(),currentX));
                boundsMax.setX( qMax(boundsMax.x(), currentX));
                boundsMin.setY(qMin(boundsMin.y(),currentY));
                boundsMax.setY(qMax(boundsMax.y(), currentY));
            }

        }
    }
    _bounds = boundsMax - boundsMin;
    m_rects = QRectF(boundsMin.x(),boundsMin.y(),_bounds.x(),_bounds.y());
    in->device()->seek(0);
    qDebug()<<tr("total comands : %1").arg(g_cmds);
}
quint64 GSendModel::getTotalLayer()
{
    return layers;
}
quint64 GSendModel::getTotalGcmd()
{
    return g_cmds;
}
quint64 GSendModel::getTotalTime()
{
    return totaltimes;
}
quint64 GSendModel::getPersent()
{
    return (1+(currentcmds*100.0/g_cmds));
}
QString GSendModel::readOneLine()
{
    QString pi = in->readLine()+"\n";
    bool goon = true;
    //QRegExp reg(QString("([a-zA-Z0-9.\s]*) *;"));
    QRegExp reg(QString("(.*) *;"));

    if(in->atEnd())
    {
        currentLine = "end";
        currentcmds=0;
        return currentLine;
    }
    while(goon&&(!in->atEnd()))
    {
        if((pi[0]==';')||(pi[0]=='\n'))
        {
           pi = in->readLine()+"\n";
        }
        else if(pi.startsWith("M109"))
        {
            pi = in->readLine()+"\n";
        }
        else
        {
            if(pi.contains(reg))
            {
                int pos = pi.indexOf(reg);
                if(pos>=0)
                currentLine = reg.cap(1)+"\n";
            }
            else
            {
                if(pi.startsWith("G"))
                    currentcmds++;
                currentLine = pi;
            }
           goon = false;
        }
    }
//    qDebug()<<"read gcode:"<<currentLine;
    return currentLine;
}
void GSendModel::reset()
{
    currentLine = "";
    currentcmds = 0;
    layers = 0;
    totaltimes = 0;
    g_cmds = 1;
    target_gcode->close();
}

