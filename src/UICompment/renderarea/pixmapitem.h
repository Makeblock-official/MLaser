#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QPainterPath>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
class PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    PixmapItem(QGraphicsItem *parent = 0);

    void ResetRect(QRectF rect);

    void setRect(QRectF rect){m_rect = rect;m_scale = 1.0*rect.width()/rect.height();}
    void setFile(QString file){m_file = file;}
    void setFont(bool b){bFont = b;}
    QString getFile(){return m_file;}
    QRectF getRect(){return m_rect;}

    bool bFont;  //是否为字体图片

protected:
    QRectF boundingRect()const;
    //    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    //    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QRectF m_rect;
    QString m_file;
    bool cScale;

    QCursor* m_cursor;
    QPointF start;
    QPointF end;
    qreal m_scale;
    int direction;

signals:
    void Sig_PosTag(QPointF,QString);
    void Sig_PosRect(QRectF,QString);
    void Sig_HidePosTag();
    void Sig_ShowPos(QRectF);

};

#endif // PIXMAPITEM_H
