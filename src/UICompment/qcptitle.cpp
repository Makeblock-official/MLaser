#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "../Update/fvupdater.h"
#include "qcptitle.h"
#include "ui_qcptitle.h"
#include "Communicate/serial/cpserialport.h"

QCPTitle::QCPTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QCPTitle)
{
    ui->setupUi(this);
    //    resize(900,600);
    this->setWindowTitle("mLaser激光雕刻");
    STATUS = IDLE;
    times = 1;
    bLoadedfile = false;
    bConnected = false;
    bPic = false;
    text    = new TextForm();
    control = new ControlForm();
//    postion = new PostionForm();
    rencode = new QrencodeForm();
    gcodeer = new GcodeForm();
//    mcreationClass = new CreationClass();
    hex = new HexLoadForm();
    posform = new PostionForm();
    fvupdatewindow = new FvUpdateWindow();
    fvupdate = new FvUpdateConfirmDialog;
//    cpserial = new CPSerialPort();

//    firmform = new FirmSettingForm();

    //简单模式第一个页面
    //    easy = new BA_EasyForm();
    easy = ui->easy;
    //简单模式第二个页面
    //    easyn = new BB_EasynForm();
    easyn = ui->easyn;
    //专家模式
    //    expert = new BC_ExpertForm();
    expert = ui->expert;

    //左侧显示图元的窗口类
    viewer  = new BD_ViewForm();
//    easyn->setElementEnable(true);
//    expert->setElementEnable(false);


    //左侧的工具栏layout
    panlelayout = new QHBoxLayout();

    panlelayout->addWidget(easy);
    panlelayout->addWidget(easyn);
    panlelayout->addWidget(expert);
    easy->hide();
    easyn->hide();
    expert->hide();

    panlelayout->setMargin(0);
    panlelayout->setSpacing(0);
    panlelayout->setContentsMargins(0,0,0,0);
    ui->framePan->setLayout(panlelayout);

    QHBoxLayout* vlayout = new QHBoxLayout();
    vlayout->addWidget(viewer);
    ui->frameView->setLayout(vlayout);

    this->update();

    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    //保存的menu
    //    QMenu *saveMenue = ui->btnSave->getMenu();
    //    saveMenue->addAction(tr("保存"),this,SLOT(slotSave()));
    //    saveMenue->addAction(tr("另存为"),this,SLOT(slotSaveAs()));


    ////////////
    //菜单的menu
    ////////////
    QMenu*toolMenu = ui->btnMenu->getMenu();
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString mode = psetting->value("mode").toString();
    QString uint = psetting->value("unit").toString();
    QString language = psetting->value("language").toString();
    bool b_first = psetting->value("showserial").toBool();
    psetting->endGroup();

    //语言
    languageMenu = new QMenu(toolMenu);
    languageMenu->setTitle(tr("语言"));
    languageGroup = new QActionGroup(this);
//    zhAction = new QAction(tr("简体中文"),this);
//    enAction = new QAction(tr("English"),this);

    zhAction = ui->zhAction;
    enAction = ui->enAction;
    connect(zhAction,SIGNAL(triggered()),this,SLOT(slotZhAction()));
    connect(enAction,SIGNAL(triggered()),this,SLOT(slotEnAction()));
    connect(hex,SIGNAL(Sig_Serial_Connect(QString)),this,SLOT(Serial_Connect(QString)));
    zhAction->setCheckable(true);
    enAction->setCheckable(true);
    QString a;
    if(language=="zh")
    {
        zhAction->setChecked(true);
        a = ":Language/c_zh.qm";
        ui->btnQrcode->setText(tr("二维码"));
        languageMenu->setTitle(tr("语言"));
    }
    else if(language=="en")
    {
        enAction->setChecked(true);
        a = ":Language/c_en.qm";
        ui->btnQrcode->setText(tr("Qrcode"));
        languageMenu->setTitle(tr("Language"));
    }
    //更新语言版本
    QTranslator *m_ens = new QTranslator();
    m_ens->load(a);
    qApp->installTranslator(m_ens);

    ui->retranslateUi(this);
//    easy->languageUpdate();
//    easyn->languageUpdate();
//    expert->languageUpdate();
//    text->languageUpdate();
//    control->languageUpdate();
//    gcodeer->languageUpdate();
//    rencode->languageUpdate();
//    postion->languageUpdate();
//    hex->languageUpdate();

    languageGroup->addAction(zhAction);
    languageGroup->addAction(enAction);
    languageMenu->addAction(zhAction);
    languageMenu->addAction(enAction);
    toolMenu->addMenu(languageMenu);

    //界面选择
    uiChoose = new QMenu(toolMenu);
    uiChoose->setTitle(tr("界面选择"));
    choseMenu    = new QActionGroup(this);
    expertAction     = ui->expertAction;
    coustomAction    = ui->coustomAction;
    expertAction->setCheckable(true);
    coustomAction->setCheckable(true);

    if(mode=="normal")
        coustomAction->setChecked(true);
    else if(mode=="expert")
        expertAction->setChecked(true);

    choseMenu->addAction(expertAction);
    choseMenu->addAction(coustomAction);
    uiChoose->addAction(expertAction);
    uiChoose->addAction(coustomAction);
    toolMenu->addMenu(uiChoose);
    connect(expertAction,SIGNAL(triggered()),this,SLOT(slotExpert()));
    connect(coustomAction,SIGNAL(triggered()),this,SLOT(slotCoustom()));

    //G代码模式
    gcodeAction = ui->gcodeAction;
    connect(gcodeAction,SIGNAL(triggered()),this,SLOT(slotGcodeMode()));
    toolMenu->addAction(gcodeAction);

    //固件升级
    updateAction = ui->firmwareAaction;
    connect(updateAction,SIGNAL(triggered()),this,SLOT(slotHexLoad()));
    toolMenu->addAction(updateAction);

    //硬件设置
//    toolMenu->addAction(tr("硬件设置"),this,SLOT(slotFirmSetting()));

    //刻度尺单位
    scaleMenu = new QMenu(toolMenu);
    scaleMenu->setTitle(tr("刻度尺单位"));
    QActionGroup *scaleGroup = new QActionGroup(this);
    mmAction = ui->mmAction;
    inchAction = ui->inchAction;
    mmAction->setCheckable(true);
    inchAction->setCheckable(true);
    if(uint=="inch")
    {
        inchAction->setChecked(true);
    }
    else if(uint =="mm")
    {
        mmAction->setChecked(true);
    }

    scaleGroup->addAction(mmAction);
    scaleGroup->addAction(inchAction);
    scaleMenu->addAction(mmAction);
    scaleMenu->addAction(inchAction);
    connect(mmAction,SIGNAL(triggered()),this,SLOT(slotMMAction()));
    connect(inchAction,SIGNAL(triggered()),this,SLOT(slotInchAction()));
    toolMenu->addMenu(scaleMenu);

    //是否开启首页窗口设置
//    serialAction = ui->serialAction;
//    serialAction->setCheckable(true);
//    if(b_first)
//        serialAction->setChecked(true);
//    else
//    {
//        serialAction->setChecked(false);
//    }

//    toolMenu->addAction(serialAction);
//    connect(serialAction,SIGNAL(toggled(bool)),this,SLOT(slotSerialAction(bool)));

    //软件版本与更新
    softwareAction = ui->downloadAction;
    connect(softwareAction,SIGNAL(triggered()),FvUpdater::sharedUpdater(),SLOT(CheckForUpdatesNotSilent()));
    toolMenu->addAction(softwareAction);
//    toolMenu->addAction(tr("软件版本与更新"),FvUpdater::sharedUpdater(),SLOT(CheckForUpdatesNotSilent()));
    //    connect(ui->updateButton, SIGNAL(clicked()),
    //            FvUpdater::sharedUpdater(), SLOT(CheckForUpdatesNotSilent()));
    //帮助
    //还未做对应的帮助文档和在线帮助网站
    //    QMenu *helpMenu = new QMenu(toolMenu);
    //    helpMenu->setTitle(tr("帮助"));
    //    QAction *easyAction = new QAction(tr("快速入门"),this);
    //    QAction *onlineAction = new QAction(tr("在线说明"),this);
    //    QAction *feedAction = new QAction(tr("反馈建议"),this);
    //    helpMenu->addAction(easyAction);
    //    helpMenu->addAction(onlineAction);
    //    helpMenu->addAction(feedAction);
    //    toolMenu->addMenu(helpMenu);

    //    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //    TitleBar *pTitleBar = new TitleBar(this);
    //    installEventFilter(pTitleBar);

    QVBoxLayout *pLayout = new QVBoxLayout();
    //    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameCent);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    //TextForm this
    connect(text,SIGNAL(Sig_AddFont(QString)),this,SLOT(slotAddFont(QString)));

    //BA connect
    connect(easy,SIGNAL(Sig_BA(QString)),this,SLOT(slotBA(QString)));

    //BB connect
    connect(easyn,SIGNAL(Sig_BB(QString)),this,SLOT(slotBB(QString)));

    //BC connect
    connect(this,SIGNAL(Sig_ReadParmeter()),expert,SLOT(slotUpdateParmeter()));
    connect(expert,SIGNAL(Sig_BC(QString)),this,SLOT(slotBC(QString)));
    //    connect(expert,SIGNAL(Sig_Carve()),this,SLOT(slotCarve()));
    connect(expert,SIGNAL(Sig_BD(QString)),this,SLOT(slotBD(QString)));

    //PostionForm ----> Viewer
    connect(posform,SIGNAL(Sig_WH(QPointF)),viewer,SLOT(slotSetHW(QPointF)));
    //BD connect
    connect(viewer,SIGNAL(Sig_CombinePrint(QStringList)),this,SLOT(slotBDCombineFile(QStringList)));
    connect(viewer,SIGNAL(Sig_ShowPos(QRectF)),this,SLOT(slotShowPos(QRectF)));

    connect(this,SIGNAL(Sig_SaveAs(QString)),viewer,SLOT(slotSaveAs(QString)));

    connect(this,SIGNAL(Sig_MAddFont(QString)),viewer,SLOT(slotMAddFont(QString)));
    connect(this,SIGNAL(Sig_BD_Open(QString)),viewer,SLOT(slotOpen(QString)));
    connect(this,SIGNAL(Sig_BD_Rect(QRect)),viewer,SLOT(slotRect(QRect)));

    connect(this,SIGNAL(Sig_Rencode()),rencode,SLOT(rencodeShow()));

    connect(this,SIGNAL(Sig_uintUpdate()),viewer,SLOT(uintUpdate()));

    //BA BB ----> This
    connect(easy,SIGNAL(Sig_Preview()),this,SLOT(slotPreview()));
    connect(easyn,SIGNAL(Sig_Cutting()),this,SLOT(slotCutting()));
    connect(easyn,SIGNAL(Sig_Carve()),this,SLOT(slotCarve()));
    connect(easyn,SIGNAL(Sig_Print()),this,SLOT(slotPrint()));
    connect(easyn,SIGNAL(Sig_Stop()),this,SLOT(slotStop()));
    connect(easyn,SIGNAL(Sig_Cancle()),this,SLOT(slotCancle()));

    connect(expert,SIGNAL(Sig_Cutting()),this,SLOT(slotCutting()));
    connect(expert,SIGNAL(Sig_Carve()),this,SLOT(slotCarve()));
    connect(expert,SIGNAL(Sig_Print()),this,SLOT(slotPrint()));
    connect(expert,SIGNAL(Sig_Stop()),this,SLOT(slotStop()));
    connect(expert,SIGNAL(Sig_Cancle()),this,SLOT(slotCancle()));
    connect(expert,SIGNAL(Sig_LaserOn(QString)),this,SLOT(slotLaserOn(QString)));
    connect(expert,SIGNAL(Sig_Preview()),this,SLOT(slotPreview()));

    //Pannel ----> This
    connect(control,SIGNAL(Sig_ToLaser(QString)),this,SLOT(slotToLaser(QString)));

    //This ----> Viewer
    //    connect(this,SIGNAL(Sig_ProcessBar(bool,unsigned int)),viewer,SLOT(setShow(bool,unsigned int)));
    connect(this,SIGNAL(Sig_LoadImageType(QString)),viewer,SLOT(slotLoadImageType(QString)));

    //Gcode -----> This
    connect(gcodeer,SIGNAL(Sig_RCmds(QString)),this,SLOT(slotToLaser(QString)));
//    connect(gcodeer,SIGNAL(Sig_Gcode(QString)),this,SIGNAL(Sig_GCODE(QString)));
    connect(gcodeer,SIGNAL(Sig_Gcode(QString)),this,SLOT(slotPrintGcode(QString)));

    QString vpath = QCoreApplication::applicationDirPath();
    QString vname = "/Config.ini";
    QString vallPath = QString("%1%2").arg(vpath).arg(vname);
    QSettings* vsetting = new QSettings(vallPath,QSettings::IniFormat);
    vsetting->beginGroup("update");
    QString versionNo = vsetting->value("version").toString();
    vsetting->endGroup();

    QString version = "mLaserV" + versionNo;
    setWindowTitle(version);
    qDebug() << "window title: " << windowTitle();
//     posform->show();
//    expert->setElementEnable(false);

}

