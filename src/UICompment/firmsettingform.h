#ifndef FIRMSETTINGFORM_H
#define FIRMSETTINGFORM_H

#include <QWidget>
#include "titlebar.h"

namespace Ui {
class FirmSettingForm;
}

class FirmSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit FirmSettingForm(QWidget *parent = 0);
    ~FirmSettingForm();

private:
    Ui::FirmSettingForm *ui;
};

#endif // FIRMSETTINGFORM_H
