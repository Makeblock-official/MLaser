#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include "./httpserver/httprequesthandler.h"
#include "./httpserver/httpsessionstore.h"
#include "./httpserver/staticfilecontroller.h"

#include "qwebservercontrol.h"

class RequestRouter : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit RequestRouter(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
    static HttpSessionStore* sessionStore;
    static StaticFileController* staticFileControler;


public slots:
    void slotServerLoad(QString);
private:
    QWebServerControl* control;
signals:
    void Sig_ServerLoad(QString);
};

#endif // REQUESTROUTER_H
