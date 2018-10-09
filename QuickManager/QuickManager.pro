QT += core gui sql printsupport

TARGET = QuickManager

TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tablemodel.cpp \
    gridwindow.cpp \
    lunits.cpp \
    editwindow.cpp \
    eunit.cpp \
    lmenu.cpp \
    emenu.cpp \
    ldishgroup.cpp \
    edishgroup.cpp \
    ldishes.cpp \
    lstorage.cpp \
    estorage.cpp \
    edish.cpp \
    checklist.cpp \
    fdish.cpp \
    lposition.cpp \
    eposition.cpp \
    erole.cpp \
    lroles.cpp \
    eemployee.cpp \
    lemployee.cpp \
    femployee.cpp \
    lgoodsgroups.cpp \
    egoodsgroup.cpp \
    eprinter.cpp \
    lprinter.cpp \
    egoods.cpp \
    lgoods.cpp \
    erecipe.cpp \
    checklist2.cpp \
    erecipeqty.cpp \
    rsales.cpp \
    fsale.cpp \
    rstore.cpp \
    estoredoc.cpp \
    egoodsfordoc.cpp \
    fstoredoc.cpp \
    rfinalreport.cpp \
    fsaleout.cpp \
    rlog.cpp \
    about.cpp \
    ecomboitem.cpp \
    emodifier.cpp \
    lmodifiers.cpp \
    amodifiers.cpp \
    echangepassword.cpp \
    astorefood.cpp \
    esalaryformula.cpp \
    rsalary.cpp \
    esalarydoc.cpp \
    aemployeesalary.cpp \
    fsalary.cpp \
    lcash.cpp \
    fcalcout.cpp \
    lhall.cpp \
    ehall.cpp \
    ltables.cpp \
    etables.cpp \
    lprintalieases.cpp \
    eprintaliases.cpp \
    eprinteraliasrealpair.cpp \
    lprinterschema.cpp \
    eprinterschema.cpp \
    lsettings.cpp \
    esettings.cpp \
    simplepassword.cpp \
    rstorebalance.cpp \
    fstorebalance.cpp \
    ../All/global.cpp \
    ../All/econnection.cpp \
    login.cpp \
    data.cpp \
    ../All/db.cpp \
    ../All/classes.cpp \
    ../All/qprint.cpp \
    esaleorder.cpp \
    egroupchange.cpp \
    dishes2.cpp \
    gridwindow2.cpp \
    rcardappend.cpp \
    ecardappend.cpp \
    edialog.cpp \
    dlgnodiscountdishes.cpp \
    rcardbyuse.cpp \
    fdaterange.cpp \
    rcardstatistic.cpp \
    defines.cpp \
    filtercardstat.cpp

HEADERS += \
    mainwindow.h \
    tablemodel.h \
    gridwindow.h \
    lunits.h \
    editwindow.h \
    eunit.h \
    lmenu.h \
    emenu.h \
    ldishgroup.h \
    edishgroup.h \
    ldishes.h \
    lstorage.h \
    estorage.h \
    edish.h \
    checklist.h \
    fdish.h \
    lposition.h \
    eposition.h \
    erole.h \
    lroles.h \
    eemployee.h \
    lemployee.h \
    femployee.h \
    lgoodsgroups.h \
    egoodsgroup.h \
    eprinter.h \
    lprinter.h \
    egoods.h \
    lgoods.h \
    erecipe.h \
    checklist2.h \
    erecipeqty.h \
    rsales.h \
    fsale.h \
    rstore.h \
    estoredoc.h \
    egoodsfordoc.h \
    fstoredoc.h \
    rfinalreport.h \
    fsaleout.h \
    rlog.h \
    about.h \
    ecomboitem.h \
    emodifier.h \
    lmodifiers.h \
    amodifiers.h \
    echangepassword.h \
    astorefood.h \
    esalaryformula.h \
    rsalary.h \
    esalarydoc.h \
    aemployeesalary.h \
    fsalary.h \
    lcash.h \
    fcalcout.h \
    lhall.h \
    ehall.h \
    ltables.h \
    etables.h \
    lprintalieases.h \
    eprintaliases.h \
    eprinteraliasrealpair.h \
    lprinterschema.h \
    eprinterschema.h \
    lsettings.h \
    esettings.h \
    global.h \
    simplepassword.h \
    rstorebalance.h \
    fstorebalance.h \
    qprint.h \
    ../All/econnection.h \
    login.h \
    data.h \
    ../All/db.h \
    ../All/classes.h \
    ../All/qprint.h \
    esaleorder.h \
    egroupchange.h \
    dishes2.h \
    gridwindow2.h \
    rcardappend.h \
    ecardappend.h \
    edialog.h \
    dlgnodiscountdishes.h \
    rcardbyuse.h \
    fdaterange.h \
    rcardstatistic.h \
    defines.h \
    filtercardstat.h

RESOURCES = res.qrc

RC_FILE = res.rc

ICON = icons/app.ico

CONFIG += windows qt plugin

FORMS += \
    ecardappend.ui \
    dlgnodiscountdishes.ui \
    fdaterange.ui \
    filtercardstat.ui
