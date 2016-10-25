#include <QVBoxLayout>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include "loadform.h"
#include "ui_loadform.h"

LoadForm::LoadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadForm)
{
    ui->setupUi(this);
    bState = false;
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);

    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameLoad);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    QSettings* psetting = new QSettings("mLaser.ini",QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString serial = psetting->value("serial").toString();
    bool b_first = psetting->value("showserial").toBool();
    psetting->endGroup();
    comList.append(serial);
    ui->comboBoxSerial->addItems(comList);

    ui->comboBoxSerial->setStyleSheet("QComboBox{border:1px solid gray;}"
                                      "QComboBox QAbstractItemView::item{height:25px;}");
    ui->comboBoxSerial->setView(new QListView());

    if(b_first)
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
        ui->checkBox->setChecked(false);
    }
    connect(ui->comboBoxSerial,SIGNAL(Sig_Popup()),this,SLOT(on_btnUpdate_clicked()));
    ui->btnNext->hide();
    autocheck = new QTimer();
    connect(autocheck,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
}

LoadForm::~LoadForm()
{
    delete ui;
}
//更新串口
void LoadForm::on_btnUpdate_clicked()
{
    comList.clear();
    ui->comboBoxSerial->clear();
    comList.clear();
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
    ui->comboBoxSerial->addItems(comList);
}

void LoadForm::on_btnConnect_clicked()
{
    //连接串口
    QString com = ui->comboBoxSerial->currentText();
    QSettings* psetting = new QSettings("mLaser.ini",QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("serial",com);
    psetting->endGroup();
    emit Sig_SerialConnect(com);
    ui->btnConnect->setText(tr("正在连接.."));
    autocheck->start(3500);
}
void LoadForm::setInitState(bool b)
{
    bState = b;
}
void LoadForm::slotTimeOut()
{
    QMessageBox::information(NULL,tr("提示"),tr("串口连接失败，请检查是否连接串口或者端口号，或重新插拔USB口。"));
//    QMessageBox::information(NULL,tr("Notice"),tr("Serial open failed."));
    ui->btnConnect->setText(tr("连接串口"));
    autocheck->stop();
}
//接收来自通信类的连接成功信号
void LoadForm::slotConnectFaile(bool m)
{
    if(m)
    {
        ui->btnConnect->setText(tr("断开连接"));
        autocheck->stop();
        //        QMessageBox::information(this,tr("提示"),tr("串口连接成功"));
        if(!bState)
        {
            this->close();
            emit Sig_ShowLoad();
        }
        qDebug()<<"Now in load form serial slot";
    }

}

void LoadForm::hideCheckBox()
{
    ui->checkBox->hide();
    ui->btnNext->show();
}
void LoadForm::languageUpdate()
{
    ui->retranslateUi(this);
    setWindowTitle(tr("Port"));
}

//发送下一步信号
void LoadForm::on_btnNext_clicked()
{
    emit Sig_LoadNext();
}
//开启串口提示对话框
void LoadForm::on_checkBox_toggled(bool checked)
{
    QSettings* psetting = new QSettings("mLaser.ini",QSettings::IniFormat);
    psetting->beginGroup("mode");
    psetting->setValue("showserial",checked);
    psetting->endGroup();
}
