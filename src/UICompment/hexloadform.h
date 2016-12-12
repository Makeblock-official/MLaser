#ifndef HEXLOADFORM_H
#define HEXLOADFORM_H

#include <QWidget>
#include <QMovie>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "titlebar.h"
#include "hexloadbase.h"

namespace Ui {
class HexLoadForm;
}

class HexLoadForm : public QWidget
{
    Q_OBJECT

public:
    explicit HexLoadForm(QWidget *parent = 0);
    ~HexLoadForm();
    void languageUpdate();
public slots:
    void UpdateSerial();
    void slotFinishHexLoad();
private slots:
    void on_btnHex_clicked();

    void on_btnLoad_clicked();

private:
    Ui::HexLoadForm *ui;
    QStringList comList;
    QThread* thread;
    QString file;
    QMovie *movie;
    HexLoadBase* hexBase;
signals:
    void Sig_Process(QString,QString);
    void Sig_Serial_Connect(QString);
};

#endif // HEXLOADFORM_H
