#ifndef TEXTFORM_H
#define TEXTFORM_H

#include <QWidget>
#include "titlebar.h"

namespace Ui {
class TextForm;
}

class TextForm : public QWidget
{
    Q_OBJECT

public:
    explicit TextForm(QWidget *parent = 0);
    ~TextForm();
    void languageUpdate();

private slots:


    void on_btnOk_clicked();

    void on_fontComboBox_activated(const QString &arg1);

    void on_btnBlod_clicked();

    void on_comboBoxFontSize_activated(const QString &arg1);

    void on_btnCancle_clicked();

    void on_btnItly_clicked();

private:
    Ui::TextForm *ui;
    QFont fonts;
    bool blod;
    bool bItly;
    float font_size_tab[23] = {5,5.5,6.5,7.5,9,10.5,12,14,15,16,18,22,24,26,36,42,48,56,72,96,120,150,180};
    //function
    void updateConfig();
signals:
    void Sig_AddFont(QString);
};

#endif // TEXTFORM_H
