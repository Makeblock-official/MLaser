/**
 *块对象类，存放DXF块对象（包括世界坐标、缩放比例、线、圆等对象）
 */
#ifndef BLOCK_OBJ_H
#define BLOCK_OBJ_H
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//线对象
class DXFLine
{
public:
    CvPoint2D32f beginPoint;
    CvPoint2D32f endPoint;
};
//圆对象
class DXFCircle
{
public:
    CvPoint2D32f centerPoint;
    double radius;
};
//多线实体对象
class DXFPolylineEntities
{
public:
    vector<CvPoint2D32f> vertex;//顶点
    bool isclose;
};
//圆弧对象
class DXFArc
{
public:
    CvPoint2D32f centerPoint;
    double radius;
    double beginAngle;//起点角度
    double endAngle;//终点角度
};
//块对象
class BlockObj
{
public:
    BlockObj()
    {
        sx = 1;
        sy = 1;
        sz = 1;
        ipx = 0;
        ipy = 0;
        ipz = 0;
        line.clear();
    }
    ~BlockObj(){}

    string name;//块名称
    //缩放比例
    double sx;
    double sy;
    double sz;
    //用户坐标系相对世界坐标系中的位置
    double ipx;
    double ipy;
    double ipz;
    vector<DXFLine> line;
    vector<DXFCircle> circle;
    vector<DXFPolylineEntities> polylineEntities;
    vector<DXFArc> arc;
    bool drawflag;
};
#endif // BLOCK_OBJ_H
