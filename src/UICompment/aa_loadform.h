#ifndef AA_LOADFORM_H
#define AA_LOADFORM_H

#include <QWidget>
#include "qcptitle.h"
namespace Ui {
class AA_LoadForm;
}

class AA_LoadForm : public QWidget
{
    Q_OBJECT

public:
    explicit AA_LoadForm(QWidget *parent = 0);
    ~AA_LoadForm();
    void languageUpdate();

private:
    Ui::AA_LoadForm *ui;
signals:
    void Sig_AA(QString);
private slots:
    void on_btnFirst_clicked();
    void on_btnLoad_clicked();
};

#endif // AA_LOADFORM_H
