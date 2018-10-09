#ifndef LHALL_H
#define LHALL_H

#include "gridwindow.h"

class EHall;

class LHall : public GridWindow
{
    Q_OBJECT

public:
    LHall(QWidget *parent = 0);

private:
    void getData();
    void slotNew();
    void slotEdit();
    void setData(EHall *h, const int &row);
};

#endif // LHALL_H
