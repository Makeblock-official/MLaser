#pragma once
#ifndef TIANCHI_TCQRENCODE_H
#define TIANCHI_TCQRENCODE_H

#ifndef TIANCHI_API
    #define TIANCHI_API
#endif

#include <QImage>
#include <QString>

class TIANCHI_API TcQrencode
{
public:
    static QImage encodeImage(const QString& s, int bulk=8);
};

#endif // TIANCHI_TCQRENCODE_H

