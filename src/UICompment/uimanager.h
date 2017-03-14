#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include "aa_loadform.h"
#include "ab_serialform.h"
#include "ac_autocheckform.h"
#include "ad_laserform.h"
#include "ae_beginform.h"
#include "af_backform.h"
#include "ag_lastform.h"
#include "ah_hexform.h"
#include "bd_viewform.h"

#include "qcptitle.h"
#include "loadform.h"

class UIManager : public QObject
{
    Q_OBJECT
public:
    explicit UIManager(QObject *parent = 0);

public slots:
    void slotAA(QString);
    void slotAB(QString);
    void slotAC(QString);
    void slotAD(QString);
    void slotAE(QString);
    void slotAF(QString);
    void slotMWin(QString);
    void slotCombinrFile(QStringList);
    void slotToLaser(QString);
    void slotSerialPort(QString m);
    void slotSerialConnect(QString m);
    void slotFinishPersent(unsigned int);
    void slotPrint();
    void slotStop();
    void slotCancle();
    void slotServerLoad(QString cmd);
    void slotProcessBar(bool,quint64);
    void slotRecover();
    void slotConnectFaile(bool);
    void slotConnectFirstFaile(bool);
    void slotShowLoad();
    void slotLoadIn();
    void slotBounding(QRectF);
    void slotAGSuccessful();
    void slotAGFailed();
    void slotAHNextStep();
    void slotLoadNext();
    void slotEndStopState(int);

    void slotFrontEndComebine(QStringList fileList);
    void slotSerialPortTranslate();

private:
    AA_LoadForm* loadform;
    AB_SerialForm* serialform;
    AC_AutoCheckForm* checkform;
    AD_LaserForm* laserform;
    AE_BeginForm* beginform;
    AF_BackForm* backform;
    AG_LastForm* lastform;
    AH_HexForm* hexform;
    BD_ViewForm* viewform;

    LoadForm* firstform;

    QCPTitle* mwindow;
    bool isMWindow;
    bool b_first;

signals:
    void Sig_FrontEndCombine(QStringList);
    void Sig_FrontEndCMD(QString);
    void Sig_SerialPort(QString);
    void Sig_SerialConnect(QString);
    void Sig_Print();
    void Sig_Stop();
    void Sig_Cancle();
    void Sig_FinishPersent(unsigned int);
    void Sig_ProcessBar(bool ,quint64);
    void Sig_Recover();
    void Sig_ConnectFaile(bool);
    void Sig_ConnectFirstFaile(bool);
    void Sig_Bounding(QRectF);
    void Sig_GCODE_ui(QString);
    void Sig_SerialPortTranslate();
};

#endif // UIMANAGER_H