QCPTitle::~QCPTitle()
{
    delete ui;
}
void QCPTitle::closeEvent(QCloseEvent *e)
{
    qApp->quit();
}
void QCPTitle::changeEvent(QEvent *event)
{
    QWidget *pWindow = this->window();
    this->repaint();
    //        if (pWindow->isTopLevel())
    //        {
    //            bool bMaximize = pWindow->isMaximized();
    //            if (bMaximize)
    //            {
    //               qDebug()<<"Fuck!!!!miniized";
    //            }
    //            else
    //            {
    //               qDebug()<<"Fuck:maxed!";
    //               this->repaint();
    //            }
    //        }
}
void QCPTitle::showEvent(QShowEvent *e)
{
    this->repaint();
    easy->repaint();
    easyn->repaint();
    expert->repaint();
}
void QCPTitle::setMode(QString m)
{
    easy->hide();
    easyn->hide();
    expert->hide();
    ui->btnSerial->hide();
    ui->btnText->hide();
    if(m=="normal")
    {
        easy->show();
        ui->btnSerial->hide();
        ui->btnText->hide();
        ui->frameTool->repaint();
    }
    else if(m=="expert")
    {
        expert->show();
        ui->btnSerial->show();
        ui->btnText->show();
        ui->frameTool->repaint();
    }
}
//切换到专家模式
void QCPTitle::slotExpert()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("mode",QString("expert"));
    psetting->endGroup();
    easy->hide();
    easyn->hide();
    ui->btnSerial->show();
    ui->btnText->show();
    ui->frameTool->repaint();
    expert->show();
}
//切换到普通模式
void QCPTitle::slotCoustom()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("mode",QString("normal"));
    psetting->endGroup();
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("carv_type",1);
    confsetting->endGroup();

    easy->show();
    easyn->hide();
    ui->btnSerial->hide();
    ui->btnText->hide();
    ui->frameTool->repaint();
    expert->hide();
}
void QCPTitle::slotZhAction()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("language",QString("zh"));
    psetting->endGroup();

    QTranslator *m_ens = new QTranslator();
    m_ens->load(":Language/c_zh.qm");
    qApp->installTranslator(m_ens);
    ui->btnQrcode->setText(tr("二维码"));
    ui->retranslateUi(this);
    easy->languageUpdate();
    easyn->languageUpdate();
    expert->languageUpdate();
    text->languageUpdate();
    control->languageUpdate();
    gcodeer->languageUpdate();
    rencode->languageUpdate();
    hex->languageUpdate();
    viewer->languageUpdate();

    posform->languageUpdate();//添加位置对话框翻译
