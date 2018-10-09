#include <QApplication>
#include "mainwindow.h"
#include "about.h"
#include "global.h"
#include "simplepassword.h"
#include "../All/econnection.h"

int main (int argc, char *argv[])
{

    Data::InitSettings();

    QTranslator t;
    t.load(":/manager.qm");

    QApplication a(argc, argv);
    a.installTranslator(&t);

    //QFont font(Data::settings[SETTINGS_FONT_NAME].toString(), Data::settings[SETTINGS_FONT_SIZE].toInt());
    QFont font("Sylfaen", 12);
    a.setFont(font);

    EConnection::ReadConnections();

//    QStringList l = QCoreApplication::libraryPaths();
//    for (QStringList::const_iterator i = l.begin(); i != l.end(); i++)
//        QMessageBox::information(0, *i, *i);

    MainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":/icons/app.ico"));
    mainWindow.showMaximized();
    return a.exec();
}
