#ifndef PGRAPHICSSCENE_H
#define PGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QtOpenGL>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSvgRenderer>
#include "pixmapitem.h"
#include "msvgitem.h"

class PGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PGraphicsScene(QObject *parent = 0);
    void resetTable(int w,int h);
    void setHW(QPointF m);
    QPointF getPixmapPos();
    qreal getDPM();

    QVector<PixmapItem*>m_picItems;
    QVector<mSvgItem*>m_svgItems;
    QStringList m_picfilenames;
    QStringList m_svgfilenames;

    void setPixmapPos(QPointF m);
    void loadImage(QString pp,bool b);


    void setRect(QRectF f);
    void drawAxis();
    void removeAllItems();
    void roateItem(qreal angle);
public slots:
    void slotPosTag(QPointF,QString);
    void slotHidePosTag();
    void slotReloadPic(QRectF,QString);
    void slotShowPos(QRectF);

    void reloadImage(QString pp);

    void slotLoadImageType(QString imageType);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    QGraphicsRectItem *m_printTableItem;
    QGraphicsRectItem* m_rectItem;
    QGraphicsEllipseItem* m_cycleItem;
    QGraphicsTextItem* tag;
    QCursor* m_cursor;

    QString reloadFile;
    PixmapItem *m_picItem;
    mSvgItem* m_svgItem;

    QRectF currentQRectF;
    QSvgRenderer *renderer;
    qreal pDpm;
signals:
    void Sig_ReloadImage(QString);
    void Sig_UReloadImage(QRectF,QString);
    void Sig_ShowPos(QRectF);

    void Sig_LoadImageType(QString);//用于处理缩放图片像素损失


};

#endif // PGRAPHICSSCENE_H
