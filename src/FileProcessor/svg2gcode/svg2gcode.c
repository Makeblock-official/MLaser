/*  
 * svg2gcode (c) Matti Koskinen 2014
 *
 * reorder-function based on StippleGen
 * Copyright (C) 2012 by Windell H. Oskay, www.evilmadscientist.com
 *
 * nanosvg.h (c) 2014 Mikko Mononen
 * some routines based on nanosvg-master example1.c
 *
 * Xgetopt used on VS2010 (or later) by Hans Dietrich,David Smith
 * code public domain
 *
 * No comments :-)
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * http://creativecommons.org/licenses/LGPL/2.1/
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "svg2gcode.h"


static SVGPoint bezPoints[64];
static SVGPoint first,last;
static int bezCount = 0;
static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }
static float bounds[4];
static int pathCount,pointsCount,shapeCount;
static int doBez = 1;
static int bLine = 1;
static int simplify = 0;
static struct NSVGimage* g_image = NULL;

uint64_t seed;

int g_width = 100;    //picture width
float feed = 500.0;
float feed_travel = 2000;
char pon [40];
char pheader[60];


int32_t rand31() {
    uint64_t tmp1;
    uint32_t tmp2;

    /* x = (16807 * x) % 0x7FFFFFFF */
    tmp1 = (uint64_t) ((int32_t) seed * (int64_t) 16807);
    tmp2 = (uint32_t) tmp1 & (uint32_t) 0x7FFFFFFF;
    tmp2 += (uint32_t) (tmp1 >> 31);
    if ((int32_t) tmp2 < (int32_t) 0)
        tmp2 = (tmp2 + (uint32_t) 1) & (uint32_t) 0x7FFFFFFF;
    return (int32_t)tmp2;
}
void seedrand(float seedval) {
    seed = (int32_t) ((double) seedval + 0.5);
    if (seed < 1L) {                   /* seed from current time */
        seed = time(NULL);
        seed = ((seed - 1UL) % 0x7FFFFFFEUL) + 1UL;
    }
    else {
        seed = ((seed - 1L) % 0x7FFFFFFEL) + 1L;
    }
    seed = rand31(seed);
    seed = rand31(seed);
}

double drnd31() {
    double x;
    seed = rand31();
    x = (double)(seed-0x3FFFFFFFL) * (2.0 / 1073741823.015625);
    if(fabs(x) > 1.0)
        x = drnd31();
    return fabs(x);
}
//#endif

float distPtSeg(float x, float y, float px, float py, float qx, float qy)
{
    float pqx, pqy, dx, dy, d, t;
    pqx = qx-px;
    pqy = qy-py;
    dx = x-px;
    dy = y-py;
    d = pqx*pqx + pqy*pqy;
    t = pqx*dx + pqy*dy;
    if (d > 0) t /= d;
    if (t < 0) t = 0;
    else if (t > 1) t = 1;
    dx = px + t*pqx - x;
    dy = py + t*pqy - y;
    return dx*dx + dy*dy;
}
void setCloseLines(int a)
{
    bLine = a;
}

// bezier smoothing
void cubicBez(float x1, float y1, float x2, float y2,
              float x3, float y3, float x4, float y4,
              float tol, int level)
{
    float x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
    float d;

    if (level > 12) return;

    x12 = (x1+x2)*0.5f;
    y12 = (y1+y2)*0.5f;
    x23 = (x2+x3)*0.5f;
    y23 = (y2+y3)*0.5f;
    x34 = (x3+x4)*0.5f;
    y34 = (y3+y4)*0.5f;
    x123 = (x12+x23)*0.5f;
    y123 = (y12+y23)*0.5f;
    x234 = (x23+x34)*0.5f;
    y234 = (y23+y34)*0.5f;
    x1234 = (x123+x234)*0.5f;
    y1234 = (y123+y234)*0.5f;

    d = distPtSeg(x1234, y1234, x1,y1, x4,y4);
    if (d > tol*tol) {
        cubicBez(x1,y1, x12,y12, x123,y123, x1234,y1234, tol, level+1);
        cubicBez(x1234,y1234, x234,y234, x34,y34, x4,y4, tol, level+1);
    } else {
        bezPoints[bezCount].x = x4;
        bezPoints[bezCount].y = y4;
        bezCount++;
        if(bezCount > 900) {
            printf("!bez count\n");
            bezCount = 900;
        }
    }
}
//#define TESTRNG
#ifdef _WIN32 //win doesn't have good RNG
#define RANDOM() drnd31() //((double)rand()/(double)RAND_MAX)
#else //OSX LINUX much faster than win
#define RANDOM() (drand48())
#endif


