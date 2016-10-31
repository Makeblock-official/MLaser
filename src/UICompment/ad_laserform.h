#ifndef AD_LASERFORM_H
#define AD_LASERFORM_H

#include <QWidget>
#include "qcptitle.h"
namespace Ui {
class AD_LaserForm;
}

class AD_LaserForm : public QWidget
{
    Q_OBJECT

public:
    explicit AD_LaserForm(QWidget *parent = 0);
    ~AD_LaserForm();
    void languageUpdate();

private:
    Ui::AD_LaserForm *ui;
    bool bPower;
signals:
    void Sig_AD(QString);
    void Sig_ToLaser(QString);
private slots:
    void on_btnPre_clicked();
    void on_btnLaser_clicked();
    void on_btnNext_clicked();
};

#endif // AD_LASERFORM_H
