#include <QSettings>
#include <QMessageBox>
#include <QTextStream>
#include "frontend.h"

FrontEnd::FrontEnd(QObject *parent,CPSerialPort*printports ) :
    QThread(parent)
{

    sender = new GSendModel();
    isStop = true;
    bounddraw = false;
    printport = printports;
    isConnect = false;
    times = 1;

    //this ---->  CPSerialPort
    connect(this,SIGNAL(Sig_ToArduino(QString)),printport,SLOT(WritePort(QString)));

    //CPSerialPort ----> this
    connect(printport,SIGNAL(Sig_GetMessage(QString)),this,SLOT(readArduino(QString)));
    connect(printport,SIGNAL(Sig_Disconnect()),this,SLOT(serialDisconnect()));
    connect(printport,SIGNAL(Sig_ConnectFail(bool)),this,SLOT(slotConnectFaile(bool)));
}

void FrontEnd::serialDisconnect()
{
    emit Sig_Disconnect();
    emit Sig_ConnectFaile(false);
}

void FrontEnd::addToSender()
{

    qDebug()<<"tosend"<<toSend;
    while(1)
    {
        toSend = sender->readOneLine();
        if(toSend.startsWith(";"))
        {

        }
        else
        {
            break;
        }
    }
    if(toSend == "end")
    {
        //CancleNow();
        {
            //            emit Sig_ToArduino("G28 X Y ");
            emit Sig_ToArduino("G1 F2000 X0 Y0 ");
            isStop = true;
            sender->reset();
            emit Sig_ProcessBar(false,0);
        }


    }
    if(toSend.startsWith("G1"))
    {
        //scene->Slot_RenderChange();
        //        emit Sig_SceneChange();
        emit Sig_ProcessBar(true,sender->getPersent());
        //        emit some signal
    }
    emit Sig_FinishPersent(sender->getPersent());

}
void FrontEnd::readArduino(QString cmd)
{
    QString ba = cmd;
    emit Sig_UiShow(cmd);
    QRegExp trig("TRIGGERED");
    QRegExp open("open");

    QRegExp xmin("x_min:");
    QRegExp ymin("y_min:");

    if(!ba.isEmpty())
    {
        QString le = ba;
//        qDebug()<<le;
        if(le.startsWith("start"))
        {
            emit Sig_Connected();
            emit Sig_ConnectFaile(true);
            emit Sig_ConnectFirstFaile(true);
            QMessageBox::information(NULL,tr("Notice"),tr("Serial conected!"));
            printport->close();

        }
        else if(le.contains(ymin))
        {
            QStringList posi = le.split("\n");
            for(int num=0;num<posi.size();num++)
            {
                QString po = posi.at(num);
                qDebug()<<"One line"<<po;
                if(po=="open")
                {
                   emit Sig_EndStopState(2);
                }
                if(po=="TRIGGERED")
                    emit Sig_EndStopState(0);

                if(po.contains(ymin))
                {
                    if(po.contains(trig))
                        emit Sig_EndStopState(1);
                    else if(po.contains(open))
                    {
                        emit Sig_EndStopState(3);
                    }
                }
            }



        }
        else if(le.startsWith("oMG"))
        {
            if(!isStop)//如果没有停止
            {
                if(!bounddraw)
                {
                    addToSender();
                    emit Sig_ToArduino(toSend);
                    // updae the progress
                }
                else
                {
                    if(boundcode.size()>0)
                    {
                        toSend = boundcode.dequeue();
                        emit Sig_ToArduino(toSend);
                    }
                    else
                    {
                        bounddraw = false;
                        isStop = true;
                        boundcode.clear();
                    }

                }

            }
        }
        else if(!isStop)
        {
            emit Sig_ToArduino(toSend);
        }
    }
}

void FrontEnd::gcodeMode(QString file)
{
    sender->SetTargetFile(file);
    isStop = false;
    addToSender();
    emit Sig_ToArduino("G90");
    emit Sig_ToArduino("G0 X0 Y0");
    //开启进度条
    if(!file.isEmpty())
    {
       emit Sig_ProcessBar(true,sender->getPersent());
    }
}
void FrontEnd::slotPrint()
{
    sender->reset();
    QString path = QCoreApplication::applicationDirPath();
    QString gcodefile = path + "/show.gcode";
    sender->SetTargetFile(gcodefile);
    isStop = false;
    addToSender();
    emit Sig_ToArduino("G90");
    emit Sig_ToArduino("G0 X0 Y0");
    //开启进度条
    emit Sig_ProcessBar(true,sender->getPersent());
}
//接收停止指令
void FrontEnd::slotStop()
{
    isStop = true;
    emit Sig_ToArduino("M4 P0");
    //    qDebug()<<"slot get stop command!";
}
//恢复发送
void FrontEnd::slotRecov()
{
    isStop = false;
    emit Sig_ToArduino(toSend);
}
//取消打印
void FrontEnd::slotCancle()
{
    isStop = true;
    sender->reset();
    emit Sig_ToArduino("M4 P0");
    //    emit Sig_ToArduino("G28 X Y");
    emit Sig_ToArduino("G1 F2000 X0 Y0 ");
    //    emit Sig_ProcessBar(false,0);
}
//接收界面发送过来的指令，并转发至串口
void FrontEnd::slotUIWrite(QString cmd)
{
    if(cmd=="disconnect")
    {
        printport->disconnect();
        isConnect = false;
    }
    else
    {
        emit Sig_ToArduino(cmd);
    }
}