//    fvupdate->languageUpdate();
    fvupdatewindow->languageUpdate();
    emit Sig_SerialPortTranslate();
    languageMenu->setTitle("语言");
    uiChoose->setTitle("界面选择");
    scaleMenu->setTitle("刻度单位");
}
void QCPTitle::slotEnAction()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("language",QString("en"));
    psetting->endGroup();

    QTranslator *m_ens = new QTranslator();
    m_ens->load(":Language/c_en.qm");
    qApp->installTranslator(m_ens);

    ui->retranslateUi(this);
//    ui->btnMenu->getMenu();
    ui->btnQrcode->setText(tr("Qcode"));
    easy->languageUpdate();
    easyn->languageUpdate();
    expert->languageUpdate();
    text->languageUpdate();
    control->languageUpdate();
    gcodeer->languageUpdate();
    rencode->languageUpdate();
    hex->languageUpdate();
    viewer->languageUpdate();
    fvupdatewindow->languageUpdate();
    emit Sig_SerialPortTranslate();
    posform->languageUpdate();//添加位置对话框翻译
    languageMenu->setTitle("Language");
    uiChoose->setTitle("Select Interface");
    scaleMenu->setTitle("Scale Unit");
}
//硬件设置
void QCPTitle::slotFirmSetting()
{
    //    firmform->show();
}

