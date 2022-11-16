QT       += core gui sql network

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

INCLUDEPATH += /home/young/libcurl/include/  \
                /home/young/libjson/include/  \
                /home/young/libopenssl/include/

LIBS += -L/home/young/libcurl/lib
LIBS += -L/home/young/libjson/
LIBS += -L/home/young/libopenssl/lib
LIBS +=   -lcurl  -lcrypto  -ljsoncpp -lssl

SOURCES += \
    albumwindow.cpp \
    loginfacewindow.cpp \
    loginicwindow.cpp \
    loginkeyboardwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    monitorwindow.cpp \
    readRfid.c \
    registerfacewindow.cpp \
    registericwindow.cpp \
    registerkeyboardwindow.cpp \
    softkey.cpp \
    surveillancewindow.cpp \
    v4l2api.cpp \
    videowindow.cpp

HEADERS += \
    aip-cpp-sdk-0.8.1/face.h \
    albumwindow.h \
    loginfacewindow.h \
    loginicwindow.h \
    loginkeyboardwindow.h \
    mainwindow.h \
    monitorwindow.h \
    readRfid.h \
    registerfacewindow.h \
    registericwindow.h \
    registerkeyboardwindow.h \
    softkey.h \
    surveillancewindow.h \
    v4l2api.h \
    videowindow.h

FORMS += \
    albumwindow.ui \
    loginfacewindow.ui \
    loginicwindow.ui \
    loginkeyboardwindow.ui \
    mainwindow.ui \
    monitorwindow.ui \
    registerfacewindow.ui \
    registericwindow.ui \
    registerkeyboardwindow.ui \
    surveillancewindow.ui \
    videowindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
