#ifndef MPTCPSOCKET_H
#define MPTCPSOCKET_H

#include <QTcpSocket>

class MPTcpSocket : public QObject
{
    Q_OBJECT
public:
    MPTcpSocket(QObject *parent = 0);
    void setServerIP(const QString &ip);
    void setValue(const QString &key, const QVariant &value);
    QJsonObject sendData();
private:
    QMap<QString, QVariant> fData;
    QString fServerIP;
signals:
    void handleJson(const QJsonObject &obj);
};

#endif // MPTCPSOCKET_H
