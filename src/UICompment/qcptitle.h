#ifndef QCPTITLE_H
#define QCPTITLE_H

#include <QWidget>
#include <QtWidgets>
#include <QMenu>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "titlebar.h"

#include "textform.h"
#include "controlform.h"
#include "gcodeform.h"
#include "postionform.h"
#include "qrencodeform.h"
#include "hexloadform.h"
#include "postionform.h"
#include "firmsettingform.h"

#include "ba_easyform.h"
#include "bb_easynform.h"
#include "bc_expertform.h"
#include "bd_viewform.h"

#include "Update/fvupdater.h"
#include "Update/fvupdatewindow.h"
#include "Update/fvupdateconfirmdialog.h"
#include "Communicate/serial/cpserialport.h"
#include "FileProcessor/dxf2svg/dl_dxf.h"
#include "FileProcessor/dxf2svg/dl_creationadapter.h"
#include "FileProcessor/dxf2svg/creationclass.h"

#include "Zipunzip/zipunzip.h"

class ControlForm;
class QrencodeForm;

#define IDLE 0
#define PRINTABLE 0x01
#define STOP 0x02
namespace Ui {
class QCPTitle;
}
enum printStatus{idle,stop,printable};
class QCPTitle : public QWidget
{
    Q_OBJECT

public:
    explicit QCPTitle(QWidget *parent = 0);
    ~QCPTitle();
    void setMode(QString);

public slots:
    void slotBA(QString);
    void slotBB(QString);
    void slotBC(QString);
    void slotBD(QString);
    void slotExpert();
    void slotCoustom();
    void slotZhAction();
    void slotEnAction();
    void slotInchAction();
    void slotMMAction();

    void slotAddFont(QString);
    void slotSave();
    void slotSaveAs();
    void slotCutting(); //保存 切割文件
    void slotCarve();   //保存 雕刻文件
    void slotBDCombineFile(QStringList);
    void slotToLaser(QString);
    void slotStop();
    void slotPrint();
    void slotCancle();
    void slotFinishPersent(unsigned int);
    void slotLaserOn(QString);
    void slotPreview();                  //画边框
    void slotProcessBar(bool,quint64);   //进度条显示
    void slotGcodeMode();
    void slotConnectFaile(bool);
    void slotSerialAction(bool t);
    void slotHexLoad();
    void slotShowPos(QRectF);
    void slotFirmSetting();

protected:
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);
private slots:
    void on_btnText_clicked();

    void on_btnOpenPanel_clicked();

    void on_btnSetHome_clicked();

    void on_btnOpen_clicked();

    void on_btnDelet_clicked();

    void on_comboBoxSerial_activated(const QString &arg1);

    void on_btnSerial_clicked();

    void on_btnQrcode_clicked();

    void on_btnSave_clicked();

private:
    Ui::QCPTitle *ui;
    TextForm* text;
    ControlForm* control;
    PostionForm* postion;
    GcodeForm* gcodeer;
    QrencodeForm* rencode;
    HexLoadForm* hex;
    PostionForm* posform;

    QHBoxLayout* panlelayout;
    BA_EasyForm* easy;
    BB_EasynForm* easyn;
    BC_ExpertForm* expert;
    BD_ViewForm* viewer;
    FirmSettingForm* firmform;
    //CPSerialPort* cpserial;
    FvUpdateWindow* fvupdatewindow;
    FvUpdateConfirmDialog* fvupdate;

    ZipUnzip* zip;

    QActionGroup *languageGroup;
    QAction *zhAction;
    QAction *enAction;

    QActionGroup * choseMenu;
    QAction * expertAction ;
    QAction * coustomAction;

    QAction * gcodeAction;
    QAction * updateAction;

    QAction *mmAction;
    QAction *inchAction;

    QAction *serialAction;

    QAction* softwareAction;

    QMenu *scaleMenu;       //刻度尺单位
    QMenu *languageMenu;    //语言
    QMenu* uiChoose;        //界面选择

    QStringList comList;

    bool bLoadedfile;
    bool bConnected;
    bool bPic;
    int STATUS;     //0 --> idle  1--> printable 2-->stop
    int times;

    CreationClass* mcreationClass;

    void enableUserWaitingCursor();
    void disableUserWaitingCursor();
    bool removeDirWithContent(QString dirName);
    void Serial_Connect(QString);
protected:
    void showEvent(QShowEvent *e);
signals:
    void Sig_MWin(QString);
    void Sig_BD_Open(QString);
    void Sig_MAddFont(QString);
    void Sig_BD_Rect(QRect);
    void Sig_ProcessItem();     //处理场景BD_ViewForm中的模型。将模型转化为gcode
    void Sig_SaveAs(QString);   //弹出对话框，发送路径及保存名至场景类BD_ViewForm，保存*ml工程文件
    void Sig_BDCombineFile(QStringList);
    void Sig_ToLaser(QString);
    void Sig_SerialPort(QString);
    void Sig_uintUpdate(void);
    void Sig_Print();
    void Sig_Stop();
    void Sig_Recover();
    void Sig_Cancle();
    void Sig_ProcessBar(bool,unsigned int);
    void Sig_Bounding(QRectF);
    void Sig_ReadParmeter();
    void Sig_GCODE(QString);
    void Sig_Rencode();
    void Sig_SerialConnect(QString);

};

#endif // QCPTITLE_H
