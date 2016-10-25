#ifndef AC_AUTOCHECKFORM_H
#define AC_AUTOCHECKFORM_H

#include <QWidget>
#include <QTimer>
#include "qcptitle.h"
namespace Ui {
class AC_AutoCheckForm;
}

class AC_AutoCheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit AC_AutoCheckForm(QWidget *parent = 0);
    ~AC_AutoCheckForm();
    void languageUpdate();
    void timeStart();
    void setConnectState(bool b){bConnect = b;}
    void setButtonStyle(int m);
public slots:
    void slotTimeOut();
private:
    Ui::AC_AutoCheckForm *ui;
    QTimer* check;
    bool bConnect;
signals:
    void Sig_AC(QString);
    void Sig_ToLaser(QString);
private slots:
    void on_btnPre_clicked();
    void on_btnAutoCheck_clicked();
    void on_btnAutoHome_clicked();
};

#endif // AC_AUTOCHECKFORM_H
