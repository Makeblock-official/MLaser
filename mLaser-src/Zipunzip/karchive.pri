DEPENDPATH += $$PWD/karchive
INCLUDEPATH += $$PWD/karchive

HEADERS += \
           #$$PWD/karchive/k7zip.h \
           #$$PWD/karchive/kar.h \
           $$PWD/karchive/karchive.h \
           $$PWD/karchive/karchive_p.h \
           $$PWD/karchive/karchivedirectory.h \
           $$PWD/karchive/karchiveentry.h \
           $$PWD/karchive/karchivefile.h \
           #$$PWD/karchive/kbzip2filter.h \
           $$PWD/karchive/kcompressiondevice.h \
           $$PWD/karchive/kfilterbase.h \
           $$PWD/karchive/kfilterdev.h \
           $$PWD/karchive/kgzipfilter.h \
           $$PWD/karchive/klimitediodevice_p.h \
           $$PWD/karchive/knonefilter.h \
           $$PWD/karchive/ktar.h \
           #$$PWD/karchive/kxzfilter.h \
           $$PWD/karchive/kzip.h \
           $$PWD/karchive/kzipfileentry.h

SOURCES += \
           #$$PWD/karchive/k7zip.cpp \
           #$$PWD/karchive/kar.cpp \
           $$PWD/karchive/karchive.cpp \
           #$$PWD/karchive/kbzip2filter.cpp \
           $$PWD/karchive/kcompressiondevice.cpp \
           $$PWD/karchive/kfilterbase.cpp \
           $$PWD/karchive/kfilterdev.cpp \
           $$PWD/karchive/kgzipfilter.cpp \
           $$PWD/karchive/klimitediodevice.cpp \
           $$PWD/karchive/knonefilter.cpp \
           $$PWD/karchive/ktar.cpp \
           #$$PWD/karchive/kxzfilter.cpp \
           $$PWD/karchive/kzip.cpp

win32: LIBS += -ladvapi32

contains(QT_CONFIG, system-zlib) {
    if(unix|win32-g++*):     LIBS_PRIVATE += -lz
    else:                    LIBS += zdll.lib
} else {
    p1 = $$[QT_INSTALL_HEADERS/get]/QtZlib
    p2 = $$[QT_INSTALL_HEADERS/src]/QtZlib
    exists($$p1): INCLUDEPATH += $$p1
    else: INCLUDEPATH += $$p1
}
