#include <QDebug>
#include "uimanager.h"

UIManager::UIManager(QObject *parent) :
    QObject(parent)
{
    isMWindow = false;
    loadform = new AA_LoadForm();

    serialform  = new AB_SerialForm();
    checkform   = new AC_AutoCheckForm();
    laserform   = new AD_LaserForm();
    beginform   = new AE_BeginForm();
    backform    = new AF_BackForm();
    lastform    = new AG_LastForm();
    hexform     = new AH_HexForm();
    mwindow     = new QCPTitle();
    firstform   = new LoadForm();
	QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    b_first = psetting->value("showserial").toBool();
    psetting->endGroup();

    connect(loadform,SIGNAL(Sig_AA(QString)),this,SLOT(slotAA(QString)));
    connect(serialform,SIGNAL(Sig_AB(QString)),this,SLOT(slotAB(QString)));
    connect(serialform,SIGNAL(Sig_LoadSuccessful()),this,SLOT(slotLoadIn()));
    connect(checkform,SIGNAL(Sig_AC(QString)),this,SLOT(slotAC(QString)));
    connect(checkform,SIGNAL(Sig_ToLaser(QString)),this,SLOT(slotToLaser(QString)));
    connect(laserform,SIGNAL(Sig_AD(QString)),this,SLOT(slotAD(QString)));
    connect(laserform,SIGNAL(Sig_ToLaser(QString)),this,SLOT(slotToLaser(QString)));
    connect(beginform,SIGNAL(Sig_AE(QString)),this,SLOT(slotAE(QString)));
    connect(beginform,SIGNAL(Sig_PrintTestFile(QStringList)),this,SLOT(slotCombinrFile(QStringList)));
    connect(backform,SIGNAL(Sig_AF(QString)),this,SLOT(slotAF(QString)));
    connect(lastform,SIGNAL(Sig_LastSuccessful()),this,SLOT(slotAGSuccessful()));
    connect(lastform,SIGNAL(Sig_LastFailed()),this,SLOT(slotAGFailed()));

    connect(hexform,SIGNAL(Sig_AHNextStep()),this,SLOT(slotAHNextStep()));

    connect(firstform,SIGNAL(Sig_SerialConnect(QString)),this,SLOT(slotSerialConnect(QString)));
    connect(firstform,SIGNAL(Sig_ShowLoad()),this,SLOT(slotShowLoad()));
    connect(firstform,SIGNAL(Sig_LoadNext()),this,SLOT(slotLoadNext()));

    connect(mwindow,SIGNAL(Sig_MWin(QString)),this,SLOT(slotMWin(QString)));
    connect(mwindow,SIGNAL(Sig_BDCombineFile(QStringList)),this,SLOT(slotCombinrFile(QStringList)));
    connect(mwindow,SIGNAL(Sig_ToLaser(QString)),this,SLOT(slotToLaser(QString)));
    connect(mwindow,SIGNAL(Sig_SerialPort(QString)),this,SLOT(slotSerialPort(QString)));
    connect(mwindow,SIGNAL(Sig_Print()),this,SLOT(slotPrint()));
    connect(mwindow,SIGNAL(Sig_Stop()),this,SLOT(slotStop()));
    connect(mwindow,SIGNAL(Sig_Cancle()),this,SLOT(slotCancle()));
    connect(mwindow,SIGNAL(Sig_Recover()),this,SLOT(slotRecover()));
    connect(mwindow,SIGNAL(Sig_Bounding(QRectF)),this,SLOT(slotBounding(QRectF)));
    connect(mwindow,SIGNAL(Sig_GCODE(QString)),this,SIGNAL(Sig_GCODE_ui(QString)));

    connect(this,SIGNAL(Sig_ProcessBar(bool,quint64)),mwindow,SLOT(slotProcessBar(bool,quint64)));
    connect(this,SIGNAL(Sig_ConnectFaile(bool)),mwindow,SLOT(slotConnectFaile(bool)));
    connect(this,SIGNAL(Sig_ConnectFirstFaile(bool)),firstform,SLOT(slotConnectFaile(bool)));;
    QSettings* tsetting = new QSettings(lallPath,QSettings::IniFormat);
    tsetting->beginGroup("mode");
    QString language = tsetting->value("language").toString();
    tsetting->endGroup();

    //语言
    QString a;
    if(language=="zh")
    {
        a = ":Language/c_zh.qm";
        mwindow->slotZhAction();
    }
    else if(language=="en")
    {
        a = ":Language/c_en.qm";
        mwindow->slotEnAction();
    }
    //更新语言版本
    QTranslator *m_ens = new QTranslator();
    m_ens->load(a);
    qApp->installTranslator(m_ens);

    //    loadform = new AA_LoadForm();

    //    serialform  = new AB_SerialForm();
    //    checkform   = new AC_AutoCheckForm();
    //    laserform   = new AD_LaserForm();
    //    beginform   = new AE_BeginForm();
    //    backform    = new AF_BackForm();
    //    mwindow     = new QCPTitle();
    //    firstform   = new LoadForm();
    loadform->languageUpdate();
    serialform->languageUpdate();
    checkform->languageUpdate();
    laserform->languageUpdate();
    beginform->languageUpdate();
    backform->languageUpdate();
    firstform->languageUpdate();
    lastform->languageUpdate();
    hexform->languageUpdate();

    serialform->show();
}

