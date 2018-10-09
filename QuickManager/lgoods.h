#ifndef LGOODS_H
#define LGOODS_H

#include "gridwindow.h"

class EGoods;

class LGoods : public GridWindow
{
    Q_OBJECT

public:
    LGoods(QWidget *parent = 0);

private:
    void getData();
    void setData(EGoods *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LGOODS_H
