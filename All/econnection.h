#ifndef ECONNECTION_H
#define ECONNECTION_H

#include "global.h"
#include <QtGui>
#include <QSqlDatabase>
#include <QLineEdit>

class EConnection : public QDialog
{
    Q_OBJECT

public:
    struct ConnectionData {
        char Alias [128];
        char Database [128];
        char Username [128];
        char Password [128];
        char SettingsPassword[128];
        ConnectionData(){memset(this, 0, sizeof(ConnectionData)); }
    };

    EConnection(QWidget *parent = 0);
    QLineEdit *m_alias;
    QLineEdit *m_path;
    QLineEdit *m_username;
    QLineEdit *m_password;
    QLineEdit *m_settingsPassword;
    void getConnection(ConnectionData &data);
    void setConnection(ConnectionData &data);

    static QVector<ConnectionData> m_connections;
    static void WriteConnection(const QString &fileName, ConnectionData connection);
    static void DeleteConnection(ConnectionData connection);
    static void ReadConnections();
    static void ReadConnection(const QString &fileName, ConnectionData &data);
    static bool CheckConnectionPassword(const QString &connectionFile, const QString &password);

    static void setConnectionParams(QSqlDatabase &db, const int &index);
    static void setConnectionParams(QSqlDatabase &db, const ConnectionData &data);

private:
    bool checkData();

private slots:
    void slotOk();
};

#endif // ECONNECTION_H
