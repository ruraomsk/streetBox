QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    project/cross.cpp \
    project/formgetdbcross.cpp \
    project/formgetemptycross.cpp \
    project/project.cpp \
    project/readcsvfile.cpp \
    project/setcsvdata.cpp \
    project/setdbdata.cpp \
    project/setemptydata.cpp \
    project/sethourdata.cpp \
    project/viewcross.cpp \
    project/viewpro.cpp \
    setup.cpp \
    stat/archstat.cpp \
    support.cpp \
    viewsetup.cpp \
    xctrl/bogko.cpp \
    xctrl/dialogaddemptyxt.cpp \
    xctrl/readjsonfile.cpp \
    xctrl/sumgraph.cpp \
    xctrl/viewcalculate.cpp \
    xctrl/viewpoints.cpp \
    xctrl/viewstrategy.cpp \
    xctrl/viewvoronoi.cpp \
    xctrl/viewxctrl.cpp \
    xctrl/voronoi.cpp \
    xctrl/xctrl.cpp \
    xctrl/xtcalculate.cpp

HEADERS += \
    mainwindow.h \
    project/cross.h \
    project/formgetdbcross.h \
    project/formgetemptycross.h \
    project/project.h \
    project/readcsvfile.h \
    project/setcsvdata.h \
    project/setdbdata.h \
    project/setemptydata.h \
    project/sethourdata.h \
    project/viewcross.h \
    project/viewpro.h \
    setup.h \
    stat/archstat.h \
    support.h \
    viewsetup.h \
    xctrl/bogko.h \
    xctrl/dialogaddemptyxt.h \
    xctrl/readjsonfile.h \
    xctrl/sumgraph.h \
    xctrl/viewcalculate.h \
    xctrl/viewpoints.h \
    xctrl/viewstrategy.h \
    xctrl/viewvoronoi.h \
    xctrl/viewxctrl.h \
    xctrl/voronoi.h \
    xctrl/xctrl.h \
    xctrl/xtcalculate.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#unix:LIBS += -lpq
#win32:LIBS += libpqdll.lib

DISTFILES += \
    data/rura.prj \
    data/streetBox.ini \
    streetBox.ini

RESOURCES += \
    streetBox.qrc

SUBDIRS += \
    data/pr1.pro \
    data/pr1.pro

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../opt/PostgreSQL/10/lib/release/ -lpq
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../opt/PostgreSQL/10/lib/debug/ -lpq
#unix: LIBS += -L$$PWD/../../../../opt/PostgreSQL/10/lib/ -lpq

#INCLUDEPATH += $$PWD/../../../../opt/PostgreSQL/10/include
#DEPENDPATH += $$PWD/../../../../opt/PostgreSQL/10/include
#unix: PRE_TARGETDEPS += $$PWD/../../../../opt/PostgreSQL/10/lib/libpq.a
