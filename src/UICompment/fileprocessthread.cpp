#include "fileprocessthread.h"
//#include <QDebug>

FileProcessThread::FileProcessThread(QObject *parent) :
    QObject(parent)
{
    //文件处理模块
    b2s = new Bitmap2Svg();
    b2g = new Bitmap2Gcode();
    s2g = new Svg2GcodeP();
    gcode = new GModel();

}

//轮廓模式打印
void FileProcessThread::slotCuttingFile(PGraphicsScene*pScene)
{
    QFile::remove("show.gcode");
    QStringList filelist;
    for(int i=0;i<pScene->m_svgItems.size();i++)
    {
        QString filename = pScene->m_svgfilenames.at(i);
        QRectF m = pScene->m_svgItems.at(i)->getRect();
        QPointF f = pScene->m_svgItems.at(i)->pos();
        QPointF r_pos(m.x(),m.y());
        f = r_pos + f -QPointF(30,30);

        QString p = tr("svg_%1.gcode").arg(i);
        QString outname = tr("printsvg_%1.gcode").arg(i);
        filelist.append(outname);
//        s2g->SetgGcodeWidth(m.width());
        s2g->SetCloseLine(false);
        s2g->SvgToGcode(filename,p);
        QPointF k(m.width(),m.height());
        gcode->filterGcode(p,f,k,outname);
        QFile::remove(p);
    }
    for(int j = 0;j<pScene->m_picItems.size();j++)
    {
        QString filename = pScene->m_picfilenames.at(j);
        QPointF f = pScene->m_picItems.at(j)->pos()-QPointF(30,30);
        QRectF m = pScene->m_picItems.at(j)->getRect();
        QPointF r_pos(m.x(),m.y());
        f = f + r_pos;

        QString p = tr("pic_%1.gcode").arg(j);
        qDebug()<<p;
        QString outname = tr("printpic_%1.gcode").arg(j);

        b2s->SetBintmap(filename);
        b2s->OutPutSvg("out.svg");
        QPointF k(m.width(),m.height());
        QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
        psetting->beginGroup("laser");
        float laserspeed =  psetting->value("laserSpeed").toFloat();
        psetting->endGroup();

        if(pScene->m_picItems.at(j)->bFont)
        {
            s2g->SetCloseLine(true);
        }
        else
        {
            s2g->SetCloseLine(false);
        }
        s2g->SvgToGcode(QString("out.svg"),p);
        gcode->filterGcode(p,f,k,outname);
        filelist.append(outname);
        QFile::remove(p);
        QFile::remove("out.svg");
    }
    emit Sig_CombinePrint(filelist);
}

//灰度雕刻模式打印
void FileProcessThread::slotCarveFile(PGraphicsScene*pScene)
{
    QFile::remove("show.gcode");
    QStringList filelist;
//    ui->labelMovie->show();
    for(int j = 0;j<pScene->m_picItems.size();j++)
    {
        QString filename = pScene->m_picfilenames.at(j);
        QPointF f = pScene->m_picItems.at(j)->pos()-QPointF(30,30);
        QRectF m = pScene->m_picItems.at(j)->getRect();
        QPointF r_pos(m.x(),m.y());
        f = f + r_pos;

        QString p = tr("pic_%1.gcode").arg(j);

        QString outname = tr("printpic_%1.gcode").arg(j);
        {
            b2g->LoadImageW(filename);
            b2g->ExportGcode(f,QPointF(m.width(),m.height()),outname);
        }
        QFile::remove(p);
        filelist.append(outname);
    }
    emit Sig_CombinePrint(filelist);
}
