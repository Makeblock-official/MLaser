#ifndef SVG2GCODE_H
#define SVG2GCODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
#include "XGetopt.h"
#else
#include <unistd.h>
#endif
#include <string.h>
#include <float.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define CUTTEROFF "M4 P0\n"
#define GFOOTER "G0 X0 Y0\nM4 P0\n"
#define GSTORAGE ";end \n;end \n;end \n;end \n;end \n;end \n;end \n;end \n;end \n;end \n;end \n"

typedef struct {
    float x;
    float y;
} SVGPoint;

typedef struct {
    float points[8];
    int city;
    char closed;
} ToolPath;



#ifdef _WIN32
int32_t rand31();
void seedrand(float);
double drand31();
#endif


float distPtSeg(float,float,float,float,float,float);
void cubicBez(float,float,float,float,float,float,float,float,float,int);
int pcomp(const void* a, const void* b);
void calcPaths(SVGPoint* points, ToolPath* paths,int *cities, int *npaths);
void calcBounds(struct NSVGimage* image);
void reorder(SVGPoint* pts, int* cities, int ncity);
void setPrintLaster( char*pon);
void setPrintHead(char *phead);
void setPrintWidth(int i);
int svgToGcode(char* p,char*o,int);
void setPrintSpeed(float s);
void setTravelSpeed(float);
void setCloseLines(int a);
#ifdef __cplusplus
}
#endif

#endif // SVG2GCODE_H