int pcomp(const void* a, const void* b) {
    SVGPoint* ap = (SVGPoint*)a;
    SVGPoint* bp = (SVGPoint*)b;
    if(sqrt(ap->x*ap->x + ap->y*ap->y) > sqrt(bp->x*bp->x+bp->y*bp->y))
        return 1;
    return -1;
}

// get all paths and add a city for each path		  
void calcPaths(SVGPoint* points, ToolPath* paths,int *cities, int *npaths) {
    struct NSVGshape* shape;
    struct NSVGpath* path;
    //    FILE *f;
    int i,j,k,l,p,b;
    SVGPoint* pts;
#ifdef DO_HPGL 
    f=fopen("test.hpgl","w");
    fprintf(f,"IN;SP1;");
#endif
    i=0;
    k=0;
    j=0;
    p=0;
    for(shape = g_image->shapes; shape != NULL; shape=shape->next) {
        for(path = shape->paths; path != NULL; path=path->next) {
            if(path->closed && simplify) {
                pts = (SVGPoint*)malloc(path->npts*sizeof(SVGPoint));

                for(l=0;l<path->npts-1;l++) {
                    float *pp=&path->pts[l*2];
                    if(l==0) {
                        points[i].x = pp[0];
                        points[i].y = pp[1];
                    }
                    pts[l].x = pp[0];
                    pts[l].y = pp[1];
                }
                qsort((void*)pts,path->npts-1,sizeof(SVGPoint),pcomp);
                paths[k].points[0] = pts[path->npts-2].x;
                paths[k].points[1] = pts[path->npts-2].y;
                paths[k].points[2] = pts[0].x;
                paths[k].points[3] = pts[0].y;
                paths[k].closed = path->closed;
                paths[k].city = i;
                k++;
                //fprintf(stderr,"i %d pts %f %f %f %f\n",i,pts[path->npts-2].x,pts[path->npts-2].y,pts[0].x,pts[0].y);
                free(pts);

                goto cont;
            }

            for(j=0;j<path->npts-1;(doBez ? j+=3 : j++)) {
                float *pp = &path->pts[j*2];
                if(j==0) {

                    points[i].x = pp[0];
                    points[i].y = pp[1];
#ifdef DO_HPGL
                    fprintf(f,"PU%d,%d;",(int)pp[0],(int)pp[1]);
                    fflush(f);

                } else {
                    fprintf(f,"PD%d,%d;",(int)pp[0],(int)pp[1]);
                    fflush(f);
#endif
                }
                if(doBez) {
                    for(b=0;b<8;b++)
                        paths[k].points[b]=pp[b];
                } else {
                    paths[k].points[0] = pp[0];
                    paths[k].points[1] = pp[1];
                    paths[k].points[2] = pp[0];
                    paths[k].points[3] = pp[1];
                }
                paths[k].closed = path->closed;
                paths[k].city = i;
                k++;

            }
cont:
            if(k>pointsCount) {
                printf("error k > \n");
#ifdef DO_HPGL
                fprintf(f,"PU0,0;\n");
                fclose(f);
#endif
                *npaths = 0;
                return;

            }
            if(i>pathCount) {
                printf("error i > \n");
#ifdef DO_HPGL
                fprintf(f,"PU0,0;\n");
                fclose(f);
#endif
                exit(-1);
                /*
      *npaths=k;
      return;
     */
            }
            cities[i] = i;
            i++;
        }

        j++;
    }
    printf("total paths %d, total points %d\n",i,k);
    *npaths = k;
#ifdef DO_HPGL
    fprintf(f,"PU0,0;\n");
    fclose(f);
#endif
}

