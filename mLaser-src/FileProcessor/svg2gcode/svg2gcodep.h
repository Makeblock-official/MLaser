#ifndef SVG2GCODEP_H
#define SVG2GCODEP_H

#include <QWidget>

#include "svg2gcode.h"

class Svg2GcodeP : public QWidget
{
    Q_OBJECT
public:
    explicit Svg2GcodeP(QWidget *parent = 0);

public slots:
    void SvgToGcode(QString inputfile,QString outfile);
    void SetGcoderHeader(QString str);
    void SetLaser(QString p);
    void SetgGcodeWidth(int w);
    void SetSpeed(float,float);
    void SetCloseLine(bool a);

signals:

};

#endif // SVG2GCODEP_H
