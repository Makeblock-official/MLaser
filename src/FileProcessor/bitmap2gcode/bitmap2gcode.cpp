#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include "bitmap2gcode.h"
#include "QCoreApplication"

const int Bs = 255;
const int Ns = 0;
const int Step1[5][5]={
    {Bs,Bs,Bs,Bs,Bs},
    {Bs,Bs,Bs,Bs,Bs},
    {Bs,Bs,Ns,Bs,Bs},
    {Bs,Bs,Bs,Bs,Bs},
    {Bs,Bs,Bs,Bs,Bs},
};
const int Step2[5][5]={
    {Bs,Bs,Bs,Bs,Bs},
    {Bs,Bs,Ns,Bs,Bs},
    {Bs,Ns,Ns,Ns,Bs},
    {Bs,Bs,Ns,Bs,Bs},
    {Bs,Bs,Bs,Bs,Bs},
};
const int Step3[5][5]={
    {Bs,Bs,Ns,Bs,Bs},
    {Bs,Ns,Ns,Ns,Bs},
    {Ns,Ns,Ns,Ns,Ns},
    {Bs,Ns,Ns,Ns,Bs},
    {Bs,Bs,Ns,Bs,Bs},
};
const int Step4[5][5]={
    {Bs,Ns,Ns,Ns,Bs},
    {Ns,Ns,Ns,Ns,Ns},
    {Ns,Ns,Ns,Ns,Ns},
    {Ns,Ns,Ns,Ns,Ns},
    {Bs,Ns,Ns,Ns,Bs},
};

//const int Mtep1[3][3]={
//    {Bs,Bs,Bs},
//    {Bs,Ns,Bs},
//    {Bs,Ns,Bs},
//};

Bitmap2Gcode::Bitmap2Gcode(QWidget *parent) :
    QWidget(parent)
{
    preOn = false;

}

//load image without threadhold
void Bitmap2Gcode::LoadImageW(QString path)
{
    img = QImage(path);
}
void Bitmap2Gcode::LoadImageW(QString path,int gthreadhold)
{
    thredhold = gthreadhold;
    //   img = GrayScalePicture(QImage(path),thredhold);
    img = QImage(path);
}
//Save the gcode to some place
void Bitmap2Gcode::ExportGcode(QPointF f,QPointF scopes,QString savepath)
{
    pos = f;
    scope = scopes;
    QString mpath = QCoreApplication::applicationDirPath();
    QString mname = "/Config.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* confsetting = new QSettings(mallPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    //    confsetting->setValue("carv_type",index);
    int c_type = confsetting->value("carv_type").toInt();
    confsetting->endGroup();

    if(c_type==1)
        ExportGrayPix(GrayScalePicture(img,thredhold),savepath);
    else if(c_type==0)
        ExportGrayPix(img,savepath);
}

//make sure the picture is binary picture
QImage Bitmap2Gcode::GrayScalePicture(QImage src,int grayscale)
{
    QString path = QCoreApplication::applicationDirPath(); 
    QImage tempImg;
    tempImg = src;
    QImage pos_1 = tempImg;                         //拷贝
    QImage show;
    if(tempImg.format()>3)
    {
        qDebug()<<tr("format = %1").arg(tempImg.format());
        QRgb *pixels = (QRgb *)tempImg.scanLine(0);
        QRgb *grays = (QRgb *)pos_1.scanLine(0);
        QRgb thread = qRgb(grayscale,grayscale,grayscale);
        QRgb threadw = qRgb(255,255,255);
        QRgb threadb = qRgb(0,0,0);
        int gray = 0;
        int v = 0;
        int error = 0;

        for(int y =0;y<tempImg.height()-2;y++)
        {
            for(int x=0;x<tempImg.width()-2;x++)
            {
                //gray scale
                gray = qGray(pixels[x+y*(tempImg.width())]);
                grays[x+y*(tempImg.width())] = qRgb(gray,gray,gray);

            }
        }

        for(int y =0;y<tempImg.height()-3;y++)
        {
            for(int x=1;x<tempImg.width()-3;x++)
            {
                //gray scale
                gray = qGray(grays[x+y*(tempImg.width())]);
                v = (gray>=127)?255:0;
                error = (gray - v) / 8;
                QRgb q_error=qRgb(error,error,error);
                QRgb q_gray=qRgb(v,v,v);

                grays[x+y*(tempImg.width())] = q_gray;

                grays[1+x+y*(tempImg.width())] = grays[1+x+y*(tempImg.width())]+q_error;
                grays[2+x+y*(tempImg.width())] = grays[2+x+y*(tempImg.width())]+q_error;
                grays[x-1+(y+1)*(tempImg.width())] = grays[x-1+(y+1)*(tempImg.width())]+q_error;
                grays[x+(y+1)*(tempImg.width())] = grays[x+(y+1)*(tempImg.width())]+q_error;
                grays[1+x+(y+1)*(tempImg.width())] = grays[1+x+(y+1)*(tempImg.width())]+q_error;
                grays[x+(y+2)*(tempImg.width())] = grays[x+(y+2)*(tempImg.width())]+q_error;
            }
        }
        show = pos_1;
    }
    else
    {
        show = tempImg;
    }
    show.save(path + "/dear.png","PNG",100);
    return show;
}

