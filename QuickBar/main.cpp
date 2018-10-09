#include <QApplication>
#include <QTranslator>
#include "qfacewindow.h"
#include "global.h"
#include "qbasedialog.h"
#include "qsettingsdialog.h"
#include <windows.h>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(new QCleanlooksStyle());
    //Check %userdir%
    QDir dir;
    if (!dir.exists(QDir::homePath() + "//QuickBar"))
        dir.mkdir(QDir::homePath() + "//QuickBar");

    Data::fileConnection = dir.homePath() + "//QuickBar//conn2.dat";
    QFile file(Data::fileConnection);
    if (!file.exists())
    {
        Connection con;
        memset(&con, 0, sizeof(Connection));
        file.open(QIODevice::WriteOnly);
        file.write(reinterpret_cast<const char *>(&con), sizeof(Connection));
        file.close();
    }

    Data::fileLogo = dir.currentPath() + "//logo.png";
    if (!QFile::exists(Data::fileLogo))
        Data::fileLogo = "";

    QSettingsDialog::readConnection();

    ulong buff_size  = 255;
    char *buff = new char [buff_size];
    GetComputerName(buff, &buff_size);
    Data::computerName = QString::fromAscii(buff, buff_size);

    QEDatabase db;
    db.setDBParams(QSettingsDialog::connection.PathToDb, QSettingsDialog::connection.Username, QSettingsDialog::connection.Password);
    db.open();
    QSqlQuery *q = db.query();
    if (db.prepare(q, "SELECT VALUE_ID, DATA FROM SETTINGS WHERE COMPUTER_NAME=:COMPUTER_NAME"))
    {
        q->bindValue("COMPUTER_NAME", Data::computerName);
        db.exec(q);
        while (q->next())
            Data::settings[q->value(0).toInt()] = q->value(1);
    }

    QString sql = "SELECT H.ID AS HALL_ID, H.NAME AS HALL_NAME, T.ID AS TABLE_ID, T.NAME AS TABLE_NAME, T.QUEUE "
                    "FROM HALL H, HALL_TABLES T "
                    "WHERE H.ID=T.HALL_ID "
                    "ORDER BY H.ID, T.QUEUE";

    if (db.exec(q, sql))
        Data::initHall(q);

    Data::initMenu(&db);

    delete q;
    db.close();

    QTranslator t;
    t.load(":/quickbar.qm");
    QFont font(Data::settings[S_FONT_NAME].toString(), Data::settings[S_FONT_SIZE].toInt());
    app.installTranslator(&t);
    app.setFont(font);

    QFaceWindow *f = new QFaceWindow();
    f->show();
    return app.exec();
}
