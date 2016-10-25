#ifndef BITMAP2GCODE_H
#define BITMAP2GCODE_H

#include <QWidget>

class Bitmap2Gcode : public QWidget
{
    Q_OBJECT
public:
    explicit Bitmap2Gcode(QWidget *parent = 0);
    void LoadImageW(QString path);
    void LoadImageW(QString path,int thredhold);
public slots:
    //function
    QImage Image2Halftone(QImage src);
    void ExportGrayPix(QImage src,QString savefile);
    QImage GrayScalePicture(QImage,int grayscale);
    void ExportGcode(QPointF,QPointF,QString savepath);

private:
    QImage img;
    QPointF pos;
    QPointF scope;
    int thredhold;
    bool preOn;


signals:

};

#endif // BITMAP2GCODE_H
