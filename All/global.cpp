#include "global.h"
#include <QFile>
#include <QDir>
#include <windows.h>

QString Data::connectionsDir;
QString Data::homeDir;
QMap<int, QVariant> Data::settings;
int Data::databaseIndex;
QString Data::computerName;

void Data::InitSettings()
{
    Data::homeDir = QDir::homePath() + "/QuickPoint";
    Data::connectionsDir = Data::homeDir + "/Connections";

    QDir dir;
    if (!dir.exists(Data::homeDir))
        dir.mkdir(Data::homeDir);
    if (!dir.exists(Data::connectionsDir))
        dir.mkdir(Data::connectionsDir);

    ulong buff_size = 255;
    wchar_t *buff = new wchar_t[buff_size];
    memset(buff, 0, 255);
    GetComputerName(buff, &buff_size);
    Data::computerName = QString::fromWCharArray(buff, buff_size);
}

QDate Data::cashDate()
{
    if (QTime::currentTime() < QTime::fromString(Data::settings[SETTINGS_AUTO_DATE_CHANGE_TIME].toString(), Data::settings[SETTINGS_TIME_FORMAT].toString()))
        return QDate::currentDate().addDays(-1);
    else
        return QDate::currentDate();
}
