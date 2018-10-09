#ifndef RSTOREBALANCE_H
#define RSTOREBALANCE_H

#include "gridwindow.h"

class RStoreBalance : public GridWindow
{
    Q_OBJECT

public:
    RStoreBalance(QWidget *parent = 0);

private:
    QDate m_date;
    QString m_storeNames;
    QString m_storeIDs;
    bool m_groupStore;
    QString m_goodsGroupsNames;
    QString m_goodsGroupsIDs;
    bool m_groupGoodsGroups;
    QString m_goodsNames;
    QString m_goodsIDs;
    bool m_groupGoods;

    void getData();
    void slotFilter();
    void slotTotal();
};

#endif // RSTOREBALANCE_H
