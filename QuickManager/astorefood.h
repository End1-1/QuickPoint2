#ifndef ASTOREFOOD_H
#define ASTOREFOOD_H

#include "editwindow.h"

class AStoreFood : public EditWindow
{
    Q_OBJECT

private:
    bool checkData();

public:
    explicit AStoreFood(const QString &defaultStoreId, const QString &defaultStoreName, QWidget *parent = 0);
    QString m_goodsId;
    QLineEdit *m_pgoodsName;
    QString m_storeId;
    QLineEdit *m_pstoreName;
    QLineEdit *m_pQty;
    QString m_unitId;
    QLineEdit *m_pUnit;

private slots:
    void slotFood();
    void slotStore();
};

#endif // ASTOREFOOD_H