//change image to halftone
QImage Bitmap2Gcode::Image2Halftone(QImage src)
{
    QString path = QCoreApplication::applicationDirPath(); 
    QImage image = src;

    int media = 0;
    int h = image.height();
    int w = image.width();
    QRgb* rgbpixel = (QRgb *)image.scanLine(0);
    for (int ii = 0; ii < image.height()/5; ii++)
    {
        for (int jj = 0; jj < image.width()/5; jj++)
        {
            media = 0;
            for(int n=0;n<5;n++)
            {
                for(int m=0;m<5;m++)
                {
                    QRgb a = rgbpixel[(ii*5+n)*w+(jj*5+m)];
                    int gray = qGray(a);
                    media+=gray;
                }
            }
            media = media/25;
            for(int y=0;y<5;y++)
            {
                for(int x=0;x<5;x++)
                {
                    if((media>250)&&(media<255))
                    {
                        QRgb a = QColor(255, 255, 255).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }
                    else if((media>=190)&&(media<250))
                    {
                        QRgb a = QColor(Step1[y][x], Step1[y][x], Step1[y][x]).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }
                    else if((media>=130)&&(media<190))
                    {
                        QRgb a = QColor(Step2[y][x], Step2[y][x], Step2[y][x]).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }
                    else if((media>=70)&&(media<130))
                    {
                        QRgb a = QColor(Step3[y][x], Step3[y][x], Step3[y][x]).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }
                    else if((media>=10)&&(media<70))
                    {
                        QRgb a = QColor(Step4[y][x], Step4[y][x], Step4[y][x]).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }
                    else if((media>=0)&&(media<10))
                    {
                        QRgb a = QColor(0, 0, 0).rgba();
                        rgbpixel[(ii*5+y)*w+(jj*5+x)] = a;
                    }

                }
            }

        }


    }
    image.save(path + "/hi.png","PNG",100);
    return image;
}


//export gcode
//void Bitmap2Gcode::ExportGrayPix(QImage src,QString savefile)
//{
//    QFile file(savefile);
//    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        QMessageBox::critical(NULL, tr("提示"), tr("无法创建文件"));
//        return ;
//    }
//    QTextStream out(&file);
//    QImage image = src.scaled(scope.x(),scope.y(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

//    Image2Halftone(src);
//    pos = pos/10.0;
//    double scale=0.1;
//    out<<"G0 X1 Y1 F6000 \n";
//    out<<"M4 P0 \n";
//    for (int ii = 0; ii < image.height(); ii++)
//    {
//        uchar* scan = image.scanLine(ii);
//        QString p1;
//        QString p2;
//        if(ii%2==0)
//        {
//            int depth =4;
//            for (int jj = 0; jj < image.width(); jj++)
//            {

//                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
//                int gray = qGray(*rgbpixel);

//                //power off
//                if(gray>240)
//                {
//                    if(preOn)
//                    {
//                        p1 = "M4 P0 \n";
//                        p2 = tr("G0 X%1 Y%2 F6000 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p1<<p2;
//                        preOn = false; //关闭
//                    }
//                    else
//                    {
//                        p2 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p2;
//                    }

//                }
//                else
//                {
//                    if(!preOn)
//                    {
//                        p1 = tr("M4 P%1 \n").arg(255-gray);
//                        p2 = tr("G1 X%1 Y%2 F4000 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        preOn = true;
//                        out<<p1<<p2;
//                    }
//                    else
//                    {
//                        p2 = tr("G1 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p2;
//                    }

