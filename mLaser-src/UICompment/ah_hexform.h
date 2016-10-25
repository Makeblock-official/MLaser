#ifndef AH_HEXFORM_H
#define AH_HEXFORM_H

#include <QWidget>
#include "qcptitle.h"
#include "hexloadform.h"

namespace Ui {
class AH_HexForm;
}

class AH_HexForm : public QWidget
{
    Q_OBJECT

public:
    explicit AH_HexForm(QWidget *parent = 0);
    ~AH_HexForm();
    void languageUpdate();
private slots:
    void on_btnHex_clicked();

    void on_btnNext_clicked();

private:
    Ui::AH_HexForm *ui;
    HexLoadForm* hex;
signals:
    void Sig_AHNextStep();
};

#endif // AH_HEXFORM_H
