#include "cpserialport.h"
#include "ui_cpserialport.h"

CPSerialPort::CPSerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPSerialPort)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("SerialPort"));
    QIcon fix(QPixmap(":/Source/icon/logo.png"));
    setWindowIcon(fix);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frame);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    mySerialport = new QSerialPort();
    ui->portCombo->setStyleSheet("QComboBox{border:1px solid gray;}"
                                 "QComboBox QAbstractItemView::item{height:25px;}");
    ui->portCombo->setView(new QListView());

    //init serial and show it in the UI combo
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
		qDebug()<<info.portName()<<info.description()<<info.manufacturer();
		comList.append(info.portName());
        if (serial.open(QIODevice::ReadWrite))
        {
          serial.close();
        }
    }
    ui->portCombo->addItems(comList);
    connect(ui->portCombo,SIGNAL(Sig_Popup()),this,SLOT(on_updateUart_clicked()));
}

CPSerialPort::~CPSerialPort()
{
    delete ui;
}

void CPSerialPort::languageUpdate()
{
    ui->retranslateUi(this);
}


void CPSerialPort::Slot_ResponseUart()
{
    QString po = mySerialport->readAll();
//        QString po = mySerialport->readLine();
    emit Sig_GetMessage(po);
}
void CPSerialPort::WritePort(QString str)
{
    if(bConnect)
    {
        QString s = str + "\n";
        mySerialport->write(s.toLatin1());
    }
    else
    {
        QMessageBox::information(this,tr("Notice"),tr("Serial open failed."));

    }
    //    qDebug()<<"serial write:"<<str;
}

void CPSerialPort::on_updateUart_clicked()
{
    ui->portCombo->clear();
    comList.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        qDebug()<<info.portName()<<info.description()<<info.manufacturer();
        comList.append(info.portName());
        if (serial.open(QIODevice::ReadWrite))
        {
          serial.close();
        }
    }
    ui->portCombo->addItems(comList);
}

void CPSerialPort::on_btnConnect_clicked()
{
    if(!bConnect)
    {
        comName = ui->portCombo->currentText();
        mySerialport->setPortName(comName);
        mySerialport->setBaudRate(115200);
        mySerialport->setDataBits(QSerialPort::Data8);
        mySerialport->setParity(QSerialPort::NoParity);
        mySerialport->setStopBits(QSerialPort::OneStop);
        mySerialport->setFlowControl(QSerialPort::NoFlowControl);
        if(mySerialport->open(QIODevice::ReadWrite))
        {
            QString com = ui->portCombo->currentText();
            QString lpath = QCoreApplication::applicationDirPath(); 
            QString lname = "/mLaser.ini";
            QString lallPath = QString("%1%2").arg(lpath).arg(lname);
            QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
            psetting->beginGroup("mode");
            psetting->setValue("serial",com);
            psetting->endGroup();
            connect(mySerialport,SIGNAL(readyRead()),this,SLOT(Slot_ResponseUart()));
            ui->btnConnect->setText(tr("Disconnect"));
            qDebug()<<comName << "connected";
            bConnect = true;
        }
        else
        {
            qDebug()<<comName << "Serial open failed";
            QMessageBox::information(this,tr("Notice"),tr("Serial open failed."));
        }
    }
    else
    {
        bConnect = false;
        ui->btnConnect->setText(tr("Connect"));
        qDebug()<<comName << "disconnected";
        mySerialport->disconnect();
        mySerialport->close();
        emit Sig_Disconnect();
    }
}

void CPSerialPort::ConnectPort(QString m)
{
    if(!m.isEmpty())
    {
        if(!bConnect)
        {
            comName = m;
            mySerialport->setPortName(comName);
            mySerialport->setBaudRate(115200);
            mySerialport->setDataBits(QSerialPort::Data8);
            mySerialport->setParity(QSerialPort::NoParity);
            mySerialport->setStopBits(QSerialPort::OneStop);
            mySerialport->setFlowControl(QSerialPort::NoFlowControl);
            if(mySerialport->open(QIODevice::ReadWrite))
            {
                QString com = ui->portCombo->currentText();
                QString lpath = QCoreApplication::applicationDirPath(); 
                QString lname = "/mLaser.ini";
                QString lallPath = QString("%1%2").arg(lpath).arg(lname);
                QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
                psetting->beginGroup("mode");
                psetting->setValue("serial",com);
                psetting->endGroup();
                connect(mySerialport,SIGNAL(readyRead()),this,SLOT(Slot_ResponseUart()));
                ui->btnConnect->setText(tr("Disconnect"));
                qDebug()<<comName << "connected 2";
                bConnect = true;
            }
            else
            {
                QMessageBox::information(this,tr("Notice"),tr("Serial open failed."));
                qDebug()<<comName << "Serial open failed 2";
            }
            //emit Sig_ConnectFail(true);

        }
        else
        {
            bConnect = false;
            ui->btnConnect->setText(tr("Connect"));
            qDebug()<<comName << "disconnected 2";
            mySerialport->disconnect();
            mySerialport->close();
            emit Sig_Disconnect();
        }
    }

}

void CPSerialPort::slotSerialPortTranslate()
{
    this->languageUpdate();
}