void QCPTitle::slotPrintGcode(QString file)
{
    QStringList fileList;
    fileList << file;
    emit Sig_PrintGcode(fileList);
}
void QCPTitle::slotInchAction()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("unit",QString("inch"));
    psetting->endGroup();
    qDebug()<<"slotInchAction";
    emit Sig_uintUpdate();
	
}
void QCPTitle::slotMMAction()
{ 
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("unit",QString("mm"));
    psetting->endGroup();
    qDebug()<<"slotMMAction";
	emit Sig_uintUpdate();
}
//是否在打开软件时显示串口连接界面
void QCPTitle::slotSerialAction(bool t)
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("showserial",t);
    psetting->endGroup();
}
//G代码模式
void QCPTitle::slotGcodeMode()
{
    gcodeer->show();
}
//烧录固件
void QCPTitle::slotHexLoad()
{
    emit Sig_ToLaser("disconnect");
    hex->show();
    hex->activateWindow();
}

void QCPTitle::Serial_Connect(QString com)
{
  qDebug()<<"Serial_Connect";
  emit Sig_SerialConnect(com);
}


/*
 *
 *接收和处理来自easy,easyn,viewer,expert实例的信号
 * ----starter------
 *
 */
void QCPTitle::slotBDCombineFile(QStringList cmd)
{
    //发送该信号给UIManager
    emit Sig_BDCombineFile(cmd);
}

/*
 *
 *接收和处理来自easy,easyn,viewer,expert实例的信号
 * ----end------
 *
 */

//保存文件的槽函数
//这里保存实际是进行切片处理
void QCPTitle::slotSave()
{

}
//保存对应的文件路径，相当于工程文件*.ml
//保存对应的文件在场景中的坐标
void QCPTitle::slotSaveAs()
{

}

