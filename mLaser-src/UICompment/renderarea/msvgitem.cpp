#include <QSvgRenderer>
#include "msvgitem.h"

mSvgItem::mSvgItem(QGraphicsItem* parent)
    : QGraphicsSvgItem(parent), size_m(-1.0, -1.0)
{
    m_rect=QRectF(0,0,400,400);

    setZValue(0);
    setAcceptDrops(true);
    setFlags(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
    cScale=false;
    m_cursor=new QCursor;
    direction=0;
    this->setFlag(QGraphicsItem::ItemIsMovable);
    //    this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

mSvgItem::mSvgItem(const QString& fileName, QGraphicsItem* parent)
    : QGraphicsSvgItem(fileName, parent), size_m(-1.0, -1.0)
{
    setZValue(0);
    setAcceptDrops(true);
    setFlags(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
    cScale=false;
    m_cursor=new QCursor;
    direction=0;
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

void mSvgItem::ResetRect(QRectF rect)
{
//    m_rect=rect;
    m_rect=QRectF(rect.x(),rect.y(),rect.width(),(1.0*rect.width())/m_scale);
    update(boundingRect());
}

QRectF mSvgItem::boundingRect()const
{
    return QRectF(m_rect.x()-2,m_rect.y()-2,m_rect.width()+4,m_rect.height()+4);
}

QPainterPath mSvgItem::shape()const{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

void mSvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!this->renderer()->isValid())
        return;


    if (elementId().isEmpty())
        this->renderer()->render(painter, boundingRect());
    else
    {
        QSvgRenderer *renderer = this->renderer();
        //        this->renderer()->render(painter, elementId(), boundingRect());
        renderer->render(painter, elementId(), boundingRect());
    }
    //    painter->drawEllipse(m_rect.x(),m_rect.y(),4,4);
    QPen pen(QColor(0,170,231),3,Qt::DashDotDotLine);
    painter->setPen(pen);
    painter->drawRect(m_rect);

}

void mSvgItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    QPointF pos1=event->scenePos();
    QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
    QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
    QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
    QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
    if((pos1.x()>(rb.x()-20)) && (pos1.x()<(rb.x()+20)) && (pos1.y()>(rb.y()-20)) && (pos1.y()<(rb.y()+20)))
    {
        m_cursor->setShape(Qt::SizeFDiagCursor);
        direction=4;
    }
    else
    {
        cScale=false;
        m_cursor->setShape(Qt::SizeAllCursor);
    }
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void mSvgItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos1=event->scenePos();
    //    QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
    //    QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
    //    QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
    QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
    if((pos1.x()>(rb.x()-20)) && (pos1.x()<(rb.x()+20)) && (pos1.y()>(rb.y()-20)) && (pos1.y()<(rb.y()+20)))
    {
        m_cursor->setShape(Qt::SizeFDiagCursor);
        direction=4;
        cScale=true;
        QString a = QObject::tr("W:%1 H:%2").arg(m_rect.width()/10.0).arg(m_rect.height()/10.0);
        emit Sig_PosTag(pos1,a);
    }
    else
    {
        cScale=false;
        m_cursor->setShape(Qt::SizeAllCursor);
    }
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void mSvgItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    cScale=false;
    this->setCursor(*m_cursor);
    update();
    emit Sig_HidePosTag();
    QGraphicsItem::hoverLeaveEvent(event);
}

void mSvgItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton)
    {
        start=event->scenePos();
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void mSvgItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    int t=0;
    t++;
    t--;
    end=event->scenePos();
    if(cScale)
    {
        QPointF dis;

        dis=end-start;
        start=end;
        switch(direction){
        //            case 1:
        //            {
        //                QRectF tem=QRectF(m_rect.x(),m_rect.y()+dis.y()/2,m_rect.width(),m_rect.height()-dis.y());
        //                this->ResetRect(tem);
        //                update(boundingRect());
        //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
        //                emit Sig_PosTag(end,a);
        //                this->moveBy(0,dis.y()/2);
        //                break;
        //            }
        //            case 2:
        //            {
        //                QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y()+dis.y()/2,
        //                                  m_rect.width()+dis.x(),m_rect.height()-dis.y());
        //                this->ResetRect(tem);

        //                update(boundingRect());
        //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
        //                emit Sig_PosTag(end,a);
        //                this->moveBy(-dis.x()/2,-dis.y()/2);
        //                break;
        //            }
        //            case 3:
        //            {
        //                QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y(),m_rect.width()+dis.x(),m_rect.height());
        //                this->ResetRect(tem);
        //                update(boundingRect());
        //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
        //                emit Sig_PosTag(end,a);
        //                this->moveBy(-dis.x()/2,0);
        //                break;
        //            }
        case 4:
        {
            QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y()+dis.y()/2,
                              m_rect.width()+dis.x(),m_rect.height()+dis.y());
            this->ResetRect(tem);
            update(boundingRect());
            QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
            emit Sig_PosTag(end,a);
            this->moveBy(-dis.x()/2,-dis.y()/2);
            break;
        }
            //            case 5:
            //            {
            //                QRectF tem=QRectF(m_rect.x(),m_rect.y()+dis.y()/2,m_rect.width(),m_rect.height()+dis.y());
            //                this->ResetRect(tem);
            //                update(boundingRect());
            //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
            //                emit Sig_PosTag(end,a);
            //                this->moveBy(0,-dis.y()/2);
            //                break;
            //            }
            //            case 6:
            //            {
            //                QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y()+dis.y()/2,
            //                                  m_rect.width()-dis.x(),m_rect.height()+dis.y());
            //                this->ResetRect(tem);
            //                update(boundingRect());
            //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
            //                emit Sig_PosTag(end,a);
            //                this->moveBy(-dis.x()/2,-dis.y()/2);
            //                break;
            //            }
            //            case 7:
            //            {
            //                QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y(),m_rect.width()-dis.x(),m_rect.height());
            //                this->ResetRect(tem);
            //                update(boundingRect());
            //                QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
            //                emit Sig_PosTag(end,a);
            //                this->moveBy(dis.x()/2,0);
            //                break;
            //            }
        case 8:
        {
            QRectF tem=QRectF(m_rect.x()+dis.x()/2,m_rect.y()+dis.y()/2,
                              m_rect.width()-dis.x(),m_rect.height()-dis.y());
            this->ResetRect(tem);
            update(boundingRect());
            QString a = QObject::tr("W:%1 H:%2").arg(tem.width()/10.0).arg(tem.height()/10.0);
            emit Sig_PosTag(end,a);
            this->moveBy(dis.x()/2,dis.y()/2);
            break;
        }
        }

    }
    else
    {
        update();
        QGraphicsItem::mouseMoveEvent(event);
    }

}

void mSvgItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(cScale)
    {
        cScale=false;
        emit Sig_HidePosTag();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void mSvgItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit Sig_ShowPos(m_rect);
    QGraphicsItem::mouseDoubleClickEvent(event);
}
