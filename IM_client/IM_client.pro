
QT      += core gui
QT      += widgets
QT      += network

TARGET = IM_client
TEMPLATE = app

#LIBS +=

SOURCES += \
    ../comm/im.pb.cc \
    chatwindow.cpp \
    client.cpp \
    file.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    ../comm/im.pb.h \
    chatwindow.h \
    client.h \
    file.h \
    login.h \
    mainwindow.h \
    msg.h

FORMS += \
    chatwindow.ui \
    login.ui \
    mainwindow.ui

INCLUDEPATH += $$PWD/../comm

win32: LIBS += -L$$PWD/../comm/protobuf/lib/ -lprotobuf
win32: INCLUDEPATH += $$PWD/../comm/protobuf/include
win32: DEPENDPATH += $$PWD/../comm/protobuf/include
