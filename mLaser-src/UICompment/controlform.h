#ifndef CONTROLFORM_H
#define CONTROLFORM_H

#include <QWidget>
#include "qcptitle.h"

namespace Ui {
class ControlForm;
}

class ControlForm : public QWidget
{
    Q_OBJECT

public:
    explicit ControlForm(QWidget *parent = 0);
    ~ControlForm();
    void languageUpdate();
protected:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_btnXp_clicked();
    void on_btnXa_clicked();
    void on_btnYa_clicked();
    void on_btnYp_clicked();
    void on_btnHome_clicked();

private:
    Ui::ControlForm *ui;
signals:
    void Sig_ToLaser(QString);
};

#endif // CONTROLFORM_H
