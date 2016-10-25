#include <QGLWidget>
#include <QActionGroup>
#include <QFileInfo>
#include <QDebug>
#include "bd_viewform.h"
#include "ui_bd_viewform.h"

BD_ViewForm::BD_ViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BD_ViewForm)
{
    ui->setupUi(this);
    fFileTag = -1;
    kPrint = 0;
    scale = 1.0;
    //右键弹出菜单
    popMenu = new QMenu();
    roatedAMenu = new QMenu(popMenu);
    roatedAMenu->setTitle(tr("旋转"));
    deletAction = new QAction(tr("删除"),this);
    posAction = new QAction(tr("位置"),this);
    pDrag = new QAction(tr("拖拽"),this);
    pSelect = new QAction(tr("选择"),this);;
    QActionGroup * pGroup = new QActionGroup(this);
    pGroup->addAction(pDrag);
    pGroup->addAction(pSelect);
    pDrag->setCheckable(true);
    pSelect->setCheckable(true);
    pSelect->setChecked(true);
    paAction = new QAction(tr("90"),this);
    pbAction = new QAction(tr("180"),this);
    pcAction = new QAction(tr("270"),this);

    fileThread = new QThread();
    fileProcess = new FileProcessThread();
    fileProcess->moveToThread(fileThread);
    fileThread->start();
    connect(pDrag,SIGNAL(triggered()),this,SLOT(slotDrag()));

    connect(pSelect,SIGNAL(triggered()),this,SLOT(slotSelect()));
    connect(paAction,SIGNAL(triggered()),this,SLOT(slotRoat90()));
    connect(pbAction,SIGNAL(triggered()),this,SLOT(slotRoat180()));
    connect(pcAction,SIGNAL(triggered()),this,SLOT(slotRoat270()));

    connect(this,SIGNAL(Sig_Carv(PGraphicsScene*)),fileProcess,SLOT(slotCarveFile(PGraphicsScene*)));
    connect(this,SIGNAL(Sig_Cut(PGraphicsScene*)),fileProcess,SLOT(slotCuttingFile(PGraphicsScene*)));
    connect(fileProcess,SIGNAL(Sig_CombinePrint(QStringList)),this,SLOT(slotCombinePrint(QStringList)));
    //定时刷新界面
    t_fps = new QTimer();
    connect(t_fps,SIGNAL(timeout()),this,SLOT(redrawPic()));
    t_fps->start(1000);

    //创建GraphicesScene
    pScene = new PGraphicsScene();
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    ui->graphicsView->setScene(pScene);

    ui->frameStatus->hide();
    connect(pScene,SIGNAL(Sig_UReloadImage(QRectF,QString)),this,SLOT(slotReloadFile(QRectF,QString)));
    connect(pScene,SIGNAL(Sig_ShowPos(QRectF)),this,SLOT(slotShowPos(QRectF)));
    movie = new QMovie(":/Source/gif/wait2.gif");
    ui->labelMovie->setMovie(movie);
    movie->start();
    ui->labelMovie->hide();
    ui->labelStatus->hide();
    this->setStyleSheet(QString("QMenu {"
                                "font: 10pt \"微软雅黑\";"
                                "color: rgb(255, 255, 255);"
                                "background-color: rgb(97, 96, 101);}"
                                "QMenu::item {"
                                "padding:20px;"
                                "height: 32px;"
                                "width:230px;"
                                "background-color: transparent;}"
                                "QMenu::item:selected { "
                                "background-color: rgb(117, 116, 121);"
                                "}"));
    //movie->start();
    //设置graphicesScene的背景颜色
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));

}

