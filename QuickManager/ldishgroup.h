#ifndef LDISHGROUP_H
#define LDISHGROUP_H

#include "gridwindow.h"

class EDishGroup;

class LDishGroup : public GridWindow
{
    Q_OBJECT

public:
    LDishGroup(QWidget *parent = 0);

private:
    void getData();
    void setData(EDishGroup *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LDISHGROUP_H
