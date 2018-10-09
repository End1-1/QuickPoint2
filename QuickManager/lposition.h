#ifndef LPOSITION_H
#define LPOSITION_H

#include "gridwindow.h"

class EPosition;

class LPosition : public GridWindow
{
    Q_OBJECT

public:
    LPosition();

private:
    void getData();
    void setData(EPosition *e, const int row);
    void slotNew();
    void slotEdit();
    void updateSalary(EPosition *e, QSqlQuery *q);
};

#endif // LPOSITION_H
