#include <QApplication>
#include <QSettings>

#include "Server/httpserver/httplistener.h"
#include "Server/httpserver/httpsessionstore.h"
#include "Server/requestrouter.h"

#include "./Update/fvupdater.h"
#include "./UICompment/uimanager.h"
#include "./Communicate/serial/cpserialport.h"
#include "./Communicate/serial/frontend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UIManager *w = new UIManager();
    CPSerialPort* printport = new CPSerialPort();
    FrontEnd* frontdoor = new FrontEnd(NULL,printport);

//    QApplication::setApplicationName("mLaser");
//    QApplication::setApplicationVersion("2.0");
    QApplication::setOrganizationName("pypt");
    QApplication::setOrganizationDomain("pypt.lt");

    // Set feed URL before doing anything else
    QSettings* pset = new QSettings("./Config.ini",QSettings::IniFormat);
    pset->beginGroup("update");
    QString url = pset->value("url").toString();
    pset->endGroup();


    FvUpdater::sharedUpdater()->SetFeedURL(url);
    FvUpdater::sharedUpdater()->CheckForUpdatesSilent();

    //UI---->FrontEnd
    QObject::connect(w,SIGNAL(Sig_FrontEndCombine(QStringList)),frontdoor,SLOT(slotCombineFile(QStringList)));
    QObject::connect(w,SIGNAL(Sig_FrontEndCMD(QString)),frontdoor,SLOT(slotUIWrite(QString)));
    QObject::connect(w,SIGNAL(Sig_SerialConnect(QString)),frontdoor,SLOT(slotSerialConnect(QString)));
    QObject::connect(w,SIGNAL(Sig_Print()),frontdoor,SLOT(slotPrint()));
    QObject::connect(w,SIGNAL(Sig_Stop()),frontdoor,SLOT(slotStop()));
    QObject::connect(w,SIGNAL(Sig_Cancle()),frontdoor,SLOT(slotCancle()));
    QObject::connect(w,SIGNAL(Sig_Recover()),frontdoor,SLOT(slotRecov()));
    QObject::connect(w,SIGNAL(Sig_SerialPort(QString)),frontdoor,SLOT(slotConnectPort(QString)));
    QObject::connect(w,SIGNAL(Sig_Bounding(QRectF)),frontdoor,SLOT(slotDrawBounding(QRectF)));


    //FrontEnd---->UI
    QObject::connect(frontdoor,SIGNAL(Sig_FinishPersent(unsigned int)),w,SLOT(slotFinishPersent(unsigned int)));
    QObject::connect(frontdoor,SIGNAL(Sig_ProcessBar(bool,quint64)),w,SLOT(slotProcessBar(bool,quint64)));
    QObject::connect(frontdoor,SIGNAL(Sig_ConnectFaile(bool)),w,SLOT(slotConnectFaile(bool)));
    QObject::connect(frontdoor,SIGNAL(Sig_ConnectFirstFaile(bool)),w,SLOT(slotConnectFirstFaile(bool)));
    QObject::connect(frontdoor,SIGNAL(Sig_EndStopState(int)),w,SLOT(slotEndStopState(int)));
    frontdoor->start();

    /*
     *
     * Server begin
     *
     * */
    QSettings* listenerSetting = new QSettings("etc/webapp.ini",QSettings::IniFormat,&a);
    //get file settings
    listenerSetting->beginGroup("files");
    RequestRouter::staticFileControler = new StaticFileController(listenerSetting,&a);
    qDebug()<<QObject::tr("this root doc path:%1").arg(listenerSetting->value("path").toString());
    listenerSetting->endGroup();

    //get session setting
    listenerSetting->beginGroup("sessions");
    RequestRouter::sessionStore=new HttpSessionStore(listenerSetting,&a);
    listenerSetting->endGroup();

    //Get the listener setting
    //Generatr HTTP server
    listenerSetting->beginGroup("listener");
    RequestRouter *router = new RequestRouter();
    new HttpListener(listenerSetting,router,&a);
    listenerSetting->endGroup();
    QObject::connect(router,SIGNAL(Sig_ServerLoad(QString)),w,SLOT(slotServerLoad(QString)));
    /************* Server end***************/

    return a.exec();
}
