#ifndef CPSERIALPORT_H
#define CPSERIALPORT_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include "UICompment/qcptitle.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class CPSerialPort;
}

class CPSerialPort : public QWidget
{
    Q_OBJECT

public:
    explicit CPSerialPort(QWidget *parent = 0);
    ~CPSerialPort();
    void languageUpdate();

public slots:
    void Slot_ResponseUart();
    void WritePort(QString str);
    void ConnectPort(QString);
    void slotSerialPortTranslate();
private:
    Ui::CPSerialPort *ui;
    QSerialPort *mySerialport;
    QStringList comList;
    QString comName;
    bool bConnect;
signals:
    void Sig_GetMessage(QString);
    void Sig_Disconnect();
    void Sig_ConnectFail(bool b);
private slots:
    void on_updateUart_clicked();
    void on_btnConnect_clicked();
};

#endif // CPSERIALPORT_H
