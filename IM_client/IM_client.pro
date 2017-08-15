#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T11:46:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IM_client
TEMPLATE = app

LIBS += -lpthread

SOURCES += \
    connectserver.cpp \
    file.cpp \
    interface.cpp \
    login.cpp \
    main.cpp \
    recvmsgthread.cpp \
    register.cpp

HEADERS  += \
    connectserver.h \
    file.h \
    interface.h \
    login.h \
    msg.h \
    recvmsgthread.h \
    register.h

FORMS += \
    register.ui \
    login.ui \
    interface.ui
