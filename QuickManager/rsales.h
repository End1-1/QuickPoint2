#ifndef RSALES_H
#define RSALES_H

#include "gridwindow.h"

class RSales : public GridWindow
{
    Q_OBJECT

public:
    RSales(QWidget *parent = 0);

private:
    void getData();

    QString m_orderId;
    bool m_groupOrderId;

    QDate m_dateStart;
    QDate m_dateEnd;
    bool m_groupDate;

    QDateTime m_fullDateStart;
    QDateTime m_fullDateEnd;
    bool m_fullDate;

    QString m_tableName;
    QString m_table;
    bool m_groupTable;

    QString m_waiterName;
    QString m_waiter;
    bool m_groupWaiter;

    QString m_dishGroupName;
    QString m_dishGroup;
    bool m_groupDishGroup;

    QString m_dishName;
    QString m_dish;
    bool m_groupDish;

    bool view_order_mode;

    struct GoodsOutput {
        QString sale_id;
        int store_id;
        int dish_id;
        int goods_id;
        double qty;
        double price;
    };

private slots:
    void slotFilter();
    void slotTotal();
    void slotDelete();
    void slotPrint();
    void slotViewLog();
    void slotEdit();
    void slotCountFoodOutpup();
};

#endif // RSALES_H
