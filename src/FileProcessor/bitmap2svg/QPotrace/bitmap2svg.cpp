#include <QDebug>
#include "bitmap2svg.h"
#include "QCoreApplication"

Bitmap2Svg::Bitmap2Svg(QWidget *parent) :
    QWidget(parent)
{
}


void Bitmap2Svg::SetBintmap(QString path)
{

    QImage tempImg(path);
    QString s_path = QCoreApplication::applicationDirPath();
    QRgb *pixels = (QRgb *)tempImg.scanLine(0);
    int w = tempImg.width();
    int h = tempImg.height();
    qDebug()<<tr("format = %1").arg(tempImg.format()) <<"w:" << w <<"h:"<<h;
    for(int y =0;y<tempImg.height();y++)
    {
        for(int x=0;x<tempImg.width();x++)
        {
            //gray scale
            if((x<3) && (y<3))
                pixels[x+y*(tempImg.width())] = qRgb(0,0,0);
            if((x>w-4)&&(y>h-4))
                pixels[x+y*(tempImg.width())] = qRgb(0,0,0);

        }
    }
    img = tempImg;
    img.save(s_path + "/hg.jpg");
    if(!potrace.trace(img)|| potrace.polygons().isEmpty())
        return ;
}

void Bitmap2Svg::OutPutSvg(QString str)
{
    QString newPath = str;

    QSvgGenerator generator;
    generator.setFileName(newPath);
    generator.setSize(QSize(200, 200));
    generator.setViewBox(QRect(0, 0, 200, 200));
    generator.setTitle(tr("MakeBlock Svg Generate"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Provide by makeblock."));
    QPainter painter;
    painter.begin(&generator);
    PaintSvg(painter);
    painter.end();
}


void Bitmap2Svg::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    PaintSvg(painter);
    painter.end();
}

void Bitmap2Svg::PaintSvg(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, 200, 200));
    painter.setPen(Qt::NoPen);

    QPainterPath *po = new QPainterPath();
    float w = 10000;
    float h = 10000;

    foreach(const QxPotrace::Polygon &polygon, potrace.polygons())
    {
        po->addPolygon(polygon.boundary);
        foreach(const QPolygonF &hole, polygon.holes)
        {
            po->addPolygon(hole);
        }
    }
    qDebug()<<tr("wi = %1,he = %2").arg(w).arg(h);
    painter.drawPath(*po);
}
