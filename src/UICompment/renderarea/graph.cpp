#include "graph.h"

Graph::Graph(QWidget*parent)
{
    gcode = false;
}
void Graph::setDrag(DragMode mode)
{
    this->setDragMode(mode);
}
