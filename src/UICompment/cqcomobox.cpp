#include "cqcomobox.h"

CQComoBox::CQComoBox(QWidget *parent) :
    QComboBox(parent)
{

}
void CQComoBox::mousePressEvent(QMouseEvent *e)
{
    emit Sig_Popup();
    this->showPopup();
}
