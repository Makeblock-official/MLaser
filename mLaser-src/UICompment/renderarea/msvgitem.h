#ifndef MSVGITEM_H
#define MSVGITEM_H

#include <QGraphicsSvgItem>
#include <QTimer>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QPainterPath>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

class mSvgItem :public QGraphicsSvgItem
{
    Q_OBJECT

public:
    explicit mSvgItem(QGraphicsItem* parent = 0);
    mSvgItem(const QString& fileName, QGraphicsItem* parent = 0);
    QRectF getRect(){return m_rect;}
    void setRect(QRectF rect){m_rect = rect;m_scale = 1.0*rect.width()/rect.height();}
    void setFile(QString file){m_file = file;}
    QString getFile(){return m_file;}
    void ResetRect(QRectF rect);
public slots:

protected:
    QRectF boundingRect()const;
    QPainterPath shape()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QRectF m_rect;
    QString m_file;
    bool cScale;
    QCursor* m_cursor;
    QPointF start;
    QPointF end;
    int direction;
    qreal m_scale;
    QSizeF size_m;

signals:
    void Sig_PosTag(QPointF,QString);
    void Sig_PosRect(QRectF,QString);
    void Sig_HidePosTag();
    void Sig_ShowPos(QRectF);
};

#endif // MSVGITEM_H
