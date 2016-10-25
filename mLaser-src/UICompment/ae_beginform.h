#ifndef AE_BEGINFORM_H
#define AE_BEGINFORM_H

#include <QWidget>
#include <QMovie>
#include "qcptitle.h"
namespace Ui {
class AE_BeginForm;
}

class AE_BeginForm : public QWidget
{
    Q_OBJECT

public:
    explicit AE_BeginForm(QWidget *parent = 0);
    ~AE_BeginForm();
    void languageUpdate();

private:
    Ui::AE_BeginForm *ui;
    QMovie *movie;
signals:
    void Sig_AE(QString);
    void Sig_PrintTestFile(QStringList);
private slots:
    void on_btnPre_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // AE_BEGINFORM_H
