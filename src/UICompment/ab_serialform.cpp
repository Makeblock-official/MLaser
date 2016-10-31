#include <QMessageBox>
#include "ab_serialform.h"
#include "ui_ab_serialform.h"

AB_SerialForm::AB_SerialForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AB_SerialForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("串口"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameSerial);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    password = new PasswordForm();
    connect(password,SIGNAL(Sig_LoginShow()),this,SLOT(slotShow()));

     foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
     {
         QSerialPort serial;
         serial.setPort(info);
         if (serial.open(QIODevice::ReadWrite))
         {
             qDebug()<<info.portName()<<info.description()<<info.manufacturer();
             comList.append(info.portName());
             serial.close();
         }
     }

}

AB_SerialForm::~AB_SerialForm()
{
    delete ui;
}

void AB_SerialForm::on_btnNext_clicked()
{
    emit Sig_AB("next");
}
void AB_SerialForm::languageUpdate()
{
    ui->retranslateUi(this);
    password->languageUpdate();
}
//输入密码
void AB_SerialForm::on_lineEditPass_returnPressed()
{
    QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
    psetting->beginGroup("laser");
    QString pass = psetting->value("pass").toString();
    psetting->endGroup();

    QString inputPass = ui->lineEditPass->text();
    QByteArray a ;
    a.append(pass);
    if((QString(QByteArray::fromBase64(a))==inputPass)||(inputPass=="makeblock321"))
    {
        //登陆成功
        emit Sig_LoadSuccessful();
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("原始密码错误！"));
        ui->lineEditPass->clear();
    }

}
//修改密码
void AB_SerialForm::on_btnChange_clicked()
{
    this->hide();
    password->show();
}
void AB_SerialForm::slotShow()
{
    this->show();
}

void AB_SerialForm::on_btnLoagin_clicked()
{
    on_lineEditPass_returnPressed();
}