void QCPTitle::slotBA(QString cmd)
{
    if(cmd=="next")
    {
        easy->hide();
        easyn->show();
    }
    else if(cmd =="backhome")
    {
        emit Sig_MWin("backhome");
    }
    else if(cmd=="showpannel")
    {
        control->show();
    }
    else if(cmd =="showtext")
    {
        text->show();
    }
    else if(cmd=="sethome")
    {
        emit Sig_ToLaser("G92 X0 Y0");
    }
}
void QCPTitle::slotBB(QString cmd)
{
    if(cmd=="pre")
    {
        easyn->hide();
        easy->show();
    }
    else if("showpannel"==cmd)
    {
        //显示打印机控制台
        control->show();
    }
}
void QCPTitle::slotBC(QString cmd)
{

}
void QCPTitle::slotBD(QString cmd)
{
    if(cmd=="showpannel")
    {
        control->show();
    }
}
void QCPTitle::slotShowPos(QRectF m)
{
    posform->setUIElement(m);
    posform->show();
}
//处理切割请求，将文件处理成轮廓打印
void QCPTitle::slotCutting()
{
    //获取场景中的灰度图文件路径列表
    //发送至对应的文件处理模块
    viewer->setPrintKind(0);
}
//处理雕刻请求，将当前的文件处理为灰度雕刻打印
void QCPTitle::slotCarve()
{
    viewer->setPrintKind(1);
}

void QCPTitle::slotToLaser(QString m)
{
    emit Sig_ToLaser(m);
}

void QCPTitle::slotPrint()
{
    if(bLoadedfile&&bConnected)
    {

        if(STATUS==IDLE)
        {
            //    viewer->showProcess(true);
            if(viewer->getPrintKind()==0)
            {
                viewer->getCuttingFile();
                //                emit Sig_Print();
                expert->setButtonIcon(false);
                easyn->setButtonIcon(false);
                viewer->setShow(false,0);
                STATUS = PRINTABLE;

            }
            else if(viewer->getPrintKind()==1)
            {
                qDebug()<<tr("灰度打印测试");
                if(bPic)
                {
                    viewer->getCarveFile();
                    expert->setButtonIcon(false);
                    easyn->setButtonIcon(false);
                    viewer->setShow(false,0);
                    STATUS = PRINTABLE;
                }
                else
                {
                    QMessageBox::information(this,tr("提示"),tr("SVG矢量图无法进行灰度雕刻！"));
                }
            }

            //    viewer->showProcess(false);
        }
        else if(STATUS==STOP)
        {
            emit Sig_Recover();
            expert->setButtonIcon(false);
            easyn->setButtonIcon(false);
            STATUS =PRINTABLE;
        }

    }
    else
    {
        QMessageBox::information(this,tr("注意"),tr("未加载文件或未连接串口！"));
    }

}
void QCPTitle::slotStop()
{
    if(STATUS==PRINTABLE)
    {
        STATUS = STOP;
        emit Sig_Stop();
        expert->setButtonIcon(true);
        easyn->setButtonIcon(true);
    }

}
void QCPTitle::slotCancle()
{
    emit Sig_Cancle();
    STATUS = IDLE;
    expert->setButtonIcon(true);
    easyn->setButtonIcon(true);
    viewer->setShow(false,0);
    this->setMaximumSize(this->maximumSize());
}
void QCPTitle::slotPreview()
{
    if(bLoadedfile)
    {
        emit Sig_Bounding(viewer->getBounding());
    }
    else
    {
        QMessageBox::information(this,tr("注意"),tr("未加载文件！"));
    }

}
//处理添加文字的请求
void QCPTitle::slotAddFont(QString cmd)
{
    QString path = QCoreApplication::applicationDirPath();
    QString file = cmd;
    int w = 800;
    int h = 800;
    double scale = 1.0;
    if(!file.isNull())
    {
        if(QFileInfo(file).suffix()=="svg")
        {
            QSvgRenderer render(file);
            QSize p = render.defaultSize();
            w = p.width();
            h = p.height();
            scale = w/(h*1.0);
        }

        else
        {
            QImage ps(file);
            {
                w = ps.width();
                h = ps.height();
                scale = w/(h*1.0);
            }

        }

        if(w>=1000||h>=1000)
        {
            if(scale>1.0)
            {
                w = 1000;
                h = 1000/scale;
            }
            else
            {
                h = 1000;
                w = 1000*scale;
            }

        }
        QImage fi(file);
        if(fi.depth()>8)
        {
            emit Sig_BD_Rect(QRect(0,0,w,h));
        }
        else if(fi.depth()<=8)
        {

            QFile::remove(path + "/24bit.png");
            QImage ne(file);

            QImage a = ne.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            a.save(path + "/24bit.png");
            emit Sig_BD_Rect(QRect(0,0,w,h));
        }

    }
    qDebug()<<"fuck"<<cmd;
    bLoadedfile = true;
    bPic = true;
//    easyn->setElementEnable(true);
//    expert->setElementEnable(true);
    emit Sig_MAddFont(cmd);

}
//弹出文字编辑框
void QCPTitle::on_btnText_clicked()
{
    text->activateWindow();
    text->show();
}
//弹出控制面板
void QCPTitle::on_btnOpenPanel_clicked()
{
    control->activateWindow();
    control->show();
}
//设置雕刻起点
void QCPTitle::on_btnSetHome_clicked()
{
    //    postion->activateWindow();
    //    postion->show();
}
//打开按键

