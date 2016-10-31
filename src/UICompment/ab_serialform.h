#ifndef AB_SERIALFORM_H
#define AB_SERIALFORM_H

#include <QWidget>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "qcptitle.h"
#include "passwordform.h"
namespace Ui {
class AB_SerialForm;
}

class AB_SerialForm : public QWidget
{
    Q_OBJECT

public:
    explicit AB_SerialForm(QWidget *parent = 0);
    ~AB_SerialForm();
    void languageUpdate();

private:
    Ui::AB_SerialForm *ui;
    QStringList comList;
    PasswordForm* password;

signals:
    void Sig_AB(QString);
    void Sig_LoadSuccessful();
private slots:
    void on_btnNext_clicked();
    void on_lineEditPass_returnPressed();
    void on_btnChange_clicked();
    void slotShow();
    void on_btnLoagin_clicked();
};

#endif // AB_SERIALFORM_H
