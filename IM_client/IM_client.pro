
QT      += core gui
QT      += widgets
QT      += network

TARGET = IM_client
TEMPLATE = app

#LIBS +=

SOURCES += \
    ../comm/im.pb.cc \
    connectserver.cpp \
    file.cpp \
    login.cpp \
    main.cpp \
    recvmsgthread.cpp \
    interface.cpp

HEADERS  += \
    ../comm/im.pb.h \
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

INCLUDEPATH += $$PWD/../comm

win32: LIBS += -L$$PWD/../comm/protobuf/lib/ -lprotobuf
win32: INCLUDEPATH += $$PWD/../comm/protobuf/include
win32: DEPENDPATH += $$PWD/../comm/protobuf/include
