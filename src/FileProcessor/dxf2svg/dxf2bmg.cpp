/*
 * @file dxf2bmg.cpp
 * Author : yangxu
 * Date : 2017/2/16
 */
#include "dxf2bmg.h"
#include <QDebug>

Dxf2Bmg::Dxf2Bmg()
{

}

CvPoint Dxf2Bmg::setWorldCoordinate(int rows,vector<BlockObj> ::iterator myblock, CvPoint2D32f mypoint)//世界坐标系转换，得到dxf文件边界位置
{
    CvPoint worldcoordinatepoint;
    worldcoordinatepoint.x = coordinatemove.x + (mypoint.x*myblock->sx + myblock->ipx) * zoom;
    worldcoordinatepoint.y = rows-((mypoint.y*myblock->sy + myblock->ipy) * zoom+ coordinatemove.y);
    return worldcoordinatepoint;
}

void Dxf2Bmg::drawdxfimg(Mat* img, Test_CreationClass *creationClass)
{
    vector<BlockObj> ::iterator blockItor;//块容器迭代器
    blockItor = creationClass->myblock.begin();
    while (blockItor != creationClass->myblock.end())
    {
        if (blockItor->drawflag == false)
        {
            blockItor++;
            continue;
        }
        //绘制直线
        vector<DXFLine> ::iterator lineItor;//块容器迭代器
        lineItor = blockItor->line.begin();
        while (lineItor != blockItor->line.end())
        {
            CvPoint	bpoint = setWorldCoordinate((*img).rows, blockItor, lineItor->beginPoint);
            CvPoint epoint = setWorldCoordinate((*img).rows, blockItor, lineItor->endPoint);
            line(*img, bpoint, epoint, Scalar(255, 255, 255));
            lineItor++;
        }
        //绘制圆
        vector<DXFCircle> ::iterator circleItor;//块容器迭代器
        circleItor = blockItor->circle.begin();
        while (circleItor != blockItor->circle.end())
        {
            CvPoint cpoint = setWorldCoordinate((*img).rows, blockItor, circleItor->centerPoint);
            int radius = circleItor->radius*blockItor->sx * zoom;
            circle(*img, cpoint, radius, Scalar(255, 255, 255));
            circleItor++;
        }
        //绘制圆弧
        vector<DXFArc> ::iterator arcItor;//块容器迭代器
        arcItor = blockItor->arc.begin();
        while (arcItor != blockItor->arc.end())
        {
            CvPoint cpoint = setWorldCoordinate((*img).rows, blockItor, arcItor->centerPoint);
            int radius = arcItor->radius*blockItor->sx * zoom;
            if (arcItor->beginAngle<arcItor->endAngle)//逆时针圆弧
            {
                ellipse(*img, cpoint, cvSize(radius, radius), 0, (360 - arcItor->endAngle), (360 - arcItor->beginAngle), cvScalar(255, 255, 255));
            }
            else//顺时针画圆弧
            {
                double bangle, eangle;
                bangle = 360 - arcItor->beginAngle;
                eangle = -arcItor->endAngle;
                ellipse(*img, cpoint, cvSize(radius, radius), 0, bangle, eangle, cvScalar(255, 255, 255));
            }
            arcItor++;
        }
        //绘制多线实体
        vector<DXFPolylineEntities> ::iterator polylineEntitiesItor;//多线实体容器迭代器
        polylineEntitiesItor = blockItor->polylineEntities.begin();
        while (polylineEntitiesItor != blockItor->polylineEntities.end())
        {
            for (size_t i = 0; i < polylineEntitiesItor->vertex.size(); i++)
            {
                int nextvertex = i + 1;
                if (nextvertex == polylineEntitiesItor->vertex.size())
                {
                    if (polylineEntitiesItor->isclose == true)//闭合则画终点到起点的线
                    {
                        nextvertex = 0;
                    }
                    else
                    {
                        break;
                    }
                }
                CvPoint	bpoint = setWorldCoordinate((*img).rows, blockItor, polylineEntitiesItor->vertex[i]);
                CvPoint epoint = setWorldCoordinate((*img).rows, blockItor, polylineEntitiesItor->vertex[nextvertex]);
                line(*img, bpoint, epoint, Scalar(255, 255, 255));
            }
            polylineEntitiesItor++;
        }
        blockItor++;
    }
}

