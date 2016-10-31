#ifndef LOADFORM_H
#define LOADFORM_H

#include <QWidget>
#include <QListView>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "titlebar.h"

namespace Ui {
class LoadForm;
}

class LoadForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoadForm(QWidget *parent = 0);
    ~LoadForm();
    void languageUpdate();
    void hideCheckBox();
    void setInitState(bool);
public slots:
    void on_checkBox_toggled(bool checked);
    void slotConnectFaile(bool m);
    void slotTimeOut();
private slots:
    void on_btnUpdate_clicked();

    void on_btnConnect_clicked();
    void on_btnNext_clicked();

private:
    Ui::LoadForm *ui;
    QSerialPort *mySerialport;
    QStringList comList;
    QTimer* autocheck;
    bool bConnect;
    bool bState;


signals:
    void Sig_SerialConnect(QString);
    void Sig_ShowLoad();
    void Sig_LoadNext();
};

#endif // LOADFORM_H
