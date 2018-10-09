#include "astorefood.h"
#include "checklist2.h"

AStoreFood::AStoreFood(const QString &defaultStoreId, const QString &defaultStoreName, QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Food"));
    QGridLayout *l = new QGridLayout(this);
    int i = -1;

    QPushButton *pbFood = newBtn(this);
    connect(pbFood, SIGNAL(clicked()), SLOT(slotFood()));
    l->addWidget(new QLabel(tr("Food")), ++i, 0);
    l->addWidget(m_pgoodsName = new QLineEdit(), i, 1);
    l->addWidget(pbFood, i, 2);
    m_pgoodsName->setReadOnly(true);

    l->addWidget(new QLabel(tr("Qty")), ++i, 0);
    l->addWidget(m_pQty = new QLineEdit(), i, 1);
    m_pQty->setValidator(new QDoubleValidator(this));

    l->addWidget(new QLabel(tr("Unit")), ++i, 0);
    l->addWidget(m_pUnit = new QLineEdit(), i, 1);
    m_pUnit->setReadOnly(true);

    QPushButton *pbStore = newBtn(this);
    connect(pbStore, SIGNAL(clicked()), SLOT(slotStore()));
    l->addWidget(new QLabel(tr("Store")), ++i, 0);
    l->addWidget(m_pstoreName = new QLineEdit());
    l->addWidget(pbStore, i, 2);
    m_pstoreName->setReadOnly(true);
    m_pstoreName->setText(defaultStoreName);
    m_storeId = defaultStoreId;

    l->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(l);
}

void AStoreFood::slotFood()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT G.ID, G.NAME, G.UNIT_ID, U.NAME AS UNAME "
                "FROM GOODS G, UNITS U "
                "WHERE G.UNIT_ID=U.ID "
                 "ORDER BY G.NAME",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_goodsId.clear();
        m_pgoodsName->clear();
        m_unitId.clear();
        m_pUnit->clear();
        if (c->m_result.count() > 0)
        {
            int row_result = c->m_result.keys().at(0);
            m_goodsId = c->m_pt->m_model.String("ID", row_result);
            m_pgoodsName->setText(c->m_pt->m_model.String("NAME", row_result));
            m_unitId = c->m_pt->m_model.String("UNIT_ID", row_result);
            m_pUnit->setText(c->m_pt->m_model.String("UNAME", row_result));
            m_pQty->setText("0");
            m_pQty->setSelection(0, 2);
            m_pQty->setFocus();
        }
    }
    delete c;
}

void AStoreFood::slotStore()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        if (c->m_result.count() > 0)
        {
            int row = c->m_result.keys().at(0);
            m_storeId = c->m_pt->m_model.String("ID", row);
            m_pstoreName->setText(c->m_pt->m_model.String("NAME", row));
        }
    }

    delete c;
}


bool AStoreFood::checkData()
{
    QString msg;
    bool result = true;

    if (m_goodsId.length() == 0)
    {
        msg += tr("The food is not selected\n");
        result = false;
    }

    if (m_pQty->text().toDouble() <= 0)
    {
        msg += tr("Invalid quantity of food\n");
        result = false;
    }

    if (m_storeId.length() == 0)
    {
        msg += tr("The store is not selected\n");
        result = false;
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}
