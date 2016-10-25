#ifndef BITMAP2SVG_H
#define BITMAP2SVG_H

#include <QWidget>
#include <QImage>
#include <QxPotrace>
#include <QSvgGenerator>
#include <QPainter>



class Bitmap2Svg : public QWidget
{
    Q_OBJECT
public:
    explicit Bitmap2Svg(QWidget *parent = 0);
    void SetBintmap(QString path);
    void OutPutSvg(QString);

public slots:

protected:
      void paintEvent(QPaintEvent *event);

private:
    QImage img;

    //function
    void PaintSvg(QPainter &painter);
    QxPotrace potrace;
signals:

};

#endif // BITMAP2SVG_H