BD_ViewForm::~BD_ViewForm()
{
    delete ui;
}
//右键回调函数
void BD_ViewForm::contextMenuEvent(QContextMenuEvent *event)
{
    popMenu->clear();
    roatedAMenu->addAction(paAction);
    roatedAMenu->addAction(pbAction);
    roatedAMenu->addAction(pcAction);
    //    popMenu->addAction(deletAction);
    popMenu->addMenu(roatedAMenu);
    //    popMenu->addAction(posAction);
    popMenu->addAction(pDrag);
    popMenu->addAction(pSelect);
    popMenu->exec(QCursor::pos());
    event->accept();
}
void BD_ViewForm::redrawPic()
{
    pScene->update();
    ui->graphicsView->update();
}
void BD_ViewForm::resetAll()
{
    fFileTag = -1;
    pScene->removeAllItems();
    //    for(int i=0;i<pScene->m_picItems.size();i++)
    //    {
    //        pScene->removeItem(static_cast<QGraphicsItem*>(pScene->m_picItems.at(i)));
    //    }
    //    for(int j = 0;j<pScene->m_svgItems.size();j++)
    //    {
    //        pScene->removeItem(static_cast<QGraphicsItem*>(pScene->m_svgItems.at(j)));
    //    }
    pScene->m_picItems.clear();
    pScene->m_svgItems.clear();
    pScene->m_svgfilenames.clear();
    pScene->m_picfilenames.clear();
    pScene->clear();
    ui->graphicsView->update();
    pScene->drawAxis();
    pScene->update();
    this->repaint();
}
void BD_ViewForm::slotSetHW(QPointF m)
{
    pScene->setHW(m);
}
//设置是使用轮廓打印还是使用灰度打印
void BD_ViewForm::setPrintKind(int k)
{
    kPrint = k;
}
//轮廓模式打印
void BD_ViewForm::getCuttingFile()
{
    emit Sig_Cut(pScene);
    ui->labelMovie->show();
}
QRectF BD_ViewForm::getBounding()
{
    QPointF topleft(9999,9999);
    QPointF bottmright(0,0);
    for(int i=0;i<pScene->m_svgItems.size();i++)
    {
        QRectF m = pScene->m_svgItems.at(i)->getRect();
        QPointF f = pScene->m_svgItems.at(i)->pos();
        QPointF r_pos(m.x(),m.y());
        f = r_pos + f -QPointF(30,30);
        QPointF nf(f.x()+m.width(),f.y()+m.height());
        if(topleft.x()>f.x())
            topleft.setX(f.x());
        if(topleft.y()>f.y())
            topleft.setY(f.y());
        if(bottmright.x()<nf.x())
            bottmright.setX(nf.x());
        if(bottmright.y()<nf.y())
            bottmright.setY(nf.y());

    }
    for(int j = 0;j<pScene->m_picItems.size();j++)
    {
        QPointF f = pScene->m_picItems.at(j)->pos()-QPointF(30,30);
        QRectF m = pScene->m_picItems.at(j)->getRect();
        QPointF r_pos(m.x(),m.y());
        f = f + r_pos;
        QPointF nf(f.x()+m.width(),f.y()+m.height());
        if(topleft.x()>f.x())
            topleft.setX(f.x());
        if(topleft.y()>f.y())
            topleft.setY(f.y());
        if(bottmright.x()<nf.x())
            bottmright.setX(nf.x());
        if(bottmright.y()<nf.y())
            bottmright.setY(nf.y());
    }
    return QRectF(topleft.x(),topleft.y(),(bottmright.x()-topleft.x()),(bottmright.y()-topleft.y()));
}
//灰度雕刻模式打印
void BD_ViewForm::getCarveFile()
{
    emit Sig_Carv(pScene);
    ui->labelMovie->show();

}
void BD_ViewForm::slotCombinePrint(QStringList filelist)
{
    ui->labelMovie->hide();
    emit Sig_CombinePrint(filelist);
}
//选择模式
void BD_ViewForm::slotSelect()
{
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
}
//拖拽模式
void BD_ViewForm::slotDrag()
{
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

void BD_ViewForm::slotRoat90()
{
    if(fFileTag==BITMAPTYPE)
    {
        //        QFile::remove("r_90.png");
        fPos = pScene->m_picItems.at(0)->scenePos();
        fScope = pScene->m_picItems.at(0)->getRect();
        fScope.setRect(fPos.x()-30,fPos.y()-30,fScope.height(),fScope.width());
        //根据旋转的角度，来变换图片
        QImage* imgRatate = new QImage();
        QMatrix matrix;
        matrix.rotate(90);
        QImage f(current_file);
        *imgRatate = f.transformed(matrix);
        imgRatate->save("r_90.png");

        //将变换之后的图片重新导入到场景中
        resetAll();
        fFileTag = -1;
        pScene->setRect(fScope);
        slotOpen("r_90.png");

    }

}
void BD_ViewForm::slotRoat180()
{
    if(fFileTag==BITMAPTYPE)
    {
        //        QFile::remove("r_90.png");
        fPos = pScene->m_picItems.at(0)->scenePos();
        fScope = pScene->m_picItems.at(0)->getRect();
        fScope.setRect(fPos.x()-30,fPos.y()-30,fScope.width(),fScope.height());
        //根据旋转的角度，来变换图片
        QImage* imgRatate = new QImage();
        QMatrix matrix;
        matrix.rotate(180);
        QImage f(current_file);
        *imgRatate = f.transformed(matrix);
        imgRatate->save("r_180.png");

        //将变换之后的图片重新导入到场景中
        resetAll();
        fFileTag = -1;
        pScene->setRect(fScope);
        slotOpen("r_180.png");
    }
}
void BD_ViewForm::slotRoat270()
{
    if(fFileTag==BITMAPTYPE)
    {
        //        QFile::remove("r_90.png");
        fPos = pScene->m_picItems.at(0)->scenePos();
        fScope = pScene->m_picItems.at(0)->getRect();
        fScope.setRect(fPos.x()-30,fPos.y()-30,fScope.height(),fScope.width());
        //根据旋转的角度，来变换图片
        QImage* imgRatate = new QImage();
        QMatrix matrix;
        matrix.rotate(90);
        QImage f(current_file);
        *imgRatate = f.transformed(matrix);
        imgRatate->save("r_270.png");

        //将变换之后的图片重新导入到场景中
        resetAll();
        fFileTag = -1;
        pScene->setRect(fScope);
        slotOpen("r_270.png");

    }
}
void BD_ViewForm::slotReloadFile(QRectF f,QString file)
{
    //    resetAll();
    //    fFileTag = -1;
    //    pScene->setRect(f);
    //    slotOpen(file);
    qDebug()<<"fff-------,you get the goode prise of it";
    pScene->update();
    ui->graphicsView->repaint();
    ui->graphicsView->update();
}
void BD_ViewForm::slotOpen(QString file)
{
    current_file = file;
    if(fFileTag==(-1))
    {
        if(QFileInfo(file).suffix()=="svg")
        {
            fFileTag = SVGTYPE;
        }
        else
        {
            fFileTag = BITMAPTYPE;
        }
        pScene->loadImage(file,false);
    }
}
void BD_ViewForm::slotShowPos(QRectF m)
{
    emit Sig_ShowPos(m);
}
void BD_ViewForm::slotSaveAs(QString file)
{
    //    pScene->m_picItems.clear();
    //    pScene->m_svgItems.clear();
    //    pScene->m_svgfilenames.clear();
    //    pScene->m_picfilenames.clear();
    //1.获取scene中的文件名
    for(int i=0;i<pScene->m_svgItems.size();i++)
    {
        QString filename = pScene->m_svgfilenames.at(i);
        QRectF m = pScene->m_svgItems.at(i)->getRect();
        QPointF f = pScene->m_svgItems.at(i)->pos();
        QPointF r_pos(m.x(),m.y());
        QString name = tr("./project/svg%1.svg").arg(i);
        QFile::copy(filename,name);
        QSettings* psetting = new QSettings("./project/config.ini",QSettings::IniFormat);
        psetting->beginGroup(tr("svg%1").arg(i));
        psetting->setValue("name",QFileInfo(name).fileName());
        psetting->setValue("bfont",0);
        psetting->setValue("x",f.x());
        psetting->setValue("y",f.y());
        psetting->setValue("width",m.width());
        psetting->setValue("height",m.height());
        psetting->endGroup();



    }
    for(int j = 0;j<pScene->m_picItems.size();j++)
    {
        QString filename = pScene->m_picfilenames.at(j);
        QPointF f = pScene->m_picItems.at(j)->pos();
        QRectF m = pScene->m_picItems.at(j)->getRect();
        QPointF r_pos(m.x(),m.y());
        bool bFont = pScene->m_picItems.at(j)->bFont;
        QString suffix = QFileInfo(filename).suffix();

        QString name = tr("./project/pic%1.%2").arg(j).arg(suffix);
        QFile::copy(filename,name);
        QSettings* psetting = new QSettings("./project/config.ini",QSettings::IniFormat);
        psetting->beginGroup(tr("pic%1").arg(j));
        psetting->setValue("name",QFileInfo(name).fileName());
        if(bFont)
            psetting->setValue("bfont",1);      //该图片为文字图片
        else
            psetting->setValue("bfont",0);      //为普通图片

        psetting->setValue("x",f.x());
        psetting->setValue("y",f.y());
        psetting->setValue("width",m.width());
        psetting->setValue("height",m.height());
        psetting->endGroup();

    }

}

void BD_ViewForm::slotRect(QRect f)
{
    pScene->setRect(f);
}
void BD_ViewForm::slotMAddFont(QString m)
{
    current_file = m;
    if((fFileTag==(-1))||(fFileTag==BITMAPTYPE))
    {

        pScene->loadImage(m,true);
        fFileTag = BITMAPTYPE;
    }
}
void BD_ViewForm::setShow(bool b,unsigned int a)
{
    if(b)
    {
        ui->frameStatus->show();
        ui->progressBar->setValue(a);
        qDebug()<<tr("current persent %1").arg(a);
    }
    else
    {
        ui->progressBar->setValue(a);
        ui->frameStatus->hide();
    }
}
void BD_ViewForm::showProcess(bool bf)
{
    if(bf)
    {
        ui->frameStatus->show();
        ui->progressBar->hide();
        movie->start();
        ui->labelProcess->show();
        ui->labelProcess->repaint();
    }
    else
    {
        ui->frameStatus->hide();
        ui->progressBar->hide();
        movie->stop();
        ui->labelProcess->hide();
    }
}

//Zoom恢复原来大小
void BD_ViewForm::on_btnReset_clicked()
{
    QMatrix matrix;
    scale = 1;
    matrix.scale(scale,scale);
    ui->graphicsView->setMatrix(matrix);
}
//放大
void BD_ViewForm::on_btnZoomOut_clicked()
{
    QMatrix matrix;
    scale += 0.1;
    matrix.scale(scale,scale);
    ui->graphicsView->setMatrix(matrix);
}
//缩小
void BD_ViewForm::on_btnZoomIn_clicked()
{
    QMatrix matrix;
    scale -= 0.1;
    if(scale<=0)
        scale = 0.1;
    matrix.scale(scale,scale);
    ui->graphicsView->setMatrix(matrix);
}
void BD_ViewForm::languageUpdate()
{
    ui->retranslateUi(this);
}
