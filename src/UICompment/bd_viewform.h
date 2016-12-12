#ifndef BD_VIEWFORM_H
#define BD_VIEWFORM_H

#include <QWidget>
#include <QMenu>
#include <QContextMenuEvent>
#include <QGraphicsView>
#include <QMovie>
#include <QThread>


#include "fileprocessthread.h"

#include "renderarea/pgraphicsscene.h"

#define SVGTYPE 0
#define BITMAPTYPE 0x1

namespace Ui {
class BD_ViewForm;
}

class BD_ViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit BD_ViewForm(QWidget *parent = 0);
    ~BD_ViewForm();
    void languageUpdate();
    void resetAll();
    void getCuttingFile();
    void getCarveFile();
    void setShow(bool,unsigned int);
    void setPrintKind(int k);
    int getPrintKind(){return kPrint;}
    QRectF getBounding();
    void showProcess(bool bf);
    void redrawPic();

public slots:
    void slotOpen(QString);
    void slotRect(QRect);
    void slotMAddFont(QString);
    void slotRoat90();
    void slotRoat180();
    void slotRoat270();
    void slotSaveAs(QString);   //保存工程文件
    void slotReloadFile(QRectF,QString);
    void slotCombinePrint(QStringList);
    void slotSetHW(QPointF m);  //设置图片的长宽
    void slotShowPos(QRectF);
    void uintUpdate();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    Ui::BD_ViewForm *ui;
    QMenu * roatedAMenu;
    QMenu * popMenu;
    QAction * deletAction;
    QAction * paAction;
    QAction * pbAction;
    QAction * pcAction;
    QAction * posAction;
    QAction * pDrag;
    QAction * pSelect;

    QPointF fPos;
    QRectF fScope;
    QString current_file;

    int fFileTag;       //0  --> svg   1--> bmp
    int kPrint;         //0  -->轮廓雕刻  1-->灰度雕刻

    double scale;

    PGraphicsScene* pScene;

    QThread * fileThread;
    FileProcessThread* fileProcess;
    QMovie* movie;
    QTimer *t_fps;
private slots:
    void slotSelect();
    void slotDrag();
    void on_btnReset_clicked();

    void on_btnZoomOut_clicked();

    void on_btnZoomIn_clicked();

signals:
    void Sig_CombinePrint(QStringList);
    void Sig_Carv(PGraphicsScene*);
    void Sig_Cut(PGraphicsScene*);
    void Sig_ShowPos(QRectF);

};

#endif // BD_VIEWFORM_H
