#ifndef BC_EXPERTFORM_H
#define BC_EXPERTFORM_H

#include <QWidget>

namespace Ui {
class BC_ExpertForm;
}

class BC_ExpertForm : public QWidget
{
    Q_OBJECT

public:
    explicit BC_ExpertForm(QWidget *parent = 0);
    ~BC_ExpertForm();
    void languageUpdate();
    void setElementEnable(bool bEnable);
    void setButtonIcon(bool );
    QString getMaterialType();
public slots:
    void slotUpdateParmeter();
private slots:
    void on_toggleLaser_valueChanged(int value);
    void on_btnSlicer_clicked();
    void on_btnCarve_clicked();
    void on_btnOpenPanel_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnPreview_clicked();
    void on_btnSetHome_clicked();
    void on_comboMaterial_currentIndexChanged(const QString &arg1);
    void on_lineEditRepeat_returnPressed();
    void on_lineEditPower_returnPressed();
    void on_lineEditPixSpeed_returnPressed();
    void on_lineEditSpaceSpeed_returnPressed();
    void on_comboMaterial_currentIndexChanged(int index);
    void on_lineEditCarvSpeed_returnPressed();
    void on_lineEditCarvPiwer_returnPressed();
    void on_lineEditCarvTime_returnPressed();
    void on_lineEditCarvPixcel_returnPressed();
    void on_btnPointLaser_released();
    void on_btnPointLaser_pressed();
    void on_comboBoxPicType_currentIndexChanged(int index);

    void on_btnPower_clicked();

    void on_btnOK1_clicked();

    void on_btnOK2_clicked();

    void on_lineEditRepeat_editingFinished();

    void on_lineEditPower_editingFinished();

    void on_lineEditPixSpeed_editingFinished();

    void on_lineEditSpaceSpeed_cursorPositionChanged(int arg1, int arg2);

    void on_lineEditCarvSpeed_editingFinished();


    void on_lineEditCarvPiwer_editingFinished();

    void on_lineEditCarvTime_editingFinished();

    void on_lineEditCarvPixcel_editingFinished();

private:
    Ui::BC_ExpertForm *ui;
    bool pStop;
    bool pPowerOn;
signals:
    void Sig_Cutting();
    void Sig_Carve();
    void Sig_BD(QString);
    void Sig_Print();
    void Sig_Stop();
    void Sig_Cancle();
    void Sig_Preview();
    void Sig_LaserOn(QString);

};

#endif // BC_EXPERTFORM_H
