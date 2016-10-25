#ifndef BB_EASYNFORM_H
#define BB_EASYNFORM_H

#include <QWidget>

namespace Ui {
class BB_EasynForm;
}

class BB_EasynForm : public QWidget
{
    Q_OBJECT

public:
    explicit BB_EasynForm(QWidget *parent = 0);
    ~BB_EasynForm();
    void languageUpdate();
    void setElementEnable(bool);
    void setButtonIcon(bool bp);
private:
    Ui::BB_EasynForm *ui;
    bool pStop ;
signals:
    void Sig_BB(QString);
    void Sig_Cutting();
    void Sig_Carve();
    void Sig_Print();
    void Sig_Stop();
    void Sig_Cancle();

private slots:
    void on_btnPre_clicked();
    void on_btnSlice_clicked();
    void on_btnDraw_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnPannel_clicked();
};

#endif // BB_EASYNFORM_H
