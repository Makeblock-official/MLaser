#include <QTime>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include "qwebservercontrol.h"
#include "QCoreApplication"

QWebServerControl::QWebServerControl()
{

}

void QWebServerControl::service(HttpRequest& request, HttpResponse& response)
{
    QString path = QCoreApplication::applicationDirPath();
    QFile::remove(path + "/upload.jpg");
    QFile::remove(path + "/upload.png");
    QString as = request.getParameter("imgfile");
    QTemporaryFile* file=request.getUploadedFile("imgfile");

    QImage a(file->fileName());
    qDebug()<<"we get a string:"<<file->fileName();
    if(QFileInfo(QFile(as)).suffix()=="jpg"||QFileInfo(QFile(as)).suffix()=="JPG")
    {
        a.save(path + "/upload.jpg");
        as =path + "/upload.jpg";
    }
    else if(QFileInfo(QFile(as)).suffix()=="png"||QFileInfo(QFile(as)).suffix()=="PNG")
    {
        a.save(path + "upload.png");
        as = path + "/upload.png";
    }
    else if(QFileInfo(QFile(as)).suffix()=="jpeg"||QFileInfo(QFile(as)).suffix()=="JPEG")
    {
        a.save(path + "/upload.jpeg");
        as =path + "/upload.jpeg";
    }
    emit Sig_ServerLoad(as);

}
