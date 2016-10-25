/*
 * @file test_creationclass.cpp
 */
#include <QString>
#include <QDebug>
#include <cmath>



/*****************************************************************************
**  $Id: test_creationclass.cpp 8865 2008-02-04 18:54:02Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "creationclass.h"

#include <iostream>
#include <stdio.h>

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

/**
 * Default constructor.
 */
CreationClass:: CreationClass()
{
    svgfile = NULL;
}


/**
 * Sample implementation of the method which handles layers.
 */
void  CreationClass::addLayer(const DL_LayerData& data) {
    printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
    printAttributes();
}

/**
 * Sample implementation of the method which handles point entities.
 */
void  CreationClass::addPoint(const DL_PointData& data) {
    printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
           data.x, data.y, data.z);

    printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 */
void  CreationClass::addLine(const DL_LineData& data) {
    printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
           data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
    QString a = QObject::tr("<path d=\"M%1,%2 %3,%4\"/>\n").arg(data.x1).arg(data.y1).arg(data.x2).arg(data.y2);
    svg += a.toStdString();
    printAttributes();
}

/**
 * Sample implementation of the method which handles arc entities.
 */
void  CreationClass::addArc(const DL_ArcData& data) {
    printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
           data.cx, data.cy, data.cz,
           data.radius, data.angle1, data.angle2);
    double x1 = data.cx + data.radius*cos(deg2rad(data.angle1));
    double y1 = data.cy + data.radius*sin(deg2rad(data.angle1));
    double x2 = data.cx + data.radius*cos(deg2rad(data.angle2));
    double y2 = data.cy + data.radius*sin(deg2rad(data.angle2));

    double a2 = data.angle2;
    if(data.angle2<data.angle1)
        a2 += 360;


    int largeArcFlag = ((a2-data.angle1)>180)?1:0;

    QString a = QObject::tr("<path d=\"M%1,%2 A%3,%4 0 %5,1 %6,%7\"/>\n").arg(x1).arg(y1).arg(data.radius).arg(data.radius).arg(largeArcFlag).arg(x2).arg(y2);
    svg += a.toStdString();
    printAttributes();
}

/**
 * Sample implementation of the method which handles circle entities.
 */
void  CreationClass::addCircle(const DL_CircleData& data) {
    printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
           data.cx, data.cy, data.cz,
           data.radius);
    QString a = QObject::tr("<circle cx=\"%1\" cy=\"%2\" r=\"%3\"/>\n").arg(data.cx).arg(data.cy).arg(data.radius);
    svg += a.toStdString();
    printAttributes();
}


/**
 * Sample implementation of the method which handles polyline entities.
 */
void  CreationClass::addPolyline(const DL_PolylineData& data) {
    printf("POLYLINE \n");
    printf("flags: %d\n", (int)data.flags);
    printAttributes();
}


/**
 * Sample implementation of the method which handles vertices.
 */
void  CreationClass::addVertex(const DL_VertexData& data) {
    printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
           data.x, data.y, data.z,
           data.bulge);
    printAttributes();
}


void  CreationClass::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i=0; i<4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
               i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}


void  CreationClass::printAttributes() {
    printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    printf(" Color: ");
    if (attributes.getColor()==256)	{
        printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        printf("BYBLOCK");
    } else {
        printf("%d", attributes.getColor());
    }
    printf(" Width: ");
    if (attributes.getWidth()==-1) {
        printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        printf("DEFAULT");
    } else {
        printf("%d", attributes.getWidth());
    }
    printf(" Type: %s\n", attributes.getLinetype().c_str());
}

void  CreationClass::setFile(std::string filename)
{
    svgfile = fopen(filename.c_str(),"wb+");
    printf("All svg:\n %s",svg.c_str());
    fprintf(svgfile,"<svg id=\"svg\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">");
    fprintf(svgfile,"<g transform=\"scale(3.543299873306695,-3.543299873306695)\"  style=\"stroke:black; stroke-width:0.2; stroke-linecap:round; stroke-linejoin:round; fill:none\">");
    fprintf(svgfile,svg.c_str());
    fprintf(svgfile,"</g>\n</svg>");
    svg = "";
    fclose(svgfile);
}


// EOF
