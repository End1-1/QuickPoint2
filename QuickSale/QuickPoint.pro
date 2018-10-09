QT += core gui sql printsupport network

TARGET = QuickPoint
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    kbd.cpp \
    print.cpp \
    ComPort.cpp \
    ../QuickManager/about.cpp \
    digits.cpp \
    ../All/global.cpp \
    ../All/db.cpp \
    ../All/qnumpadwidget.cpp \
    ../All/econnection.cpp \
    ../All/qprint.cpp \
    qfacewindow.cpp \
    qmessagedialog.cpp \
    data.cpp \
    ../All/classes.cpp \
    change.cpp \
    dlgreadcard.cpp \
    ../../NewTax/printtaxn.cpp \
    serversocket.cpp \
    nettaxthread.cpp \
    mptcpsocket.cpp

HEADERS += \
    mainwidget.h \
    kbd.h \
    print.h \
    ComPort.h \
    ../QuickManager/about.h \
    digits.h \
    ../All/global.h \
    ../All/db.h \
    ../All/qnumpadwidget.h \
    ../All/econnection.h \
    ../All/qprint.h \
    qfacewindow.h \
    qmessagedialog.h \
    data.h \
    classes.h \
    ../All/classes.h \
    change.h \
    dlgreadcard.h \
    ../../NewTax/printtaxn.h \
    serversocket.h \
    nettaxthread.h \
    mptcpsocket.h

RESOURCES += res.qrc

ICON = icons/app.ico

RC_FILE = res.rc

DEFINES += QPOINT

VERSION = 1.0

FORMS += \
    dlgreadcard.ui

LIBS += -lwsock32
LIBS += -LC:/OpenSSL-Win32/lib
LIBS += -lopenssl
LIBS += -llibcrypto
LIBS += -lVersion

INCLUDEPATH += C:/Qt/projects/NewTax
INCLUDEPATH += C:/OpenSSL-Win32.0/include
INCLUDEPATH += C:/OpenSSL-Win32.0/include/openssl
