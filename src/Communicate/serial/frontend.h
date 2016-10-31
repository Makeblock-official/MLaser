#ifndef FRONTEND_H
#define FRONTEND_H

#include <QThread>
#include <QQueue>

#include "cpserialport.h"
#include "../../FileProcessor/gcoder/gsendmodel.h"

class FrontEnd : public QThread
{
    Q_OBJECT
public:
    explicit FrontEnd(QObject *parent = 0,CPSerialPort* port = 0);


public slots:
    //打印控制类槽函数
    void slotPrint();
    void slotStop();
    void slotRecov();
    void slotCancle();
    void slotDrawBounding(QRectF);
    void addToSender();
    void readArduino(QString cmd);
    void genBounding(QRectF);
    void setBounding(QQueue <QString> a,bool bounddraws){boundcode=a;bounddraw=bounddraws;}
    void resetBounding();
    void gcodeMode(QString);
    //通用字符串槽函数
    void slotUIWrite(QString);

    //串口同连接槽函数
    void serialDisconnect();
    void slotCombineFile(QStringList);
    void slotConnectPort(QString m);
    void slotSerialConnect(QString m);
    void slotConnectFaile(bool );
protected:
    void run();
private:
    CPSerialPort* printport;
    GSendModel*sender;


    QString toSend;
    bool isStop;
    bool isConnect;

    //draw the bounding
    bool bounddraw;
    int times;
    QQueue <QString>boundcode;

signals:
    void Sig_ToArduino(QString);
    void Sig_UiShow(QString);
    void Sig_SceneChange();
    void Sig_Connected();
    void Sig_Disconnect();
    void Sig_FinishPersent(unsigned int);
    void Sig_ProcessBar(bool,quint64);
    void Sig_ConnectFaile(bool b);
    void Sig_ConnectFirstFaile(bool);
    void Sig_EndStopState(int);
};

#endif // FRONTEND_H
