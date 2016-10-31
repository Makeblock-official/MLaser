#include <QDebug>
#include <QMessageBox>
#include "ac_autocheckform.h"
#include "ui_ac_autocheckform.h"

AC_AutoCheckForm::AC_AutoCheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AC_AutoCheckForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("自动检测");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameCheck);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    check = new QTimer();
    bConnect = false;
    connect(check,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
    //    check->start(2000);
}

AC_AutoCheckForm::~AC_AutoCheckForm()
{
    delete ui;
}
void AC_AutoCheckForm::timeStart()
{
    if(bConnect)
        check->start(500);
    else
        QMessageBox::information(NULL,tr("提示"),tr("串口未连接，请返回上一步连接串口！"));
}
void AC_AutoCheckForm::on_btnPre_clicked()
{

    emit Sig_AC("pre");
    check->stop();
}

void AC_AutoCheckForm::on_btnAutoCheck_clicked()
{

    emit Sig_AC("next");
    check->stop();
}

void AC_AutoCheckForm::slotTimeOut()
{
//    qDebug()<<"time out";
    emit Sig_ToLaser("M119");
//    emit Sig_ToLaser("G28 X Y");
}
//更新语言
void AC_AutoCheckForm::languageUpdate()
{
    ui->retranslateUi(this);
}

//开始机器自检，执行机器X Y轴复位，验证是否安装OK
void AC_AutoCheckForm::on_btnAutoHome_clicked()
{
    emit Sig_ToLaser("G28 X Y");
}
void AC_AutoCheckForm::setButtonStyle(int m)
{
    qDebug()<<tr("In the button style,value=%1").arg(m);
    switch(m)
    {
    case 0:
        qDebug()<<"X pressed";
        ui->btnX->setStyleSheet("QPushButton#btnX{background-color:rgb(111, 158, 77);}");
        break;

    case 1:
        qDebug()<<"Y pressed";
        ui->btnY->setStyleSheet("QPushButton#btnY{background-color:rgb(111, 158, 77);}");
        break;

    case 2:
        qDebug()<<"X open";
        ui->btnX->setStyleSheet("QPushButton#btnX{background-color:rgb(197, 97, 97);}");
        break;

    case 3:
        qDebug()<<"Y open";
        ui->btnY->setStyleSheet("QPushButton#btnY{background-color:rgb(190, 97, 97);}");
        break;
    }
    this->repaint();
}
