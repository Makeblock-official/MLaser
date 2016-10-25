#include <QTime>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include "qwebservercontrol.h"

QWebServerControl::QWebServerControl()
{

}

void QWebServerControl::service(HttpRequest& request, HttpResponse& response)
{
    QFile::remove("upload.jpg");
    QFile::remove("upload.png");
    QString as = request.getParameter("imgfile");
    QTemporaryFile* file=request.getUploadedFile("imgfile");

    QImage a(file->fileName());
    qDebug()<<"we get a string:"<<file->fileName();
    if(QFileInfo(QFile(as)).suffix()=="jpg"||QFileInfo(QFile(as)).suffix()=="JPG")
    {
        a.save("upload.jpg");
        as = "upload.jpg";
    }
    else if(QFileInfo(QFile(as)).suffix()=="png"||QFileInfo(QFile(as)).suffix()=="PNG")
    {
        a.save("upload.png");
         as = "upload.png";
    }
    else if(QFileInfo(QFile(as)).suffix()=="jpeg"||QFileInfo(QFile(as)).suffix()=="JPEG")
    {
        a.save("upload.jpeg");
         as = "upload.jpeg";
    }
    emit Sig_ServerLoad(as);

}
