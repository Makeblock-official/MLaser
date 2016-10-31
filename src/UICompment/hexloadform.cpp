#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>
#include <QSettings>
#include "hexloadform.h"
#include "ui_hexloadform.h"

HexLoadForm::HexLoadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexLoadForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("固件更新"));
    setWindowIcon(QIcon(":/Source/icon/logo.jpg"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frame);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    movie = new QMovie(":/Source/gif/wait2.gif");
    thread = new QThread();
    hexBase = new HexLoadBase();

    ui->label->setMovie(movie);
    ui->label->hide();
    ui->btnHex->hide();

    ui->portCombo->setStyleSheet("QComboBox{border:1px solid gray;}"
                                 "QComboBox QAbstractItemView::item{height:25px;}");
    ui->portCombo->setView(new QListView());

    QSettings* psetting = new QSettings("mLaser.ini",QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString serial = psetting->value("serial").toString();
    psetting->endGroup();
    comList.append(serial);
    ui->portCombo->addItems(comList);
    connect(ui->portCombo,SIGNAL(Sig_Popup()),this,SLOT(UpdateSerial()));
    connect(this,SIGNAL(Sig_Process(QString,QString)),hexBase,SLOT(HexLoad(QString,QString)));
    connect(hexBase,SIGNAL(Sig_FinishHexLoad()),this,SLOT(slotFinishHexLoad()));
    hexBase->moveToThread(thread);
    thread->start();
}

HexLoadForm::~HexLoadForm()
{
    delete ui;
}
//更新串口
void HexLoadForm::UpdateSerial()
{
    comList.clear();
    ui->portCombo->clear();
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
    ui->portCombo->addItems(comList);
    ui->label->hide();
}
//选择对应的Hex文件
void HexLoadForm::on_btnHex_clicked()
{
    file = QFileDialog::getOpenFileName(this,tr("OpenHex"),".","Hex(*.hex)");
//    file = "./mLaser.hex";
}
//烧录固件
void HexLoadForm::on_btnLoad_clicked()
{
    file = "./mLaser.hex";
    QString a = ui->portCombo->currentText();
    emit Sig_Process(file,a);
    ui->label->show();
    movie->start();
}
void HexLoadForm::languageUpdate()
{
    ui->retranslateUi(this);
}

//停止刷新界面
void HexLoadForm::slotFinishHexLoad()
{
//    movie->stop();
    QMessageBox::information(this,tr("提示"),tr("固件刷新完成！软件将会自动关闭，关闭后请拔出USB，重新连接一次。"));
    ui->label->hide();
    qApp->quit();
}
