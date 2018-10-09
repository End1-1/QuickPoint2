#ifndef LROLES_H
#define LROLES_H

#include "gridwindow.h"

#define ROLE_EDIT_PARTIAL_DATA 1
#define ROLE_EDIT_ALL_DATA 2
#define ROLE_EDIT_STORE_DOCS 3
#define ROLE_VIEW_SALE_DOCS 4
#define ROLE_DEL_SALE_DOCS 5
#define ROLE_EDIT_SALARY_1 6 //Only with formula
#define ROLE_EDIT_SALARY_2 7 //Free, can input any value
#define ROLE_MAKE_SALE 8

class ERole;

class LRoles : public GridWindow
{
    Q_OBJECT

public:
    LRoles(QWidget *parent = 0);

private:
    void getData();
    void setData(ERole *e, const int row);
    void slotNew();
    void slotEdit();

private slots:
    void slotRolesSet();
};

#endif // LROLES_H
