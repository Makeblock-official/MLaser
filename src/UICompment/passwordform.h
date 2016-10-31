#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QWidget>
#include "qcptitle.h"
namespace Ui {
class PasswordForm;
}

class PasswordForm : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordForm(QWidget *parent = 0);
    ~PasswordForm();
    void languageUpdate();
private slots:
    void on_btnChange_clicked();

private:
    Ui::PasswordForm *ui;
signals:
    void Sig_LoginShow();
};

#endif // PASSWORDFORM_H