void QCPTitle::on_btnOpen_clicked()
{
    QString path = QCoreApplication::applicationDirPath();
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* psetting = new QSettings(allPath,QSettings::IniFormat);
    psetting->beginGroup("laser");
    QString dir = psetting->value("dir").toString();
    QString file = QFileDialog::getOpenFileName(this,tr("OpenFile"),dir,tr("All (*.png *.jpg *.jpeg *.svg *.dxf *.bmp *.mbl);;Image Files (*.png *.jpg *.jpeg *.svg *.dxf);;Project (*.mbl)"));
    qDebug()<<"file"<<file;
    if(!file.isEmpty())
    {
        QFile::remove(path + "/show.gcode");
        on_btnDelet_clicked();
    }
    psetting->setValue("dir",file);
    psetting->endGroup();
    int w = 800;
    int h = 800;
    double scale = 1.0;
    if(!file.isNull())
    {
        if(QFileInfo(file).suffix()=="svg")             //svg
        {
            QSvgRenderer render(file);
            QSize p = render.defaultSize();
            w = p.width();
            h = p.height();
            scale = w/(h*1.0);
            if(w>=1000||h>=1000)
            {
                if(scale>1.0)
                {
                    w = 1000;
                    h = 1000/scale;
                }
                else
                {
                    h = 1000;
                    w = 1000*scale;
                }
            }
            emit Sig_BD_Rect(QRect(30,30,w,h));
            emit Sig_BD_Open(file);
            bPic = false;
        }
        else if(QFileInfo(file).suffix()=="dxf")
        {
//            DL_Dxf* dxf = new DL_Dxf();
//            if (!dxf->in(file.toStdString(), mcreationClass)) { // if file open failed
//                std::cerr << file.toStdString() << " could not be opened.\n";
//                return;
//            }
//            mcreationClass->setFile("dxf.svg");
//            QSvgRenderer render(QString("dxf.svg"));

//            QSvgRenderer render(QString("saveDXF.bmp"));
//            QSize p = render.defaultSize();
//            w = p.width();
//            h = p.height();
//            qDebug() << "w: " << w << endl;
//            qDebug() << "h: " << h << endl;
//            scale = w/(h*1.0);
//            if(w>=1000||h>=1000)
//            {
//                if(scale>1.0)
//                {
//                    w = 1000;
//                    h = 1000/scale;
//                }
//                else
//                {
//                    h = 1000;
//                    w = 1000*scale;
//                }
//            }
//            file = "dxf.svg";
//            file = "saveDXF.bmp";
//            emit Sig_BD_Rect(QRect(30,30,w,h));
//            emit Sig_BD_Open("saveDXF.bmp");
//            bPic = false;
            QString newFile = "saveDXF.bmp";
            QFile::remove(path + "/saveDXF.bmp");

            mdxf2bmg.setFile(file.toStdString());
            QImage ps(newFile);
            if(ps.format()<=3)
            {
                qDebug()<<"Your image is invalid!";
                w = ps.width();
                h = ps.height();
            }
            else
            {
                w = ps.width();
                h = ps.height();
                scale = w/(h*1.0);
                if(w>=1000||h>=1000)
                {
                    if(scale>1.0)
                    {
                        w = 1000;
                        h = 1000/scale;
                    }
                    else
                    {
                        h = 1000;
                        w = 1000*scale;
                    }

                }
            }
            QFile::remove(path + "/24bit.jpg");
            QImage ne(newFile);

            QImage a = ne.convertToFormat(QImage::Format_RGB32);
            a.save(path + "/24bit.jpg");
            emit Sig_BD_Rect(QRect(0,0,w,h));
            emit Sig_BD_Open(path + "/24bit.jpg");
            bPic = true;
        }
        else if(QFileInfo(file).suffix()=="mbl")
        {
            QString dspath = QCoreApplication::applicationDirPath();
            QString dsname = "/ds/config.ini";
            QString dsallPath = QString("%1%2").arg(dspath).arg(dsname);
            QString _dspath = dspath + "/ds";
            removeDirWithContent(_dspath);
            zip->unzip(file,_dspath);
            QSettings* pset = new QSettings(dsallPath,QSettings::IniFormat);
            QStringList m = pset->childGroups();
            for(int p=0;p<m.count();p++)
            {
                QString name = m.at(p);
                if(name != "laser")
                {
                    pset->beginGroup(name);
                    QString cFileName = pset->value("name").toString();
                    int bFont = pset->value("bfont").toInt();
                    int x = pset->value("x").toInt();
                    int y = pset->value("y").toInt();
                    int width = pset->value("width").toInt();
                    int height = pset->value("height").toInt();
                    QString filedir = _dspath + "/" + cFileName;
                    qDebug()<<"Get the file:"<<filedir;
                    if(bFont==0)
                    {
                        emit Sig_BD_Rect(QRect(x-30,y-30,width,height));
                        emit Sig_BD_Open(filedir);
                    }
                    else if(bFont==1)
                    {
                        emit Sig_BD_Rect(QRect(x-30,y-30,width,height));
                        emit Sig_MAddFont(filedir);
                    }
                    pset->endGroup();
                }
            }
            emit Sig_ReadParmeter();

        }
        else if(QFileInfo(file).suffix()=="png")
        {
            QImage pis(file);
            QImage alpha_image(pis.width(),pis.height(),QImage::Format_ARGB32_Premultiplied);
            QImage pis_image = pis.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            QImage show = pis.convertToFormat(QImage::Format_RGB32);
            if(pis_image.hasAlphaChannel())
            {
               alpha_image = pis_image.alphaChannel();
//               alpha_image.invertPixels(QImage::InvertRgba);
               show.invertPixels(QImage::InvertRgb);
               show.setAlphaChannel(alpha_image);
               show.invertPixels(QImage::InvertRgb);
               qDebug()<<"The image has alpha channel!";
            }
            QImage ps(file);
            if(ps.format()<=3)
            {
                qDebug()<<"Your image is invalid!";
                w = ps.width();
                h = ps.height();
            }
            else
            {
                w = ps.width();
                h = ps.height();
                scale = w/(h*1.0);
                if(w>=1000||h>=1000)
                {
                    if(scale>1.0)
                    {
                        w = 1000;
                        h = 1000/scale;
                    }
                    else
                    {
                        h = 1000;
                        w = 1000*scale;
                    }

                }
            }
            QFile::remove(path + "/24bit.jpg");
            show.save(path + "/24bit.jpg");
            emit Sig_BD_Rect(QRect(0,0,w,h));
            emit Sig_BD_Open(path + "/24bit.jpg");
            bPic = true;
        }
		else
        {
            QImage ps(file);
            if(ps.format()<=3)
            {
                qDebug()<<"Your image is invalid!";
                qDebug() << "ps.format(): " << ps.format();
                w = ps.width();
                h = ps.height();
            }
            else
            {
                w = ps.width();
                h = ps.height();
                scale = w/(h*1.0);
                if(w>=1000||h>=1000)
                {
                    if(scale>1.0)
                    {
                        w = 1000;
                        h = 1000/scale;
                    }
                    else
                    {
                        h = 1000;
                        w = 1000*scale;
                    }    

                }
            }
            QFile::remove(path + "/24bit.jpg");
            QImage ne(file);

            QImage a = ne.convertToFormat(QImage::Format_RGB32);
            a.save(path + "/24bit.jpg");
            emit Sig_BD_Rect(QRect(0,0,w,h));
            emit Sig_BD_Open(path + "/24bit.jpg");
            bPic = true;
        }
        bLoadedfile = true;
//        easyn->setElementEnable(true);
//        expert->setElementEnable(true);
    }
    //发送加载图片类型信号-->用于缩放图像时重新加载图片，防止像素损失

/*
 * Mac版本bug问题出处
 */
    if(!file.isNull() && QFileInfo(file).suffix()!="svg")
    {
        if(QFileInfo(file).suffix()=="mbl")
        {
            emit Sig_LoadImageType("mbl");
        }
        else
        {
            emit Sig_LoadImageType("24bit");
        }
    }



}
//删除按键
void QCPTitle::on_btnDelet_clicked()
{
    viewer->resetAll();
    bLoadedfile = false;
    STATUS = IDLE;
    //删除24bit.jpg
    QString path = QCoreApplication::applicationDirPath();
    QString current_file_image = path + "/24bit.jpg";
    QString current_file_font = path + "/font1.jpg";
//    qDebug() << "yangxu1: " << current_file;

    QFile image(current_file_image);
    image.remove(current_file_image);
    QFile font(current_file_font);
    font.remove(current_file_font);
    //
//    viewer->resetAll();
//    easyn->setElementEnable(false);
//    expert->setElementEnable(false);
}
//串口
void QCPTitle::on_comboBoxSerial_activated(const QString &arg1)
{
    emit Sig_SerialPort(arg1);
}
//修改打印进度条
void QCPTitle::slotFinishPersent(unsigned int m)
{
    viewer->setShow(true,m);
}
//串口,按下，传递参数在
void QCPTitle::on_btnSerial_clicked()
{
    emit Sig_SerialPort("show");
}
void QCPTitle::slotLaserOn(QString m)
{
    emit Sig_ToLaser(m);
}

