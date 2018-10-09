#include "classes.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSqlError>

CUser::CUser(Database &db) :
    m_db(db)
{
}

bool CUser::checkPassword(QString password)
{
    if (!m_db.db.open())
    {
        QMessageBox::critical(0, "SQL error", "Connection error\n" + m_db.db.lastError().databaseText());
        return false;
    }

    QCryptographicHash h(QCryptographicHash::Md5);
    password = h.hash(password.toLatin1(), QCryptographicHash::Md5).toBase64();

    QSqlQuery *q = m_db.QuerySelect("SELECT EMPLOYEE_ID, ROLE_ID FROM LOGINS WHERE PASSWORD=:PASSWORD",
                                    QStringList() << "PASSWORD",
                                    QVector<QVariant>() << password);

    if (!q->next())
    {
        delete q;
        m_db.db.close();
        return false;
    }

    Id = q->value(0).toInt();
    RoleId = q->value(1).toInt();
    delete q;

    q = m_db.QuerySelect("SELECT NAME FROM EMPLOYEES WHERE ID=:ID",
                         QStringList() << "ID",
                         QVector<QVariant>() << Id);
    if (!q->next())
    {
        delete q;
        m_db.db.close();
        return false;
    }

    Name = q->value(0).toString();
    delete q;

    q = m_db.QuerySelect("SELECT ACTION_ID FROM ROLES_SET WHERE ROLE_ID=:ROLE_ID",
                         QStringList() << "ROLE_ID",
                         QVector<QVariant>() << RoleId);
    m_roles.clear();
    while (q->next())
        m_roles.push_back(q->value(0).toInt());
    delete q;

    m_db.db.close();
    return true;
}

CUser &CUser::operator =(const CUser &user)
{
    Id = user.Id;
    RoleId = user.RoleId;
    Name = user.Name;
    m_db = user.m_db;
    m_roles = user.m_roles;
    return *this;
}