void calcBounds(struct NSVGimage* image)
{
    struct NSVGshape* shape;
    struct NSVGpath* path;
    int i;
    bounds[0] = FLT_MAX;
    bounds[1] = FLT_MAX;
    bounds[2] = -FLT_MAX;
    bounds[3] = -FLT_MAX;
    pathCount = 0;
    pointsCount = 0;
    shapeCount = 0;
    for (shape = image->shapes; shape != NULL; shape = shape->next) {
        for (path = shape->paths; path != NULL; path = path->next) {
            for (i = 0; i < path->npts-1; i++) {
                float* p = &path->pts[i*2];
                bounds[0] = minf(bounds[0], p[0]);
                bounds[1] = minf(bounds[1], p[1]);
                bounds[2] = maxf(bounds[2], p[0]);
                bounds[3] = maxf(bounds[3], p[1]);
                pointsCount++;
            }
            pathCount++;
        }
        shapeCount++;
    }
    printf("shapes %d\n",shapeCount);

}
//reorder  the paths to minimize cutter movement
void reorder(SVGPoint* pts, int* cities, int ncity) {
    int i,j,k,temp1,temp2,indexA,indexB, indexH, indexL;
    float dx,dy,dist,dist2;
    SVGPoint p1,p2,p3,p4;
    for(i=0;i<800*ncity;i++) {
        indexA = (int)(RANDOM()*(ncity-2));
        indexB = (int)(RANDOM()*(ncity-2));
        if(abs(indexB-indexA) < 2)
            continue;
        if(indexB < indexA) {
            temp1 = indexB;
            indexB = indexA;
            indexA = temp1;
        }
        p1 = pts[cities[indexA]];
        p2 = pts[cities[indexA+1]];
        p3 = pts[cities[indexB]];
        p4 = pts[cities[indexB+1]];
        dx = p1.x-p2.x;
        dy = p1.y-p2.y;
        dist = dx*dx+dy*dy;
        dx = p3.x-p4.x;
        dy = p3.y-p4.y;
        dist += (dx*dx+dy*dy);
        dx = p1.x-p3.x;
        dy = p1.y-p3.y;
        dist2 = dx*dx+dy*dy;
        dx = p2.x-p4.x;
        dy = p2.y-p4.y;
        dist2 += (dx*dx+dy*dy);
        if(dist2 < dist) {
            indexH = indexB;
            indexL = indexA+1;
            while(indexH > indexL) {
                temp1 = cities[indexL];
                cities[indexL]=cities[indexH];
                cities[indexH] = temp1;
                indexH--;
                indexL++;
            }
        }
    }
}
void  setPrintLaster(char *pons )
{
    int i = strlen(pons);
    strncpy(pon,pons,i);
}
void setPrintHead(char* phead)
{
    int i = strlen(phead);
    strncpy(pheader,phead,i);
}
void setPrintWidth(int w)
{
    g_width = w;
}
void setTravelSpeed(float s)
{
    feed_travel = s;
}
void setPrintSpeed(float s)
{
    feed = s;
}
int svgToGcode(char * file,char *outputfile,int be)
{
    int i,j,k,l,first;
    doBez = be;

    SVGPoint* points;
    ToolPath* paths;
    int *cities;
    int npaths;
    int numReord = 30;
    int flip = 0;
    int skip = 0;
    int printed=0;
    int cncMode = 0;

    float ztraverse = 1.;
    float zengage = 1.;
    float width = -1;
    float w = -1.0;

    float scale = 0.01;
    float tol = 0.5;
    float x,y,bx,by,bxold,byold,d,firstx,firsty;
    float xold,yold;

    FILE *gcode;

    char *input_file = file;

    simplify = 0;
    //get svg file
    g_image = nsvgParseFromFile(input_file,"mm",0.1);
    if(g_image == NULL) {
        printf("error: Can't open input %s\n",input_file);
        return -1;
    }
    calcBounds(g_image);
    //  set width of the picture
    width = g_width;
    w = fabs(bounds[0]-bounds[2]);
    float ws = fabs(bounds[1]-bounds[3]);
    printf("bounds 0=%f 1=%f 2=%f 3=%f\n",bounds[0],bounds[1],bounds[2],bounds[3]);
    scale = width/w;
#ifdef _WIN32
    seedrand((float)time(0));
#endif
    gcode=fopen(outputfile,"w");
    if(gcode == NULL)
    {
        return -1;
    }
    printf("paths %d points %d\n",pathCount, pointsCount);
    points = (SVGPoint*)malloc(pathCount*2*sizeof(SVGPoint));
    cities = (int*)malloc(pathCount*2*sizeof(int));
    paths = (ToolPath*)malloc(pointsCount*2*sizeof(ToolPath));

    npaths = 0;
    calcPaths(points,paths,cities,&npaths);

    printf("reorder ");
    //export to the known file
    for(k=0;k<numReord;k++) {
        reorder(points,cities,pathCount);
        printf("%d... ",k);
        fflush(stdout);
    }
    printf("\n");
//    fprintf(gcode,";bounds %f %f %f %f paths %d\n",(bounds[0]*scale),(bounds[1]*scale),(bounds[2]*scale),(bounds[3]*scale),npaths);

    //print header
    fprintf(gcode,pheader);
    int g_store = 0;
    k=0;
    i=0;
    for(i=0;i<pathCount;i++)
    {
        for(k=0;k<npaths;k++)
        {
            if(paths[k].city == -1)
                continue;
            if(paths[k].city == cities[i])
                break;
        }
        if(k >= npaths-1) {
            //printf("k > \n");
            continue;
        }
        firstx = x = (paths[k].points[0]-bounds[0])*scale;
        firsty = y =  (flip ? (bounds[3]-paths[k].points[1])*scale : (paths[k].points[1]-bounds[1])*scale);
        if(!cncMode)
            fprintf(gcode,"G0 X%.1f Y%.1f Z%.1f\n",x,y,ztraverse);
        else
            fprintf(gcode,"G0 X%.1f Y%.1f\n",x,y);


        if(!cncMode)
        {
           fprintf(gcode,pon);
           fprintf(gcode,"G1 F%.1f \n",feed);
        }

        else
            fprintf(gcode,"G1 Z%.1f \n",zengage);
        printed=0;
        for(j=k;j<npaths;j++)
        {
            xold = x;
            yold = y;
            g_store++;
            //printf("bezC %d\n",bezCount);
            skip = 0;
            first = 1;

            if(paths[j].city == cities[i])
            {
                //bez
                if(doBez)
                {
                    bezCount = 0;
                    if(paths[j].points[0] == paths[j].points[2] && paths[j].points[1]==paths[j].points[3])
                        ;//continue;
                    cubicBez(paths[j].points[0],paths[j].points[1],paths[j].points[2],paths[j].points[3],paths[j].points[4],paths[j].points[5],paths[j].points[6],paths[j].points[7],tol,0);
                    bxold=x;
                    byold=y;
                    for(l=0;l<bezCount;l++)
                    {
                        if(bezPoints[l].x > bounds[2] || bezPoints[l].x < bounds[0] || isnan(bezPoints[l].x)) {
                            printf("bezPoints %f %f\n",bezPoints[l].x,bounds[0]);
                            continue;
                        }
                        if(bezPoints[l].y > bounds[3]) {
                            printf("bezPoints Y %d\n",l);
                            continue;
                        }
                        bx = (bezPoints[l].x-bounds[0])*scale;
                        by = (flip ? (bounds[3]-bezPoints[l].y)*scale : (bezPoints[l].y-bounds[1])*scale);
                        d = sqrt((bx-bxold)*(bx-bxold)+(by-byold)*(by-byold));
                        fprintf(gcode,"G1 X%.1f Y%.1f Z%.1f \n",bx,by,zengage);
                        bxold = bx;
                        byold = by;
                    }
                }
                else
                {
                    x = (paths[j].points[0]-bounds[0])*scale;
                    y = (flip ? (bounds[3]-paths[j].points[1])*scale : (paths[j].points[1]-bounds[1])*scale);
                    //	  if(paths[j].points[0]==paths[j].points[2] && paths[j].points[1]==paths[j].points[3]) {
                    if(1)
                    {
                        d = sqrt((x-xold)*(x-xold)+(y-yold)*(y-yold));
                        fprintf(gcode,"G1 X%.1f Y%.1f Z%.1f \n",x,y,zengage);
                        first = 0;
                        xold = x;
                        yold = y;
                    }
                    else
                    {
                        x = (paths[j].points[0]-bounds[0])*scale;
                        y = (flip ? (bounds[3]-paths[j].points[1])*scale : (paths[j].points[1]-bounds[1])*scale);
                        fprintf(gcode,CUTTEROFF);
                        fprintf(gcode,"( simplified )\n");
                        fprintf(gcode,"G0 X%.1f Y%.1f Z%.1f\n",x,y,ztraverse);
                        x = (paths[j].points[2]-bounds[0])*scale;
                        y = (flip ? (bounds[3]-paths[j].points[3])*scale : (paths[j].points[3]-bounds[1])*scale);
                        fprintf(gcode,pon);
                        xold = x;
                        yold = y;
                    }
                }
                paths[j].city = -1;
            } else
                break;
        }
        if(paths[j].closed||bLine)
        {

            fprintf(gcode,"G1 X%.1f Y%.1f Z%.1f \n",firstx,firsty,zengage);
//            fprintf(gcode,"here is a cloase cycle \n\n");
        }
        if(!cncMode)
        {
            fprintf(gcode,CUTTEROFF);
            fprintf(gcode,"G0 F%.1f \n",feed_travel);
        }

        else
            fprintf(gcode,"G1 Z%f \n",ztraverse);
    }
    fprintf(gcode,GFOOTER);

    fclose(gcode);
    free(points);
    free(cities);
    free(paths);
    nsvgDelete(g_image);
    return 0;
}
