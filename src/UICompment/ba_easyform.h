#ifndef BA_EASYFORM_H
#define BA_EASYFORM_H

#include <QWidget>
#include <QMenu>

#include "qmenubutoon.h"

namespace Ui {
class BA_EasyForm;
}

class BA_EasyForm : public QWidget
{
    Q_OBJECT

public:
    explicit BA_EasyForm(QWidget *parent = 0);
    ~BA_EasyForm();
    void languageUpdate();

private slots:
    void slotMa();
    void slotMb();
    void slotMc();
    void slotMd();

    void on_btnBackHome_clicked();

    void on_btnNext_clicked();

    void on_btnPannel_clicked();

    void on_btnAddText_clicked();

    void on_btnSetHome_clicked();

    void on_btnMateril_clicked();

private:
    Ui::BA_EasyForm *ui;
//    QMenu* menu;
signals:
    void Sig_BA(QString);
    void Sig_Preview();
};

#endif // BA_EASYFORM_H
