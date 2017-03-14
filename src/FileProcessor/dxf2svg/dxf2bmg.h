#ifndef DXF2BMG_H
#define DXF2BMG_H

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "test_creationclass.h"

class Dxf2Bmg
{

public:
    void setFile(std::string filename);
    Dxf2Bmg();

private:
    CvPoint setWorldCoordinate(int rows,vector<BlockObj> ::iterator myblock, CvPoint2D32f mypoint);//设置世界坐标系，得到dxf文件边界位置
    void getWorldCoordinateborder(vector<BlockObj> ::iterator myblock, CvPoint2D32f mypoint);//获得世界坐标系，得到dxf文件边界位置
    void drawdxfimg(Mat* img, Test_CreationClass *creationClass);
    void getimgdxfborder(Test_CreationClass *creationClass);

    int zoom = 1;//放大倍数
    CvPoint coordinatemove;//坐标偏移
    int xleftborder = 0;
    int xrightborder = 0;
    int ytopborder = 0;
    int ybuttomborder = 0;
};

#endif // DXF2BMG_H
