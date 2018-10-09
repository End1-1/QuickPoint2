#include "db.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QDate>

uint Database::m_db_count = 0;

QSqlQuery *Database::QuerySelect(const QString &sql, const QStringList &paramNames, const QVector<QVariant> &paramValues)
{
    if (!db.isOpen())
        db.open();
    QSqlQuery *q =  new QSqlQuery(db);
    if (!q->prepare(sql))
    {
        QMessageBox::critical(0, "SQL error", q->lastError().databaseText());
        db.close();
        return 0;
    }
    for (int i = 0; i < paramNames.count(); i++)
        q->bindValue(":" + paramNames[i], paramValues[i]);

    if (!q->exec())
    {
        qDebug() << lastQuery(q);
        QMessageBox::critical(0, "SQL error", q->lastError().databaseText());        
        db.close();
        return 0;
    }
    qDebug() << lastQuery(q);
    return q;
}

bool Database::QueryInsert(const QString &table, const QStringList &fieldsNames, const QVector<QVariant> &values)
{
    QString sql = "INSERT INTO " + table + "(";

    for (QStringList::const_iterator i = fieldsNames.begin(); i != fieldsNames.end(); i++)
        sql += *i + ",";
    sql = sql.remove(sql.length() - 1, 1) + ") VALUES (";

    for(QStringList::const_iterator i = fieldsNames.begin(); i != fieldsNames.end(); i++)
        sql += ":" + *i + ",";
    sql = sql.remove(sql.length() - 1, 1) + ");";

    if (!db.isOpen())
        db.open();

    QSqlQuery q(db);
    if (!q.prepare(sql))
    {
        QMessageBox::critical(0, "SQL error", q.lastError().databaseText());
        db.close();
        return false;
    }

    for (int i = 0; i < values.count(); i++)
        q.bindValue(":" + fieldsNames[i], values[i]);

    if (!q.exec())
    {
        QMessageBox::critical(0, "SQL error", q.lastError().databaseText());
        qDebug() << lastQuery(&q);
        db.close();
        return false;
    }

    qDebug() << lastQuery(&q);
    db.close();
    return true;
}

bool Database::QueryUpdate(const QString &table, const QStringList &fieldsNames, const QVector<QVariant> &values, const QString &condField, const QVariant &condValue)
{
    QString sql = "UPDATE " + table + " SET ";

    for (int i = 0; i < fieldsNames.count(); i++)
        sql += fieldsNames[i] + "=:" + fieldsNames[i] + ",";
    sql = sql.remove(sql.length() - 1, 1);
    sql += " WHERE " + condField + "=:" + condField;

    if (!db.isOpen())
        db.open();

    QSqlQuery q(db);
    if (!q.prepare(sql))
    {
        QMessageBox::critical(0, "SQL error", q.lastError().databaseText());
        db.close();
        return false;
    }

    for (int i = 0; i < fieldsNames.count(); i++)
        q.bindValue(":" + fieldsNames[i], values[i]);
    q.bindValue(":" + condField, condValue);

    if (!q.exec())
    {
        QMessageBox::critical(0, "SQL error", q.lastError().databaseText());
        qDebug() << lastQuery(&q);
        db.close();
        return false;
    }
    qDebug() << lastQuery(&q);
    db.close();
    return true;
}

int Database::GetID(const QString &genName)
{
    QSqlQuery *q = QuerySelect("SELECT GEN_ID(" + genName + ", 1) FROM RDB$DATABASE", QStringList(), QVector<QVariant>());
    q->next();
    int result = q->value(0).toInt();
    delete q;
    db.close();
    return result;
}

void Database::commit()
{
    db.commit();
}

Database::Database()
{
    m_db_number = ++m_db_count;
    db = QSqlDatabase::addDatabase("QIBASE", QString::number(m_db_number));
}

Database::~Database()
{
}

QString Database::lastQuery(QSqlQuery *q)
{
    QString sql = q->lastQuery();
    QMapIterator<QString, QVariant> it(q->boundValues());
    while (it.hasNext()) {
        it.next();
        QVariant value = it.value();
        switch (it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;
        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString("dd.MM.yyyy"));
            break;
        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString("dd.MM.yyyy HH:mm:ss"));
            break;
        case QVariant::Double:
            value = QString("%1").arg(value.toDouble());
            break;
        case QVariant::Int:
            value = QString("%1").arg(value.toInt());
            break;
        default:
            break;
        }
        sql.replace(it.key(), value.toString());
    }
    return sql;
}


QSqlQuery *Database::QuerySelect(const QString &sql)
{
    return QuerySelect(sql, QStringList(), QVector<QVariant>());
}


void Database::close(QSqlQuery *q)
{
    delete q;
    db.close();
}
