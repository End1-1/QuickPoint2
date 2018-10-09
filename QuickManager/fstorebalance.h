#ifndef FSTOREBALANCE_H
#define FSTOREBALANCE_H

#include "editwindow.h"

class FStoreBalance : public EditWindow
{
    Q_OBJECT

public:
    FStoreBalance(QWidget *parent = 0);
    QDateEdit *m_pdate;
    QLineEdit *m_pStoreNames;
    QString m_StoreIDs;
    QCheckBox *m_pGroupStore;

    QLineEdit *m_pGroupGoodsNames;
    QString m_GroupGoodsIDs;
    QCheckBox *m_pGroupGoodsGroup;

    QLineEdit *m_pGoodsNames;
    QString m_GoodsIDs;
    QCheckBox *m_pGroupGoods;


private:
    bool checkData();

private slots:
    void slotStore();
    void slotGoodsGroups();
    void slotGoods();
};

#endif // FSTOREBALANCE_H
