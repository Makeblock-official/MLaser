#ifndef QMENUBUTOON_H
#define QMENUBUTOON_H

#include <QPushButton>
#include <QMenu>

class QMenuButoon : public QPushButton
{
    Q_OBJECT
public:
    explicit QMenuButoon(QWidget *parent = 0);
    QMenu * getMenu();
public slots:

private:
    QMenu * menu;
private slots:
    void popUpMenu();
signals:

};

#endif // QMENUBUTOON_H
