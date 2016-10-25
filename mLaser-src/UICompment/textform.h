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

    //function
    void updateConfig();
signals:
    void Sig_AddFont(QString);
};

#endif // TEXTFORM_H
