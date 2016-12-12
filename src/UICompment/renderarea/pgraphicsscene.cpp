#include "pgraphicsscene.h"

PGraphicsScene::PGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    drawAxis();
    tag = new QGraphicsTextItem();
    m_cursor=new QCursor;
    tag->setVisible(false);
    this->addItem(tag);
    connect(this,SIGNAL(Sig_ReloadImage(QString)),this,SLOT(reloadImage(QString)));

}
void PGraphicsScene::setHW(QPointF m)
{
    if(m_svgItems.size()>0)
    {
        m_svgItems.at(0)->ResetRect(QRectF(0,0,m.x(),m.y()));

    }
    else if(m_picItems.size()>0)
    {
        m_picItems.at(0)->ResetRect(QRectF(0,0,m.x(),m.y()));
    }
}
void PGraphicsScene::roateItem(qreal angle)
{
    if(m_svgItems.size()>0)
    {
        QPointF c = m_svgItems.at(0)->getRect().center();
        m_svgItems.at(0)->setTransformOriginPoint(QPoint(c.x(),c.y()));
        m_svgItems.at(0)->setRotation(angle);

    }
    else if(m_picItems.size()>0)
    {
        QPointF c = m_picItems.at(0)->getRect().center();
        m_picItems.at(0)->setTransformOriginPoint(QPoint(c.x(),c.y()));
        m_picItems.at(0)->setRotation(angle);
        m_picItems.at(0)->update();
    }
}
void PGraphicsScene::removeAllItems()
{
    QList<QGraphicsItem*> itemsList = this->items();
    QList<QGraphicsItem*>::iterator iter = itemsList.begin();
    QList<QGraphicsItem*>::iterator end = itemsList.end();
    while(iter != end)
    {
        QGraphicsItem* item = (*iter);
        this->removeItem(item); iter++;
        //        delete item;
    }
}
//void PGraphicsScene::saveFile(QString file)
//{
//    QFile f(file);
//    if(!f.open(QIODevice::ReadWrite))
//        return ;
//    QTextStream out(&f);
////    for()
////    {

////    }
//    f.close();
//}

void PGraphicsScene::drawAxis()
{
    QString mpath = QCoreApplication::applicationDirPath();
    QString mname = "/mLaser.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* apsetting = new QSettings(mallPath,QSettings::IniFormat);
    apsetting->beginGroup("mode");
    QString uint_value = apsetting->value("unit").toString();
    float unit = 1;
    if(uint_value == "inch")
    {
        QGraphicsTextItem *po = new QGraphicsTextItem("-(in)");
        po->setPos(5,5);
        this->addItem(po);
        unit = 2.5;
        qDebug()<<"drawAxis inch";
    }
    else if(uint_value == "mm")
    {
        QGraphicsTextItem *po = new QGraphicsTextItem("-(mm)");
        po->setPos(5,5);
        this->addItem(po);
        qDebug()<<"drawAxis mm";
        unit = 1.0;
    }
    apsetting->endGroup();

    int home = 30;
    int seg_num = 0;
	int seg_pos = 0;


    this->addLine(home,home,4000,home);//横线
    for(int i=home;i<4000;i++)
    {
        seg_pos = round(10 * seg_num * unit);
        if((i-home) == (seg_pos))
        {
            if((seg_num % 5) == 0)
            {
                this->addLine(i,home,i,home-10);
                QGraphicsTextItem *po = new QGraphicsTextItem(QString::number(seg_num));
                po->setPos(i-10,-5);
                this->addItem(po);
            }
            else
            {
                this->addLine(i,home,i,home-5);
            }
            seg_num++;
        }

    }

    seg_num = 0;
	seg_pos = 0;
    this->addLine(home,home,home,4000);
    for(int i=home;i<4000;i++)
    {
        seg_pos = round(10 * seg_num * unit);
        if((i-home) == seg_pos)
        {
            if((seg_num % 5) == 0)
            {
                this->addLine(home,i,home-10,i);
                QGraphicsTextItem *po = new QGraphicsTextItem(QString::number(seg_num));
                po->setPos(-2,i-12);
                this->addItem(po);
            }
            else
            {
                this->addLine(home,i,home-5,i);
            }
            seg_num++;
        }
    }

    QPen pen;
    QBrush brush(Qt::red);
    this->addEllipse(25,25,10,10,pen,brush);
}
void PGraphicsScene::drawBackground(QPainter *painter, const QRectF &)
{

    if (painter->paintEngine()->type() != QPaintEngine::OpenGL2) {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }

}
void PGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}
void PGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void PGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void PGraphicsScene::resetTable(int w,int h)
{
    m_printTableItem->setRect(QRectF(0,0,w*pDpm,h*pDpm));
}


QPointF PGraphicsScene::getPixmapPos()
{
    //    return m_picItem->scenePos();
}

qreal PGraphicsScene::getDPM()
{
    return pDpm;
}

void PGraphicsScene::setPixmapPos(QPointF m)
{
    //    m_picItem->setPos(m);
}

