#ifndef GCODEFORM_H
#define GCODEFORM_H

#include <QWidget>
#include "titlebar.h"
namespace Ui {
class GcodeForm;
}

class GcodeForm : public QWidget
{
    Q_OBJECT

public:
    explicit GcodeForm(QWidget *parent = 0);
    ~GcodeForm();
    void languageUpdate();

private slots:
    void on_btnLoad_clicked();

    void on_btnClear_clicked();

    void on_btnInput_clicked();

private:
    Ui::GcodeForm *ui;

signals:
    void Sig_RCmds(QString);
};

#endif // GCODEFORM_H