//                }

//            }
//        }
//        else
//        {
//            int depth =4;
//            for (int jj = image.width(); jj >0; jj--)
//            {

//                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
//                int gray = qGray(*rgbpixel);

//                //power off
//                if(gray>=240)
//                {
//                    if(preOn)
//                    {
//                        p1 = "M4 P0 \n";
//                        p2 = tr("G0 X%1 Y%2 F6000 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p1<<p2;
//                        preOn = false; //关闭
//                    }
//                    else
//                    {
//                        p2 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p2;
//                    }
//                }
//                else
//                {
//                    if(!preOn)
//                    {
//                        p1 = tr("M4 P%1 \n").arg(255-gray);
//                        p2 = tr("G1 X%1 Y%2 F4000 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        preOn = true;
//                        out<<p1<<p2;
//                    }
//                    else
//                    {
//                        p2 = tr("G1 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p2;
//                    }

//                }

//            }
//        }

//    }
//    qDebug()<<"export finish!!";
//    file.close();

//}

//快速打印黑白照片
//void Bitmap2Gcode::ExportGrayPix(QImage src,QString savefile)
//{
//    QFile file(savefile);
//    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        QMessageBox::critical(NULL, tr("提示"), tr("无法创建文件"));
//        return ;
//    }
//    QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
//    psetting->beginGroup("laser");
//    QString type = psetting->value("material").toString();
//    psetting->endGroup();

//    QSettings* tpsetting = new QSettings("material.ini",QSettings::IniFormat);
//    tpsetting->beginGroup(type);
//    int laserTravelSpeed =  tpsetting->value("laserTravelSpeed").toInt();
//    int laserSpeed =  tpsetting->value("laserSpeed").toInt();
//    int laserPower = tpsetting->value("laserPowerHigh").toInt();
//    tpsetting->endGroup();

//    QTextStream out(&file);
//    //是否使用halftone
//    QImage img_1 = Image2Halftone(src);
//    QImage image = src.scaled(scope.x(),scope.y(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);


//    pos = pos/10.0;
//    double scale=0.1;
//    int gray =0;
//    out<<"G0 X1 Y1 F6000 \n";
//    out<<"M4 P0 \n";
//    for (int ii = 0; ii < image.height(); ii+=2)
//    {
//        uchar* scan = image.scanLine(ii);
//        QString p0;
//        QString p1;
//        QString p2;
//        QString ps;
//        if(ii%4==0)
//        {
//            int depth =4;
//            for (int jj = 0; jj < image.width(); jj+=2)
//            {

//                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
//                gray = qGray(*rgbpixel);
//                qDebug()<<tr("gray scale value = %1").arg(gray);
//                //power off
//                if(gray>240)
//                {
//                    if(preOn)
//                    {
//                        //G1在此结束，写G1
//                        p0 = tr("G1 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        p1 = "M4 P0 \n";
//                        ps = tr("G0 F%1 \n").arg(laserTravelSpeed);
//                        p2 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p0<<p1<<ps<<p2;
//                        preOn = false; //关闭
//                    }

//                }
//                else
//                {
//                    if(!preOn)
//                    {
//                        p0 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        p1 = tr("M4 P%1 \n").arg(laserPower);
//                       p1 = tr("M4 P%1 \n").arg(230);
//                        ps = tr("G1 F%1 \n").arg(laserSpeed);
//                        p2 = tr("G1 X%1 Y%2  \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        preOn = true;
//                        out<<p0<<p1<<ps<<p2;
//                    }

//                }

//            }
//        }
//        else
//        {
//            int depth =4;
//            for (int jj = image.width(); jj >0; jj-=2)
//            {

//                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
//                gray = qGray(*rgbpixel);

//                //power off
//                if(gray>=240)
//                {
//                    if(preOn)
//                    {
//                        //G1在此结束，写G1
//                        p0 = tr("G1 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        p1 = "M4 P0 \n";
//                        ps = tr("G0 F%1 \n").arg(laserTravelSpeed);
//                        p2 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        out<<p0<<p1<<ps<<p2;
//                        preOn = false; //关闭
//                    }
//                }
//                else
//                {
//                    if(!preOn)
//                    {
//                        p0 = tr("G0 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        p1 = tr("M4 P%1 \n").arg(laserPower);
//                        p1 = tr("M4 P%1 \n").arg(230);
//                        ps = tr("G1 F%1 \n").arg(laserSpeed);
//                        p2 = tr("G1 X%1 Y%2  \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
//                        preOn = true;
//                        out<<p0<<p1<<ps<<p2;
//                    }

