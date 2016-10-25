#include "qmenubutoon.h"

QMenuButoon::QMenuButoon(QWidget *parent) :
    QPushButton(parent)
{
    menu = new QMenu(this);
    connect(this,SIGNAL(clicked()),this,SLOT(popUpMenu()));
}

QMenu* QMenuButoon::getMenu()
{
    return menu;
}

void QMenuButoon::popUpMenu()
{   QPoint pos; //获取按键菜单的坐标

   // int x = pos.x();

    int y = pos.y();
   // pos.setX(x + this->geometry().width()/2);
    pos.setY(y+this->geometry().height());
    //返回菜单下面的action不管有没被选中，可以算一个小事件循环
    //里面参数的意思是在哪个坐标弹出菜单，这里将自定义的pop按键的坐标作参考，并在其下面弹出菜单
    menu->exec(this->mapToGlobal(pos));

    //exec(QCursor::pos());  当前光标的坐标
}
