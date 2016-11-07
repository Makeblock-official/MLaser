#-------------------------------------------------
#
# Project created by QtCreator 2016-08-06T12:22:22
#
#-------------------------------------------------

QT       += core gui serialport
QT       += svg opengl webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mLaser
VERSION = 2.2
TEMPLATE = app
CONFIG += c++11

ICON = logo.icns
QMAKE_INFO_PLIST = Info.plist
OTHER_FILES += Info.plist

include(./Zipunzip/karchive.pri)

isEmpty(FV_APP_NAME) {
        warning("Fervor: falling back to application name '$$TARGET'")
        DEFINES += FV_APP_NAME=\\\"$$TARGET\\\"
} else {
        message("Fervor: building for application name '$$FV_APP_NAME'")
        DEFINES += FV_APP_NAME=\\\"$$FV_APP_NAME\\\"
}

isEmpty(FV_APP_VERSION) {
        warning("Fervor: falling back to application version '$$VERSION'")
        DEFINES += FV_APP_VERSION=\\\"$$VERSION\\\"
} else {
        message("Fervor: building for application version '$$FV_APP_VERSION'")
        DEFINES += FV_APP_VERSION=\\\"$$FV_APP_VERSION\\\"
}

include(./FileProcessor/bitmap2svg/QPotrace/QxPotrace.pri)
SOURCES += main.cpp \
    Communicate/serial/cpserialport.cpp \
    Communicate/serial/frontend.cpp \
    FileProcessor/bitmap2gcode/bitmap2gcode.cpp \
    FileProcessor/gcoder/gmodel.cpp \
    FileProcessor/gcoder/gsendmodel.cpp \
    FileProcessor/svg2gcode/svg2gcodep.cpp \
    UICompment/renderarea/graph.cpp \
    UICompment/renderarea/msvgitem.cpp \
    UICompment/renderarea/pixmapitem.cpp \
    UICompment/aa_loadform.cpp \
    UICompment/ab_serialform.cpp \
    UICompment/ac_autocheckform.cpp \
    UICompment/ad_laserform.cpp \
    UICompment/ae_beginform.cpp \
    UICompment/af_backform.cpp \
    UICompment/ba_easyform.cpp \
    UICompment/bb_easynform.cpp \
    UICompment/bc_expertform.cpp \
    UICompment/bd_viewform.cpp \
    UICompment/controlform.cpp \
    UICompment/loadform.cpp \
    UICompment/postionform.cpp \
    UICompment/qcptitle.cpp \
    UICompment/qmenubutoon.cpp \
    UICompment/textform.cpp \
    UICompment/titlebar.cpp \
    UICompment/uimanager.cpp \
    FileProcessor/svg2gcode/nanosvg.c \
    FileProcessor/svg2gcode/svg2gcode.c \
    FileProcessor/svg2gcode/XGetopt.c \
    UICompment/renderarea/pgraphicsscene.cpp \
    FileProcessor/bitmap2svg/QPotrace/bitmap2svg.cpp \
    Update/fvavailableupdate.cpp \
    Update/fvignoredversions.cpp \
    Update/fvplatform.cpp \
    Update/fvupdateconfirmdialog.cpp \
    Update/fvupdater.cpp \
    Update/fvupdatewindow.cpp \
    Update/fvversioncomparator.cpp \
    FileProcessor/dxf2svg/creationclass.cpp \
    FileProcessor/dxf2svg/dl_dxf.cpp \
    FileProcessor/dxf2svg/dl_writer_ascii.cpp \
    Server/httpserver/httpconnectionhandler.cpp \
    Server/httpserver/httpconnectionhandlerpool.cpp \
    Server/httpserver/httpcookie.cpp \
    Server/httpserver/httpglobal.cpp \
    Server/httpserver/httplistener.cpp \
    Server/httpserver/httprequest.cpp \
    Server/httpserver/httprequesthandler.cpp \
    Server/httpserver/httpresponse.cpp \
    Server/httpserver/httpsession.cpp \
    Server/httpserver/httpsessionstore.cpp \
    Server/httpserver/staticfilecontroller.cpp \
    Server/qwebservercontrol.cpp \
    Server/requestrouter.cpp \
    UICompment/qrencodeform.cpp \
    UICompment/encode/tcQrencode.cpp \
    UICompment/encode/bitstream.c \
    UICompment/encode/mask.c \
    UICompment/encode/mmask.c \
    UICompment/encode/mqrspec.c \
    UICompment/encode/qrencode.c \
    UICompment/encode/qrinput.c \
    UICompment/encode/qrspec.c \
    UICompment/encode/rscode.c \
    UICompment/encode/split.c \
    UICompment/gcodeform.cpp \
    UICompment/cqcomobox.cpp \
    UICompment/passwordform.cpp \
    UICompment/hexloadform.cpp \
    UICompment/hexloadbase.cpp \
    UICompment/fileprocessthread.cpp \
    UICompment/ag_lastform.cpp \
    Zipunzip/zipunzip.cpp \
    UICompment/ah_hexform.cpp \
    UICompment/firmsettingform.cpp

