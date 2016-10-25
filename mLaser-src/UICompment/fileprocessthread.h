#ifndef FILEPROCESSTHREAD_H
#define FILEPROCESSTHREAD_H

#include <QObject>
#include <QFile>
#include "renderarea/pgraphicsscene.h"

#include "FileProcessor/bitmap2svg/QPotrace/bitmap2svg.h"
#include "FileProcessor/bitmap2gcode/bitmap2gcode.h"
#include "FileProcessor/svg2gcode/svg2gcodep.h"
#include "FileProcessor/font2svg/font2svg.h"
#include "FileProcessor/gcoder/gmodel.h"

class FileProcessThread : public QObject
{
    Q_OBJECT
public:
    explicit FileProcessThread(QObject *parent = 0);



public slots:
    void slotCuttingFile(PGraphicsScene*pScene);
    void slotCarveFile(PGraphicsScene*pScene);
private:
    Bitmap2Svg* b2s;
    Bitmap2Gcode* b2g;
    Svg2GcodeP* s2g;
    GModel* gcode;
signals:
    void Sig_CombinePrint(QStringList);

};

#endif // FILEPROCESSTHREAD_H
