
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IM_client
TEMPLATE = app

LIBS += -lpthread

SOURCES += \
    connectserver.cpp \
    file.cpp \
    login.cpp \
    main.cpp \
    recvmsgthread.cpp \
    interface.cpp

HEADERS  += \
    connectserver.h \
    file.h \
    login.h \
    msg.h \
    recvmsgthread.h \
    userinfo.h \
    interface.h

FORMS += \
    login.ui \
    interface.ui