void QCPTitle::enableUserWaitingCursor()
{
    QCursor curs;
    curs.setShape(Qt::WaitCursor);
    QApplication::setOverrideCursor(curs);
}

void QCPTitle::disableUserWaitingCursor()
{
    QCursor curs;
    curs.setShape(Qt::ArrowCursor);
    QApplication::setOverrideCursor(curs);
}

//处理进度条的槽函数
void QCPTitle::slotProcessBar(bool s, quint64 m)
{
    qDebug()<<"finish persent:"<<QString::number(m);
    if((!s)&&(m==0))
    {
        viewer->setShow(false,0);
        easyn->setButtonIcon(true);
        expert->setButtonIcon(true);
        STATUS = IDLE;
        emit Sig_Cancle();
//        expert->setElementEnable(true);

    }
    else
        viewer->setShow(true,m);
}

//接受来自FrontEnd的串口连接失败的信号
void QCPTitle::slotConnectFaile(bool b)
{
    bConnected = b;
    if(b)
    {
//        expert->setElementEnable(true);
    }
}
//显示二维码
void QCPTitle::on_btnQrcode_clicked()
{
    emit Sig_Rencode();
    rencode->show();
    rencode->activateWindow();
}
//保存为工程文件，工程文件中记录了打印的信息
void QCPTitle::on_btnSave_clicked()
{
    //创建临时目录和配置文件
    QString propath = QCoreApplication::applicationDirPath();
    QString proname = "/project/config.ini";
    QString proallPath = QString("%1%2").arg(propath).arg(proname);
    QString _propath = propath + "/project";
//    QDir *temp = new QDir();
//    bool exist = temp->exists(_propath);
    removeDirWithContent((_propath));
    //    QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
//    bool ok = temp->mkdir(_propath);
    QFile config(proallPath);
    config.open(QIODevice::ReadWrite);
    config.close();

    //保存当前打印相关的参数
    int type = viewer->getPrintKind();
    if(type ==0)        //切割
    {
        //获取材料相关的参数设置
        QString material = expert->getMaterialType();
        qDebug() << "material: " << material;
        if(material == "纸张")
        {
            material = "Paper";
        }
        if(material == "木头")
        {
            material = "Wood";
        }
        if(material == "皮革")
        {
            material = "Leather";
        }
		QString mpath = QCoreApplication::applicationDirPath(); 
	    QString mname = "/material.ini";
        QString mallPath = QString("%1%2").arg(mpath).arg(mname);
        QSettings * pset = new QSettings(mallPath,QSettings::IniFormat);
        pset->beginGroup(material);
        QString times = pset->value("times").toString();
        QString laserPowerHigh=pset->value("laserPowerHigh").toString();
        QString laserPowerLow=pset->value("laserPowerLow").toString();
        QString laserSpeed=pset->value("laserSpeed").toString();
        QString laserTravelSpeed = pset->value("laserTravelSpeed").toString();
        pset->endGroup();
        QSettings * config = new QSettings(proallPath,QSettings::IniFormat);
        config->beginGroup("laser");
        config->setValue("type",0);
//        qDebug() << "material: " << material;
        config->setValue("material", material);//增加material属性
//        qDebug() << "times: " << times;
        config->setValue("times",times);
//        qDebug() << "laserPowerHigh: " << laserPowerHigh;
        config->setValue("laserPowerHigh",laserPowerHigh);
//        qDebug() << "laserPowerLow: " << laserPowerLow;
        config->setValue("laserPowerLow",laserPowerLow);
        config->setValue("laserSpeed",laserSpeed);
        config->setValue("laserTravelSpeed",laserTravelSpeed);
        config->endGroup();

    }
    else if(type==1)    //雕刻
    {
        //获取雕刻相关的参数设置
        QString path = QCoreApplication::applicationDirPath(); 
	    QString name = "/Config.ini";
        QString allPath = QString("%1%2").arg(path).arg(name);
        QSettings * pset = new QSettings(allPath,QSettings::IniFormat);
        pset->beginGroup("laser");
        int carv_type=pset->value("carv_type").toInt();
        QString p_power_time = pset->value("p_power_time").toString();
        QString p_move_speed = pset->value("p_move_speed").toString();
        QString p_power = pset->value("p_power").toString();
        QString r_per_pixcel = pset->value("r_per_pixcel").toString();
        pset->endGroup();

        QSettings * config = new QSettings(proallPath,QSettings::IniFormat);
        config->beginGroup("laser");
        config->setValue("type",1);
        config->setValue("carv_type",carv_type);
        config->setValue("p_power_time",p_power_time);
        config->setValue("p_move_speed",p_move_speed);
        config->setValue("p_power",p_power);
        config->setValue("r_per_pixcel",r_per_pixcel);
        config->endGroup();
    }
    //保存位置参数以及图片相关的参数
    viewer->slotSaveAs("rr");
    QString storeName = QFileDialog::getSaveFileName(this,tr("保存工程"),".","Project (*.mbl)");
    zip->zip(_propath,storeName);

}

////删除某一目录
bool QCPTitle::removeDirWithContent(const QString &folderDir)
{
    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())  {return false;}//文件不存，则返回false
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0)//跳出条件
    {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--)
        {
            curFile=fileList[i];
            if(curFile.isFile())//如果是文件，删除文件
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
            if(curFile.isDir())//如果是文件夹
            {
                QDir dirTemp(curFile.filePath());
                QFileInfoList fileList1=dirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size()==0)//下层没有文件或文件夹
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j=0;j<fileList1.size();j++)
                    {
                        if(!(fileList.contains(fileList1[j])))
                            fileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
   return true;
}
