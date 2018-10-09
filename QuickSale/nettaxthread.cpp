#include "nettaxthread.h"
#include "data.h"
#include "printtaxn.h"
#include <winsock2.h>
#include <QMutex>

QMutex ml;

NetTaxThread::NetTaxThread(int socket) :
    QThread(0)
{
    fSocket = socket;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void NetTaxThread::run()
{
    qint32 bytesRead;
    QByteArray dataBuff;
    qint32 buffSize = 1024;
    qint32 dataSize;
    qint32 totalDataSize;
    char tempbuff[buffSize];
    int dataSizeRead = recv(fSocket, reinterpret_cast<char *>(&dataSize), sizeof(qint32), 0);
    if (dataSizeRead <= 0) {
        quit();
        return;
    }
    totalDataSize = dataSize;
    while ((dataSize > 0) && ((bytesRead = recv(fSocket, tempbuff, buffSize, 0)) > 0)) {
        dataBuff.append(tempbuff, bytesRead);
        dataSize -= bytesRead;
    }

    QString firm, hvhh, fiscal, num;
    if (totalDataSize == dataBuff.size()) {
        QMutexLocker m(&ml);
        QJsonDocument jdoc = QJsonDocument::fromJson(dataBuff);
        QJsonObject jobj = jdoc.object();
        Database db;
        EConnection::setConnectionParams(db.db, CD);
        QSqlQuery *q = db.QuerySelect("select sb.dish_id, d.name, sb.qty, sb.price \
                                      from sale_body sb \
                                      left join dishes d on d.id=sb.dish_id \
                                      where order_id='" + jobj.value("order").toString() + "' ");
        if (q) {
            QSettings s("QuickPoint", "QuickPoint");
            if (s.value("taxip").toString().length() == 0 ) {
                s.setValue("taxip","");
                s.setValue("taxport","");
                s.setValue("taxpass","");
            }
            PrintTaxN pt(s.value("taxip").toString(),
                         s.value("taxport").toInt(),
                         s.value("taxpass").toString());
            while (q->next()) {
                pt.addGoods(s.value("taxdep").toString(),
                        s.value("taxadgt").toString(),
                        q->value(0).toString(),
                        q->value(1).toString(),
                        q->value(3).toDouble(),
                        q->value(2).toDouble());
            }
            QString in, out, err;
            if (pt.makeJsonAndPrint(0, 0, in, out, err) != 0) {
                QFile f("./logs.txt");
                f.open(QIODevice::Append);
                f.write(in.toUtf8());
                f.write("\r\n");
                f.write(err.toUtf8());
                f.write(out.toUtf8());
                f.write("\r\n");
                f.close();
                QMessageBox::critical(0, tr("Tax error"), err+ "<br>" + out);
            } else {
                pt.parseResponse(out, firm, hvhh, fiscal, num);
            }
            delete q;
        } else {

        }
    }
    QJsonObject obj;
    obj["firm"] = firm;
    obj["hvhh"] = hvhh;
    obj["fiscal"] = fiscal;
    obj["num"] = num;
    QByteArray ba = QJsonDocument(obj).toJson();
    dataSize = ba.length();
    send(fSocket, reinterpret_cast<char *>(&dataSize), sizeof(dataSize), 0);
    send(fSocket, ba.data(), ba.length(), 0);
    closesocket(fSocket);
    quit();
}
