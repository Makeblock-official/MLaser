/*******************************************************************************
*                                                                              *
* Author    :  Peng Chen                                                       *
* Version   :  0.0.1                                                           *
* Date      :  12 July 2014                                                    *
* Website   :  http://www.3green-tech.com                                      *
* Copyright :  3green-tech 2010-2014                                           *
*                                                                              *
* Attributions:                                                                *
* This Class was use for promotion the QGraphicsView which                     *
* the QT designer provide.After been promoted,the main useful                  *
* founction "resizeEvent()"will be rewritted.                                  *
*                                                                              *
*******************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>

class Graph : public QGraphicsView
{
public:
    Graph(QWidget*);
    bool gcode;
    void setDrag(DragMode mode);
protected:
    void resizeEvent(QResizeEvent *event)
    {

        if(scene())
            if(gcode)
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
};

#endif // GRAPH_H