void Dxf2Bmg::getWorldCoordinateborder(vector<BlockObj> ::iterator myblock, CvPoint2D32f mypoint)//世界坐标系转换，得到dxf文件边界位置
{
    CvPoint worldcoordinatepoint;
    worldcoordinatepoint.x = (mypoint.x*myblock->sx + myblock->ipx) * zoom;
    worldcoordinatepoint.y = (mypoint.y*myblock->sy + myblock->ipy) * zoom;
    if (xleftborder>worldcoordinatepoint.x)
    {
        xleftborder = worldcoordinatepoint.x;
    }
    if (xrightborder<worldcoordinatepoint.x)
    {
        xrightborder = worldcoordinatepoint.x;
    }
    if (ytopborder>worldcoordinatepoint.y)
    {
        ytopborder = worldcoordinatepoint.y;
    }
    if (ybuttomborder<worldcoordinatepoint.y)
    {
        ybuttomborder = worldcoordinatepoint.y;
    }
}

void Dxf2Bmg::getimgdxfborder(Test_CreationClass *creationClass)
{
    vector<BlockObj> ::iterator blockItor;//块容器迭代器
    blockItor = creationClass->myblock.begin();
    while (blockItor != creationClass->myblock.end())
    {
        if (blockItor->drawflag==false)
        {
            blockItor++;
            continue;
        }
        //直线
        vector<DXFLine> ::iterator lineItor;//块容器迭代器
        lineItor = blockItor->line.begin();
        while (lineItor != blockItor->line.end())
        {
            getWorldCoordinateborder(blockItor, lineItor->beginPoint);
            getWorldCoordinateborder(blockItor, lineItor->endPoint);
            lineItor++;
        }
        //圆
        vector<DXFCircle> ::iterator circleItor;//块容器迭代器
        circleItor = blockItor->circle.begin();
        while (circleItor != blockItor->circle.end())
        {
            getWorldCoordinateborder(blockItor, circleItor->centerPoint);
//            int radius = circleItor->radius*blockItor->sx * zoom;
            circleItor++;
        }
        //圆弧
        vector<DXFArc> ::iterator arcItor;//块容器迭代器
        arcItor = blockItor->arc.begin();
        while (arcItor != blockItor->arc.end())
        {
            getWorldCoordinateborder(blockItor, arcItor->centerPoint);
//            int radius = arcItor->radius*blockItor->sx * zoom;

            arcItor++;
        }
        //绘制多线实体
        vector<DXFPolylineEntities> ::iterator polylineEntitiesItor;//多线实体容器迭代器
        polylineEntitiesItor = blockItor->polylineEntities.begin();
        while (polylineEntitiesItor != blockItor->polylineEntities.end())
        {
            for (size_t i = 0; i < polylineEntitiesItor->vertex.size(); i++)
            {
                int nextvertex = i + 1;
                if (nextvertex == polylineEntitiesItor->vertex.size())
                {
                    if (polylineEntitiesItor->isclose == true)//闭合则画终点到起点的线
                    {
                        nextvertex = 0;
                    }
                    else
                    {
                        break;
                    }
                }
                getWorldCoordinateborder(blockItor, polylineEntitiesItor->vertex[i]);
                getWorldCoordinateborder(blockItor, polylineEntitiesItor->vertex[nextvertex]);
            }
            polylineEntitiesItor++;
        }
        blockItor++;
    }
}

void Dxf2Bmg::setFile(std::string filename)
{
//    char* file = filename;
//    std::cout << "Reading file " << filename << "...\n";

    Test_CreationClass* creationClass = new Test_CreationClass();
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(filename, creationClass))// if file open failed
    {
        std::cerr << filename << " could not be opened.\n";
    }
    getimgdxfborder(creationClass);//获取dxf文件描述图像宽高，以便建立相应图像
    Mat img((ybuttomborder - ytopborder)*1.2, (xrightborder - xleftborder)*1.2, CV_8UC3, Scalar(0, 0, 0));//1.2中的0.2为图像边缘的留白区域
    coordinatemove.x = (xrightborder - xleftborder)*0.1- xleftborder;
    coordinatemove.y = (ybuttomborder - ytopborder)*0.1- ytopborder;
    drawdxfimg(&img, creationClass);//绘制图像
    bitwise_xor(img,Scalar::all(255),img);
    imwrite("saveDXF.bmp", img);
    img.release();
    //重置边界参数
    ybuttomborder = 0;
    ytopborder = 0;
    xrightborder = 0;
    xleftborder = 0;

//    system("saveDXF.bmp");
    delete dxf;
    delete creationClass;
}
