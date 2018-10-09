#include <QApplication>
#include "qfacewindow.h"
#include "../All/global.h"
#include "../All/econnection.h"
#include "data.h"
#include "serversocket.h"
#include <QSqlError>
//#include <QStyleFactory>

int main (int argc, char *argv[])
{
    QString appFullPath(argv[0]);
    appFullPath = appFullPath.mid(0, appFullPath.lastIndexOf("\\"));
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(appFullPath);
    QCoreApplication::setLibraryPaths(paths);


    QTranslator t;
    t.load(":/quicksale.qm");

    QApplication a(argc, argv);
    a.installTranslator(&t);

    QFontDatabase fontDb;
    int fontId = fontDb.addApplicationFont(a.applicationDirPath() + "/ahuni.ttf");
    QString fontFamily = fontDb.applicationFontFamilies(fontId).at(0);
    QFont font = fontDb.font(fontFamily, "Normal", 11);
    a.setFont(font);

    Data::InitSettings();
    EConnection::ReadConnection(Data::homeDir + "/sale.con", CD);
    EConnection::setConnectionParams(DB.db, CD);

    if (DB.db.open())
    {
        QSqlQuery *q = DB.QuerySelect("SELECT VALUE_ID, DATA FROM SETTINGS WHERE COMPUTER_NAME=:COMPUTER_NAME",
                                      QStringList() << "COMPUTER_NAME",
                                      QVector<QVariant>() << Data::computerName + "/Sale");
        while (q->next())
            Data::settings.insert(q->value(0).toInt(), q->value(1));
        delete q;
        DB.db.close();

        QFont font(Data::settings[SETTINGS_FONT_NAME].toString(), Data::settings[SETTINGS_FONT_SIZE].toInt());
        font.setBold(true);
        a.setFont(font);
    } else {
        QMessageBox::critical(0, "Error", DB.db.lastError().databaseText() +
                              "<br>" + CD.Database + "<br>" + CD.Password + "<br>" + CD.Username);
    }

    QSettings s("QuickPoint", "QuickPoint");
    if (s.value("taxserver").toInt() == 1) {
        ServerSocket *ss = new ServerSocket();
        ss->start();
    }

 //   a.setStyle(QStyleFactory::create("fusion"));

    QFaceWindow *f = new QFaceWindow();
    f->show();

    return a.exec();
}
