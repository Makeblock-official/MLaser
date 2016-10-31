#ifndef QWEBSERVERCONTROL_H
#define QWEBSERVERCONTROL_H

#include "./httpserver/httprequesthandler.h"

class QWebServerControl : public HttpRequestHandler
{
    Q_OBJECT
public:
    QWebServerControl();
    void service(HttpRequest& request, HttpResponse& response);

private:
signals:
    void Sig_ServerLoad(QString);
};

#endif // QWEBSERVERCONTROL_H
