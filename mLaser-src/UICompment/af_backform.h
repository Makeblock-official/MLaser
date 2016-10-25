#ifndef AF_BACKFORM_H
#define AF_BACKFORM_H

#include <QWidget>
#include "qcptitle.h"
namespace Ui {
class AF_BackForm;
}

class AF_BackForm : public QWidget
{
    Q_OBJECT

public:
    explicit AF_BackForm(QWidget *parent = 0);
    ~AF_BackForm();
    void languageUpdate();

private:
    Ui::AF_BackForm *ui;

signals:
    void Sig_AF(QString);
private slots:
    void on_pushButton_clicked();
};

#endif // AF_BACKFORM_H
