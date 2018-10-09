QT += core gui sql

TEMPLATE = app

DEFINES = QBAR

RESOURCES = bar.qrc

RC_FILE = res.rc

SOURCES += \
    main.cpp \
    qbasedialog.cpp \
    qorderdialog.cpp \
    qsettingsdialog.cpp \
    qfacewindow.cpp \
    qmessagedialog.cpp \
    global.cpp \
    qhalldialog.cpp \
    qmenuwindow.cpp \
    qpassworddialog.cpp \
    qmoveorderdialog.cpp \
    qcashdialog.cpp \
    qcostumdishdialog.cpp \
    qlistdialog.cpp \
    ../All/qprint.cpp \
    ../All/qnumpadwidget.cpp

HEADERS += \
    qbasedialog.h \
    qorderdialog.h \
    global.h \
    qsettingsdialog.h \
    qfacewindow.h \
    qmessagedialog.h \
    qhalldialog.h \
    qmenuwindow.h \
    qpassworddialog.h \
    qmoveorderdialog.h \
    qcashdialog.h \
    qcostumdishdialog.h \
    qlistdialog.h \
    ../All/qprint.h \
    ../All/qnumpadwidget.h

ICON = icons/app.ico

RC_FILE = res.rc
