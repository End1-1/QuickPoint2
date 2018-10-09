#include "fstorebalance.h"
#include "checklist2.h"

FStoreBalance::FStoreBalance(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Store balance report filter"));

    int i = -1;
    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 400);

    plMain->addWidget(new QLabel(tr("Date"), this), ++i, 0);
    plMain->addWidget(m_pdate = new QDateEdit(QDate::currentDate(), this), i, 1, 1, 2);

    plMain->addWidget(new QLabel(tr("Store"), this), ++i, 0);
    plMain->addWidget(m_pStoreNames = new QLineEdit(this), i, 1);
    m_pStoreNames->setEnabled(false);
    plMain->addWidget(m_pGroupStore = new QCheckBox(this), i, 2);
    QPushButton *pbtnStore = newBtn(this);
    plMain->addWidget(pbtnStore, i, 3);

    plMain->addWidget(new QLabel(tr("Goods groups"), this), ++i, 0);
    plMain->addWidget(m_pGroupGoodsNames = new QLineEdit(this), i, 1);
    m_pGroupGoodsNames->setEnabled(false);
    plMain->addWidget(m_pGroupGoodsGroup = new QCheckBox(this), i, 2);
    QPushButton *pbtnGroupGoods = newBtn(this);
    plMain->addWidget(pbtnGroupGoods, i, 3);

    plMain->addWidget(new QLabel(tr("Goods"), this), ++i, 0);
    plMain->addWidget(m_pGoodsNames = new QLineEdit(this), i, 1);
    m_pGoodsNames->setEnabled(false);
    plMain->addWidget(m_pGroupGoods = new QCheckBox(this), i, 2);
    QPushButton *pbtnGoods = newBtn(this);
    plMain->addWidget(pbtnGoods, i, 3);

    plMain->addWidget(m_pwidget, ++i, 0, 1, 4);
    setLayout(plMain);

    connect(pbtnStore, SIGNAL(clicked()), SLOT(slotStore()));
    connect(pbtnGroupGoods, SIGNAL(clicked()), SLOT(slotGoodsGroups()));
    connect(pbtnGoods, SIGNAL(clicked()), SLOT(slotGoods()));
}

bool FStoreBalance::checkData()
{
    return true;
}

void FStoreBalance::slotStore()
{
    CheckList2::Store(m_pStoreNames, m_StoreIDs, true, this);
}

void FStoreBalance::slotGoodsGroups()
{
    CheckList2::GroupGoods(m_pGroupGoodsNames, m_GroupGoodsIDs, true, this);
}

void FStoreBalance::slotGoods()
{
    CheckList2::Goods(m_GroupGoodsIDs, m_pGoodsNames, m_GoodsIDs, true, this);
}