/*
 *
 *  接收来自服务器的信号
 *  ----start----
 *
 */
void UIManager::slotServerLoad(QString cmd)
{
    //将手机上传的图片转载到软件中去
    mwindow->slotAddFont(cmd);
}
/*
 *
 *  接收来自服务器的信号
 *  ----end----
 *
 */



/*
 *
 *  初次使用向导界面SLOT函数
 *  ----start----
 *
 */
void UIManager::slotAA(QString cmd)
{
    if(cmd=="next")
    {
        loadform->hide();
        //        checkform->show();
        hexform->show();
    }
    else if(cmd == "show")
    {
        loadform->hide();
		QString lpath = QCoreApplication::applicationDirPath(); 
	    QString lname = "/mLaser.ini";
        QString lallPath = QString("%1%2").arg(lpath).arg(lname);
        QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
        psetting->beginGroup("mode");
        QString mode = psetting->value("mode").toString();
        psetting->endGroup();
        mwindow->setMode(mode);
        mwindow->showMaximized();
        //        mwindow->show();

    }
}
void UIManager::slotLoadIn()
{
    serialform->close();
    if(b_first)
    {
        firstform->show();
    }
    else
    {
        loadform->show();

    }
}
void UIManager::slotAB(QString cmd)
{
    if(cmd=="next")
    {
        serialform->hide();
        checkform->show();
        checkform->timeStart();
    }
}

void UIManager::slotAC(QString cmd)
{
    if(cmd =="pre")
    {

        checkform->hide();
        firstform->setInitState(true);
        firstform->hideCheckBox();
        firstform->show();
    }
    else if(cmd=="next")
    {
        checkform->hide();
        laserform->show();
    }
}

void UIManager::slotAD(QString cmd)
{
    if(cmd =="pre")
    {
        laserform->hide();
        checkform->show();
        checkform->timeStart();
    }
    else if(cmd=="next")
    {
        laserform->hide();
        beginform->show();
    }
}

void UIManager::slotAE(QString cmd)
{
    if(cmd=="pre")
    {
        beginform->hide();
        laserform->show();
    }
    else if(cmd == "next")
    {
        beginform->hide();
        lastform->show();
    }
}

void UIManager::slotAF(QString cmd)
{
    if(cmd=="back")
    {
        backform->hide();
        loadform->show();
    }
}
void UIManager::slotAGSuccessful()
{
    lastform->hide();
    loadform->show();
}
void UIManager::slotAGFailed()
{
    lastform->hide();
    backform->show();
}
void UIManager::slotAHNextStep()
{
    hexform->hide();
    //    checkform->show();
    firstform->setInitState(true);
    firstform->hideCheckBox();
    firstform->show();
}
void UIManager::slotLoadNext()
{
    firstform->hideCheckBox();
    firstform->hide();
    checkform->show();
    checkform->timeStart();

}
void UIManager::slotMWin(QString cmd)
{
    if(cmd=="backhome")
    {
        loadform->show();
        mwindow->hide();

        qDebug()<<"backhome here";
    }
}
void UIManager::slotShowLoad()
{
    qDebug()<<"flsakdjf";
    //    if(!mwindow->isActiveWindow())
    // if(!isMWindow)
    if(mwindow->isHidden())
    {
        loadform->show();
        isMWindow = true;
    }

}
//来自qcptitle中的画图片边框的槽函数
void UIManager::slotBounding(QRectF f)
{
    emit Sig_Bounding(f);
}

/*
 *  初次使用向导界面SLOT函数
 *  ----end----
 */



/*
 *
 *  UIManager和FrontEnd之间的通信
 *  ----start----
 *
 */
void UIManager::slotCombinrFile(QStringList m)
{
    //发送至FrontEnd
    emit Sig_FrontEndCombine(m);
}

//发送gcode指令给fontend这个类
void UIManager::slotToLaser(QString m)
{
    emit Sig_FrontEndCMD(m);
}


void UIManager::slotSerialPort(QString m)
{
    emit Sig_SerialPort(m);
}
void UIManager::slotSerialConnect(QString m)
{
    emit Sig_SerialConnect(m);
}
//接收FrontEnd发送过来的打印完成比例
void UIManager::slotFinishPersent(unsigned int m)
{
    //界面显示对应的打印比例
    emit Sig_FinishPersent(m);
}

//发送打印指令到FrontEnd
void UIManager::slotPrint()
{
    emit Sig_Print();
}
//发送停止指令到FrontEnd
void UIManager::slotStop()
{
    emit Sig_Stop();
}
//发送取消指令到FrontEnd
void UIManager::slotCancle()
{
    emit Sig_Cancle();
}
void UIManager::slotRecover()
{
    emit Sig_Recover();
}
void UIManager::slotProcessBar(bool s,quint64 m)
{
    emit Sig_ProcessBar(s,m);
}

void UIManager::slotConnectFaile(bool b)
{
    emit Sig_ConnectFaile(b);
    checkform->setConnectState(b);
    qDebug()<<"UIManager receive:--------";
}
void UIManager::slotEndStopState(int m)
{
    checkform->setButtonStyle(m);
}
//首页接受串口连接与否的槽函数
void UIManager::slotConnectFirstFaile(bool b)
{
    emit Sig_ConnectFirstFaile(b);
}

