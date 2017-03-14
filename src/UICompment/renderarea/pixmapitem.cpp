#include <QDebug>
#include "pixmapitem.h"
#include "QSettings"
#include "QCoreApplication"

PixmapItem::PixmapItem(QGraphicsItem *parent)
    :QGraphicsPixmapItem(parent)
{
    setZValue(0);
    setAcceptDrops(true);
    setFlags(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
    cScale=false;
    bFont = false;
    m_cursor=new QCursor;
    direction=0;
    m_scale = 1.0;
    rescale = 1.0;
    QString mpath = QCoreApplication::applicationDirPath();
    QString mname = "/Config.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings *config = new QSettings(mallPath,QSettings::IniFormat);
    config->beginGroup("laser");
    config->setValue("scale",1);
    config->endGroup();
    this->setFlags(QGraphicsItem::ItemIsMovable |  QGraphicsItem::ItemIsSelectable );
}

void PixmapItem::ResetRect(QRectF rect)
{
//    qreal m_scale = 1.0*rect.width()/rect.height();
    m_rect=QRectF(rect.x(),rect.y(),rect.width(),(1.0*rect.width())/m_scale);
    update(boundingRect());
}

void PixmapItem::slotLoadImageType(QString imageType)
{
    if(imageType=="svg")
    {
        flagImageType = "svg";
    }
    else if(imageType=="mbl")
    {
        flagImageType = "mbl";
    }
    else if(imageType=="24bit")
    {
        if(bFont==0)
        {
            flagImageType = "24bit";
        }
        else if(bFont==1)
        {
            flagImageType = "font1";
        }
    }
}

QRectF PixmapItem::boundingRect()const
{
    return QRectF(m_rect.x(),m_rect.y(),m_rect.width()+20,m_rect.height()+20);
}

QPainterPath PixmapItem::shape()const{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPixmap a = this->pixmap();
    painter->drawPixmap(m_rect.x(),m_rect.y(),a);
    QPen pen(QColor(0,170,231),2,Qt::DashDotDotLine);
    painter->setPen(pen);
    painter->drawRect(m_rect);
    painter->drawRect(QRectF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height(),5,5));

}

void PixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    QPointF pos1=event->scenePos();
    //    QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
    //    QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
    //    QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
    QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
    //    qDebug()<<tr("rbx=%1,rby=%2").arg(rb.x()).arg(rb.y());
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
    QPixmap nf = this->pixmap().scaled(m_rect.width(),m_rect.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    this->setPixmap(nf);
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void PixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos1=event->scenePos();
//    QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
//    QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
//    QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
    QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
    rescale = m_rect.width();
    qDebug()<<"rescale"<<rescale;
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
    //    QPixmap nf = this->pixmap().scaled(m_rect.width(),m_rect.height(),Qt::IgnoreAspectRatio,Qt::FastTransformation);
    //    this->setPixmap(nf);
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void PixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    cScale=false;
    this->setCursor(*m_cursor);
    emit Sig_HidePosTag();
    QGraphicsItem::hoverLeaveEvent(event);
}

void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton)
    {
        start=event->scenePos();
    }
    QGraphicsItem::mousePressEvent(event);
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    int t=0;
    t++;
    t--;
    end=event->scenePos();
    if((end.x()>m_rect.x())&&(end.y()>m_rect.y()))
    if(cScale)
    {
        QPointF dis;
        dis=end-start;
        start=end;
        switch(direction)
        {

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

        }

    }
    else
    {
        update();
        QGraphicsItem::mouseMoveEvent(event);
    }

}

void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(cScale)
    {
        cScale=false;
        qDebug()<<"mouse release checked!";       

        QPixmap srcImage;
        QString imagepath = QCoreApplication::applicationDirPath();

        if(flagImageType=="mbl")
        {
            srcImage.load(imagepath + "/ds/pic0.jpg");
        }
        else if(flagImageType=="24bit")
        {
            srcImage.load(imagepath + "/24bit.jpg");
        }
        else if(flagImageType=="font1")
        {
            srcImage.load(imagepath + "/font1.jpg");
        }
        else
        {
            srcImage = this->pixmap();
            qDebug() << "function run!";
        }

        QPixmap nf = srcImage.scaled(m_rect.width(),m_rect.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        this->setPixmap(nf);
        QString mpath = QCoreApplication::applicationDirPath();
        QString mname = "/Config.ini";
        QString mallPath = QString("%1%2").arg(mpath).arg(mname);
        QSettings *config = new QSettings(mallPath,QSettings::IniFormat);
        config->beginGroup("laser");
        qDebug()<<"now"<<m_rect.width();
        float _mscale = m_rect.width()/rescale;
        config->setValue("scale",_mscale);
        config->endGroup();
        emit Sig_HidePosTag();        
//        emit Sig_PosRect(m_rect,m_file);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void PixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit Sig_ShowPos(m_rect);
    QGraphicsItem::mouseDoubleClickEvent(event);
}
