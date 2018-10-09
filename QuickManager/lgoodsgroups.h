#ifndef LGOODSGROUPS_H
#define LGOODSGROUPS_H

#include "gridwindow.h"

class EGoodsGroup;

class LGoodsGroups : public GridWindow
{
    Q_OBJECT

public:
    LGoodsGroups(QWidget *parent = 0);

private:
    void getData();
    void setData(EGoodsGroup *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LGOODSGROUPS_H
