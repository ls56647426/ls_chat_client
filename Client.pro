QT       += core gui network

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
    addfriend.cpp \
    find.cpp \
    form/findcard.cpp \
    form/searchlineedit.cpp \
    include/Msg.cpp \
    include/client.cpp \
    login.cpp \
    lschat.cpp \
    main.cpp \
    pojo/Friend.cpp \
    pojo/Group.cpp \
    pojo/User.cpp \
    pojo/UserGroupMap.cpp

HEADERS += \
    addfriend.h \
    find.h \
    form/findcard.h \
    form/searchlineedit.h \
    include/Msg.h \
    include/client.h \
    login.h \
    lschat.h \
    pojo/Friend.h \
    pojo/Group.h \
    pojo/User.h \
    pojo/UserGroupMap.h

FORMS += \
    addfriend.ui \
    find.ui \
    login.ui \
    lschat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# zlib支持
INCLUDEPATH += D:/zlib-1.2.11
LIBS += -LD:/zlib-1.2.11 -lzlib1

RESOURCES += \
    icon.qrc
