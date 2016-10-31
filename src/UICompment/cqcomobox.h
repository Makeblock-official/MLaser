#ifndef CQCOMOBOX_H
#define CQCOMOBOX_H

#include <QComboBox>

class CQComoBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CQComoBox(QWidget *parent = 0);
    virtual void mousePressEvent(QMouseEvent *e);
signals:
    void Sig_Popup();
public slots:

};

#endif // CQCOMOBOX_H
