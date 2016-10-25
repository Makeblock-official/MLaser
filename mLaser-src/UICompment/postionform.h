#ifndef POSTIONFORM_H
#define POSTIONFORM_H

#include <QWidget>
#include "titlebar.h"

namespace Ui {
class PostionForm;
}

class PostionForm : public QWidget
{
    Q_OBJECT

public:
    explicit PostionForm(QWidget *parent = 0);
    ~PostionForm();
    void languageUpdate();
    void setUIElement(QRectF);

private slots:
    void on_btnCertern_clicked();
    void on_lineEditH_editingFinished();
    void on_lineEditW_returnPressed();

    void on_lineEditW_editingFinished();

private:
    Ui::PostionForm *ui;
//    double scale;
    double old_w;
    double old_h;

signals:
    void Sig_WH(QPointF);
};

#endif // POSTIONFORM_H