FORMS += \
    Communicate/serial/cpserialport.ui \
    UICompment/aa_loadform.ui \
    UICompment/ab_serialform.ui \
    UICompment/ac_autocheckform.ui \
    UICompment/ad_laserform.ui \
    UICompment/ae_beginform.ui \
    UICompment/af_backform.ui \
    UICompment/ba_easyform.ui \
    UICompment/bb_easynform.ui \
    UICompment/bc_expertform.ui \
    UICompment/bd_viewform.ui \
    UICompment/controlform.ui \
    UICompment/loadform.ui \
    UICompment/postionform.ui \
    UICompment/qcptitle.ui \
    UICompment/textform.ui \
    Update/fvupdateconfirmdialog.ui \
    Update/fvupdatewindow.ui \
    UICompment/qrencodeform.ui \
    UICompment/gcodeform.ui \
    UICompment/passwordform.ui \
    UICompment/hexloadform.ui \
    UICompment/ag_lastform.ui \
    UICompment/ah_hexform.ui \
    UICompment/firmsettingform.ui

HEADERS += \
    Communicate/serial/cpserialport.h \
    Communicate/serial/frontend.h \
    FileProcessor/bitmap2gcode/bitmap2gcode.h \
    FileProcessor/gcoder/gmodel.h \
    FileProcessor/gcoder/gsendmodel.h \
    FileProcessor/svg2gcode/nanosvg.h \
    FileProcessor/svg2gcode/svg2gcode.h \
    FileProcessor/svg2gcode/svg2gcodep.h \
    FileProcessor/svg2gcode/XGetopt.h \
    UICompment/renderarea/graph.h \
    UICompment/renderarea/msvgitem.h \
    UICompment/renderarea/pixmapitem.h \
    UICompment/aa_loadform.h \
    UICompment/ab_serialform.h \
    UICompment/ac_autocheckform.h \
    UICompment/ad_laserform.h \
    UICompment/ae_beginform.h \
    UICompment/af_backform.h \
    UICompment/ba_easyform.h \
    UICompment/bb_easynform.h \
    UICompment/bc_expertform.h \
    UICompment/bd_viewform.h \
    UICompment/controlform.h \
    UICompment/loadform.h \
    UICompment/postionform.h \
    UICompment/qcptitle.h \
    UICompment/qmenubutoon.h \
    UICompment/textform.h \
    UICompment/titlebar.h \
    UICompment/uimanager.h \
    UICompment/renderarea/pgraphicsscene.h \
    FileProcessor/bitmap2svg/QPotrace/bitmap2svg.h \
    Update/fvavailableupdate.h \
    Update/fvignoredversions.h \
    Update/fvplatform.h \
    Update/fvupdateconfirmdialog.h \
    Update/fvupdater.h \
    Update/fvupdatewindow.h \
    Update/fvversioncomparator.h \
    FileProcessor/dxf2svg/creationclass.h \
    FileProcessor/dxf2svg/dl_attributes.h \
    FileProcessor/dxf2svg/dl_codes.h \
    FileProcessor/dxf2svg/dl_creationadapter.h \
    FileProcessor/dxf2svg/dl_creationinterface.h \
    FileProcessor/dxf2svg/dl_dxf.h \
    FileProcessor/dxf2svg/dl_entities.h \
    FileProcessor/dxf2svg/dl_exception.h \
    FileProcessor/dxf2svg/dl_extrusion.h \
    FileProcessor/dxf2svg/dl_global.h \
    FileProcessor/dxf2svg/dl_writer.h \
    FileProcessor/dxf2svg/dl_writer_ascii.h \
    Server/httpserver/httpconnectionhandler.h \
    Server/httpserver/httpconnectionhandlerpool.h \
    Server/httpserver/httpcookie.h \
    Server/httpserver/httpglobal.h \
    Server/httpserver/httplistener.h \
    Server/httpserver/httprequest.h \
    Server/httpserver/httprequesthandler.h \
    Server/httpserver/httpresponse.h \
    Server/httpserver/httpsession.h \
    Server/httpserver/httpsessionstore.h \
    Server/httpserver/staticfilecontroller.h \
    Server/qwebservercontrol.h \
    Server/requestrouter.h \
    UICompment/qrencodeform.h \
    UICompment/encode/bitstream.h \
    UICompment/encode/mask.h \
    UICompment/encode/mmask.h \
    UICompment/encode/mqrspec.h \
    UICompment/encode/qrencode.h \
    UICompment/encode/qrencode_inner.h \
    UICompment/encode/qrinput.h \
    UICompment/encode/qrspec.h \
    UICompment/encode/rscode.h \
    UICompment/encode/split.h \
    UICompment/encode/tcQrencode.h \
    UICompment/gcodeform.h \
    UICompment/cqcomobox.h \
    UICompment/passwordform.h \
    UICompment/hexloadform.h \
    UICompment/hexloadbase.h \
    UICompment/fileprocessthread.h \
    UICompment/ag_lastform.h \
    Zipunzip/zipunzip.h \
    UICompment/ah_hexform.h \
    UICompment/firmsettingform.h

RESOURCES += \
    icon.qrc \
    translation.qrc

RC_FILE = lp.rc

TRANSLATIONS = c_zh.ts \
               c_en.ts
