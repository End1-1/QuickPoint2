#ifndef LUNITS_H
#define LUNITS_H

#include "gridwindow.h"

class EUnit;

class LUnits : public GridWindow
{
    Q_OBJECT

public:
    LUnits(QWidget *parent = 0);

protected:
    void getData();
    void setData(EUnit *e, int row);

public slots:
    void slotNew();
    void slotEdit();
};

#endif // LUNITS_H