//                }

//            }
//        }

//    }
//    qDebug()<<"export finish!!";
//    file.close();
//}

//export gcode,包含所有细节，深浅
void Bitmap2Gcode::ExportGrayPix(QImage src,QString savefile)
{
    QFile file(savefile);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::critical(NULL, tr("提示"), tr("无法创建文件"));
        return ;
    }
    QTextStream out(&file);
    //    QImage temp_d = Image2Halftone(src);
    QImage image = src.scaled(scope.x(),scope.y(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    QString mpath = QCoreApplication::applicationDirPath();
    QString mname = "/mLaser.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* apsetting = new QSettings(mallPath,QSettings::IniFormat);
    apsetting->beginGroup("mode");
    QString uint = apsetting->value("unit").toString();
    double scale=0.1;
    //     pos = pos/10;
    if(uint=="inch")
    {
        pos = pos/10;
        scale = 0.1;

    }
    else if(uint=="mm")
    {
        pos = pos/10;
        scale = 0.1;
    }
    apsetting->endGroup();

    out<<"G0 X1 Y1 F6000 \n";
    out<<"M4 P0 \n";
    QString cpath = QCoreApplication::applicationDirPath();
    QString cname = "/Config.ini";
    QString callPath = QString("%1%2").arg(cpath).arg(cname);
    QSettings* psetting = new QSettings(callPath,QSettings::IniFormat);
    psetting->beginGroup("laser");
    int time = psetting->value("p_power_time").toInt();
    int r_pmp  = psetting->value("r_per_pixcel").toInt();
    int v_pmp  = psetting->value("v_per_pixcel").toInt();
    int p_power = psetting->value("p_power").toInt();
    if(p_power>=100)
        p_power=100;
    p_power = p_power*2.10;
    double depth_scale = p_power/255.0;

    psetting->endGroup();


    for (int ii = 0; ii < image.height(); ii+=r_pmp)
    {
        uchar* scan = image.scanLine(ii);
        QString p1;
        QString p2;
        QString p3;
        QString p4;
        if(ii%(2*r_pmp)==0)
        {
            int depth =4;
            for (int jj = 0; jj < image.width(); jj+=v_pmp)
            {

                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
                int gray = qGray(*rgbpixel);

                //power off
                if(gray>=254)
                {
                    if(preOn)
                    {
                        p1 = "M4 P0 \n";
                        p2 = tr("G0 X%1 Y%2 F3500 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
                        out<<p1<<p2;
                        preOn = false; //关闭
                    }
                }
                else
                {
                    if(!preOn)
                    {
                        preOn = true;
                        out<<"G1 F3000 \n";
                    }

                    p1 = tr("M4 P%1 \n").arg(40+(255-gray)*depth_scale);
                    //                    p1 = tr("M4 P%1 \n").arg((255-gray));
                    p2 = tr("G1 X%1 Y%2 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
                    p3 = tr("G4 P%1 \n").arg(time);
                    p4 = "M4 P0 \n";
                    out<<p2<<p1<<p3<<p4;
                }
            }
        }
        else
        {
            int depth =4;
            for (int jj = image.width(); jj >0; jj-=v_pmp)
            {

                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
                int gray = qGray(*rgbpixel);

                //power off
                if(gray>=254)
                {
                    if(preOn)
                    {
                        p1 = "M4 P0 \n";
                        p2 = tr("G0 X%1 Y%2 F6000 \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
                        out<<p1<<p2;
                        preOn = false; //关闭
                    }
                }
                else
                {
                    if(!preOn)
                    {
                        preOn = true;
                        out<<"G1 F3000 \n";
                    }

                    p1 = tr("M4 P%1 \n").arg(40+(255-gray)*depth_scale);
                    //                    p1 = tr("M4 P%1 \n").arg((255-gray));
                    p2 = tr("G1 X%1 Y%2  \n").arg(jj*scale+pos.x()).arg(ii*scale+pos.y());
                    p3 = tr("G4 P%1 \n").arg(time);
                    p4 = "M4 P0 \n";
                    out<<p2<<p1<<p3<<p4;
                    preOn = true;
                }
            }
        }

    }
    qDebug()<<"export finish!!";
    file.close();
}

