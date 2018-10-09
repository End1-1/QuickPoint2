#ifndef DB_H
#define DB_H

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVector>

class Database
{
private:
    uint m_db_number;
    static uint m_db_count;

public:
    QSqlDatabase db;
    QSqlQuery *QuerySelect(const QString &sql, const QStringList &paramNames, const QVector<QVariant> &paramValues);
    QSqlQuery *QuerySelect(const QString &sql);
    void close(QSqlQuery *q);
    bool QueryInsert(const QString &table, const QStringList &fieldsNames, const QVector<QVariant> &values);
    bool QueryUpdate(const QString &table, const QStringList &fieldsNames, const QVector<QVariant> &values, const QString &condField, const QVariant &condValue);
    int GetID(const QString &genName);
    void commit();
    Database();
    ~Database();
    static QString lastQuery(QSqlQuery *q);
};

#endif
