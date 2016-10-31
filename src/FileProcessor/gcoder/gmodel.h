#ifndef GMODEL_H
#define GMODEL_H

#include <QString>
#include <QVector>
#include <QVector3D>
#include <math.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


//Just store the temp line information for rendingthe line in graphicsview
class partIsland
{
public:
    QVector<QVector3D>island;

};


class GModel
{
public:
    GModel();
    void loadFile(QString filePath);
    void filterGcode(QString filePath,QPointF pos,QPointF scope,QString outfile);
    void resetData();
    void render();
    QVector3D bounds;
    qreal scale;
    QVector3D outMin;

    QString fileName() const { return m_fileName; }
    QVector<QVector3D> m_points;
    QVector<partIsland>m_parts;
private:
    QString m_fileName;
    QPointF last_pos;
    int cur_speed;  // 1:normal g1 speed 2:accerlte g1 speed
};

#endif // GMODEL_H
