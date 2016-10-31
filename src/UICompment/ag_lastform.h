#ifndef AG_LASTFORM_H
#define AG_LASTFORM_H

#include <QWidget>
#include "qcptitle.h"

namespace Ui {
class AG_LastForm;
}

class AG_LastForm : public QWidget
{
    Q_OBJECT

public:
    explicit AG_LastForm(QWidget *parent = 0);
    ~AG_LastForm();
    void languageUpdate();

private slots:
    void on_btnPass_clicked();

    void on_btnFailed_clicked();

private:
    Ui::AG_LastForm *ui;

signals:
    void Sig_LastFailed();
    void Sig_LastSuccessful();
};

#endif // AG_LASTFORM_H
