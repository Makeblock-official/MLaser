#ifndef GSENDMODEL_H
#define GSENDMODEL_H

#include <QFile>
#include <QObject>
#include <QRectF>
#include <QVector3D>
#include <QTextStream>

class GSendModel : public QObject
{
    Q_OBJECT
public:
    explicit GSendModel(QObject *parent = 0);
    ~GSendModel();
//    QString ReadOneLine();
    QString readOneLine();
    QRectF getRect(){return m_rects;}
    void SetTargetFile(QString file);
    void reset();
    quint64  getTotalLayer();
    quint64  getTotalGcmd();
    quint64  getTotalTime();
    quint64  getPersent();

public slots:
private:
    QFile* target_gcode;
    QString print_file;
    QRectF m_rects;
    QTextStream* in;
    QString currentLine;

    quint64 layers;
    quint64 g_cmds;
    quint64 totaltimes;
    quint64 currentcmds;
private slots:

};

#endif // GSENDMODEL_H