//画一个bounding
void FrontEnd::genBounding(QRectF rf)
{
    bounddraw = true;

    //获取当前sender的总边框
    //    sender->SetTargetFile(gcodefile);
    //    QRectF rf = sender->getRect();
    double scale = 0.1;
    rf = QRectF(rf.x()*scale,rf.y()*scale,scale*rf.width(),scale*rf.height());
    //    QRectF rf(10,10,300,300);

    //生成对应的边框gcode
    QString a = tr("M4 P0 \n");
    QString mode = tr("G90 \n");
    boundcode.push_back(mode);
    boundcode.push_back(a);

    //    int t_w = 200;
    //    int t_h = 200;
    for(int i=0;i<2;i++)
    {
        QString a0 = tr("G0 X%1 Y%2 F3000\n").arg(rf.x()).arg(rf.y());
        QString a1 = tr("M4 P30 \n");
        QString a2 = tr("G1 X%1 Y%2 F3000 \n").arg(rf.x()+rf.width()).arg(rf.y());
        QString a3 = tr("G1 X%1 Y%2 \n").arg(rf.x()+rf.width()).arg(rf.y()+rf.height());
        QString a4 = tr("G1 X%1 Y%2 \n").arg(rf.x()).arg(rf.y()+rf.height());
        QString a5 = tr("G1 X%1 Y%2 \n").arg(rf.x()).arg(rf.y());

        //        //for line test
        //        t_w -= 10;
        //        t_h -= 10;
        //        QString a0 = tr("G0 X%1 Y%2 F2000\n").arg(i).arg(i);
        //        QString a1 = tr("M4 P10 \n");
        //        QString a2 = tr("G1 X%1 Y%2 F1000 \n").arg(i+t_w).arg(i);
        //        QString a3 = tr("G1 X%1 Y%2 \n").arg(i+t_w).arg(i+t_h);
        //        QString a4 = tr("G1 X%1 Y%2 \n").arg(i).arg(i+t_h);
        //        QString a5 = tr("G1 X%1 Y%2 \n").arg(i).arg(i);

        boundcode.push_back(a0);
        boundcode.push_back(a1);
        boundcode.push_back(a2);
        boundcode.push_back(a3);
        boundcode.push_back(a4);
        boundcode.push_back(a5);
    }
    QString a6 = tr("M4 P0 \n");
    QString a7 = tr("G1 F2000 X0 Y0 \n");
    boundcode.push_back(a6);
    boundcode.push_back(a7);
    //    qDebug()<<a<<a1<<a2<<a3<<a4<<a5;

}
void FrontEnd::resetBounding()
{
    bounddraw = false;
    boundcode.clear();
}
void FrontEnd::slotDrawBounding(QRectF f)
{
    qDebug()<<"Here we get the bounding command";
    if(isConnect)
    {
        genBounding(f);
        isStop = false;
    }
    emit Sig_ToArduino("M4 P0");
}
void FrontEnd::slotCombineFile(QStringList p)
{
    //将gcode进行多合一显示
    //    if(QFile("show.gcode").exists())
    sender->reset();
    QString path = QCoreApplication::applicationDirPath();
    QFile::remove(path + "/show.gcode");
    QFile file(path + "/show.gcode");

    if(!file.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(NULL,"Notice","Can not show!");
    }
    QTextStream out(&file);
    QString cpath = QCoreApplication::applicationDirPath();
    QString cname = "/Config.ini";
    QString callPath = QString("%1%2").arg(cpath).arg(cname);
    QSettings* psetting = new QSettings(callPath,QSettings::IniFormat);
    psetting->beginGroup("laser");
    QString type = psetting->value("material").toString();
    psetting->endGroup();
    QString mpath = QCoreApplication::applicationDirPath();
    QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* tpsetting = new QSettings(mallPath,QSettings::IniFormat);
    tpsetting->beginGroup(type);
    times =  tpsetting->value("times").toInt();
    tpsetting->endGroup();
    qDebug()<<tr("material=%2 times=%1").arg(times).arg(type);
    for(int j=0;j<times;j++)
    {
        for(int i=0;i<p.size();i++)
        {
            QFile tempfile(p.at(i));
            tempfile.open(QIODevice::ReadWrite);
            QTextStream tempstream(&tempfile);
            while(!tempstream.atEnd())
                out<<tempstream.readLine()<<"\n";
            tempfile.close();

        }
    }
    out<<";end\n";
    for(int m=0;m<p.size();m++)
    {
        if(!(p.at(m)=="./firsttest.gcode"))
            QFile::remove(p.at(m));
    }
    file.close();
    slotPrint();
}
void FrontEnd::slotConnectPort(QString m)
{
    //    printport->ConnectPort(m);
    if(m=="show")
    {
        isConnect = true;
        printport->show();
    }
}
void FrontEnd::slotSerialConnect(QString m)
{
    printport->ConnectPort(m);
    isConnect = true;
    qDebug()<<"Now int the serial connect slot";
}

void FrontEnd::slotConnectFaile(bool b)
{
    emit Sig_ConnectFaile(b);
}
void FrontEnd::run()
{
    exec();
}
