
#include <QDebug>
#include "requestrouter.h"

HttpSessionStore* RequestRouter::sessionStore=0;
StaticFileController* RequestRouter::staticFileControler=0;

RequestRouter::RequestRouter(QObject *parent) :
    HttpRequestHandler(parent)
{
    control = new QWebServerControl();
    connect(control,SIGNAL(Sig_ServerLoad(QString)),this,SLOT(slotServerLoad(QString)));
}

void RequestRouter::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    //接收控制指令
    if (path==("/dd"))
    {
        control->service(request, response);
    }
    else if(path.startsWith("/files"))
    {
        staticFileControler->service(request,response);
    }
    else
    {
        response.setStatus(404,"Not found");
        response.write("The URL is wrong, no such document.",true);
    }
    qDebug("RequestMapper: finished request");
}
void RequestRouter::slotServerLoad(QString cmd)
{
    emit Sig_ServerLoad(cmd);
}