void PGraphicsScene::loadImage(QString pp,bool bFont)
{
    reloadFile = pp;
    if(QFileInfo(pp).suffix()=="svg")
    {
        renderer = new QSvgRenderer(pp);
        m_svgfilenames.append(pp);
        mSvgItem* _svgItem = new mSvgItem();
        _svgItem->setSharedRenderer(renderer);
        m_svgItems.append(_svgItem);
        m_svgItem = _svgItem;
        m_svgItem->setFile(pp);
        connect(m_svgItem,SIGNAL(Sig_ShowPos(QRectF)),this,SLOT(slotShowPos(QRectF)));
        QObject::connect(m_svgItem,SIGNAL(Sig_PosTag(QPointF,QString)),this,SLOT(slotPosTag(QPointF,QString)));
        connect(m_svgItem,SIGNAL(Sig_HidePosTag()),this,SLOT(slotHidePosTag()));
        connect(m_svgItem,SIGNAL(Sig_PosRect(QRectF,QString)),this,SLOT(slotReloadPic(QRectF,QString)));
        addItem(_svgItem);
        _svgItem->setRect(currentQRectF);
        //        _svgItem->setPos(30,30);
    }
    else
    {
        m_picfilenames.append(pp);
        QPixmap free(pp);
        PixmapItem* _pixmapItem = new PixmapItem();
        m_picItems.append(_pixmapItem);
        m_picItem = _pixmapItem;
        m_picItem->setFile(pp);
        //如果是文字，设置一个文字标志
        if(bFont)
        {
            m_picItem->setFont(true);
        }
        else
        {
            connect(_pixmapItem,SIGNAL(Sig_ShowPos(QRectF)),this,SLOT(slotShowPos(QRectF)));
        }

        _pixmapItem->setRect(currentQRectF);
        //动态显示坐标tag
        _pixmapItem->setPixmap(free);
        addItem(_pixmapItem);
        QObject::connect(_pixmapItem,SIGNAL(Sig_PosTag(QPointF,QString)),this,SLOT(slotPosTag(QPointF,QString)));
        QObject::connect(_pixmapItem,SIGNAL(Sig_HidePosTag()),this,SLOT(slotHidePosTag()));
        QObject::connect(_pixmapItem,SIGNAL(Sig_PosRect(QRectF,QString)),this,SLOT(slotReloadPic(QRectF,QString)));
        _pixmapItem->setVisible(true);
        _pixmapItem->setPos(30,30);
    }
}
void PGraphicsScene::slotShowPos(QRectF m)
{
    emit Sig_ShowPos(m);
}

void PGraphicsScene::reloadImage(QString pp)
{
    reloadFile = pp;
    if(QFileInfo(pp).suffix()=="svg")
    {
        renderer = new QSvgRenderer(pp);
        mSvgItem* _svgItem = new mSvgItem();
        _svgItem->setSharedRenderer(renderer);
        m_svgItems.append(_svgItem);
        m_svgItem = _svgItem;
        m_svgItem->setFile(pp);
        QObject::connect(m_svgItem,SIGNAL(Sig_PosTag(QPointF,QString)),this,SLOT(slotPosTag(QPointF,QString)));
        connect(m_svgItem,SIGNAL(Sig_HidePosTag()),this,SLOT(slotHidePosTag()));
        connect(m_svgItem,SIGNAL(Sig_PosRect(QRectF,QString)),this,SLOT(slotReloadPic(QRectF,QString)));
        addItem(_svgItem);
        _svgItem->setRect(currentQRectF);
    }
    else
    {
        QPixmap free(pp);
        PixmapItem* _pixmapItem = new PixmapItem();
        m_picItems.append(_pixmapItem);
        m_picItem = _pixmapItem;
        m_picItem->setFile(pp);
        _pixmapItem->setRect(currentQRectF);
        //动态显示坐标tag
        _pixmapItem->setPixmap(free);
        addItem(_pixmapItem);
        QObject::connect(_pixmapItem,SIGNAL(Sig_PosTag(QPointF,QString)),this,SLOT(slotPosTag(QPointF,QString)));
        QObject::connect(_pixmapItem,SIGNAL(Sig_HidePosTag()),this,SLOT(slotHidePosTag()));
        QObject::connect(_pixmapItem,SIGNAL(Sig_PosRect(QRectF,QString)),this,SLOT(slotReloadPic(QRect,QString)));
        _pixmapItem->setVisible(true);
        _pixmapItem->setPos(30,30);
    }
    this->update();
}



void PGraphicsScene::slotPosTag(QPointF p,QString m)
{
    tag->setPos(p.x(),p.y());
    tag->setPlainText(m);
    //    tag->setZValue(3);
    tag->setVisible(true);
    this->update();

}
void PGraphicsScene::slotHidePosTag()
{
    tag->setVisible(false);
}
void PGraphicsScene::setRect(QRectF f)
{
    currentQRectF = f;
}
//鼠标拖拽了模型之后，重新载入图片
void PGraphicsScene::slotReloadPic(QRectF f,QString file)
{
    currentQRectF = f;
    removeAllItems();
    this->clear();
    drawAxis();
    qDebug()<<"Reload file from :"<<file;
    emit Sig_ReloadImage(file);
    emit Sig_UReloadImage(f,file);
}

