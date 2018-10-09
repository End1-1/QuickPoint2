#ifndef CLASSES_H
#define CLASSES_H

#include "db.h"

class CUser {
    Database &m_db;

public:
    CUser(Database &db);
    int Id;
    int RoleId;
    QString Name;
    QVector<int> m_roles;

    bool checkPassword(QString password);
    CUser &operator =(const CUser &user);
};

#endif // CLASSES_H
