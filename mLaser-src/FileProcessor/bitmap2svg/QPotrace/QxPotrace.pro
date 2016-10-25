QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QxPotrace
TEMPLATE = lib
DESTDIR = $$PWD/bin
DEFINES += QXPOTRACE_LIBRARY

SOURCES += \
  $$PWD/src/qxpotrace.cpp \
  $$PWD/src/potrace/trans.c \
  $$PWD/src/potrace/trace.c \
  $$PWD/src/potrace/progress_bar.c \
  $$PWD/src/potrace/potracelib.c \
  $$PWD/src/potrace/decompose.c \
  $$PWD/src/potrace/curve.c \
  $$PWD/src/potrace/bbox.c

HEADERS  += \
  $$PWD/include/QxPotrace \
  $$PWD/include/qxpotrace.h \
  $$PWD/include/qxpotrace_global.h \
  $$PWD/src/potrace/trans.h \
  $$PWD/src/potrace/trace.h \
  $$PWD/src/potrace/progress_bar.h \
  $$PWD/src/potrace/progress.h \
  $$PWD/src/potrace/potracelib.h \
  $$PWD/src/potrace/main.h \
  $$PWD/src/potrace/lists.h \
  $$PWD/src/potrace/decompose.h \
  $$PWD/src/potrace/curve.h \
  $$PWD/src/potrace/bitmap.h \
  $$PWD/src/potrace/bbox.h \
  $$PWD/src/potrace/auxiliary.h

INCLUDEPATH += $$PWD/include

DEFINES += POTRACE=\"\\\"potrace\\\"\"
DEFINES += MKBITMAP=\"\\\"mkbitmap\\\"\"
DEFINES += VERSION=\"\\\"1.10\\\"\"
DEFINES += PACKAGE=\"\\\"potrace\\\"\"
