#include "egoodsfordoc.h"
#include "tablemodel.h"
#include "checklist2.h"
#include "global.h"
#include "../All/econnection.h"

EGoodsForDoc::EGoodsForDoc(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Food"));
    next = false;
    int i = -1;
    QGridLayout *l = new QGridLayout();
    l->setColumnMinimumWidth(1, 250);

    l->addWidget(new QLabel(tr("Code")), ++i, 0);
    l->addWidget(m_peFoodId = new QLineEdit(), i, 1);
    connect(m_peFoodId, SIGNAL(editingFinished()), SLOT(slotFoodByCode()));
    QPushButton *pbFood = new QPushButton("...");
    pbFood->setMaximumSize(25, 25);
    connect(pbFood, SIGNAL(clicked()), SLOT(slotFood()));
    l->addWidget(pbFood, i, 2);
    //setTabOrder(m_peFoodId, m_peQty);

    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(m_peFoodName = new QLineEdit(), i, 1);
    m_peFoodName->setEnabled(false);

    l->addWidget(new QLabel(tr("Quantity")), ++i, 0);
    l->addWidget(m_peQty = new QLineEdit());
    m_peQty->setValidator(new QDoubleValidator());
    connect(m_peQty, SIGNAL(editingFinished()), SLOT(slotQty()));

    l->addWidget(new QLabel(tr("Unit")), ++i, 0);
    l->addWidget(m_peUnit = new QLineEdit(), i, 1);
    m_peUnit->setEnabled(false);

    l->addWidget(new QLabel(tr("Price")), ++i, 0);
    l->addWidget(m_pePrice = new QLineEdit(), i, 1);
    m_pePrice->setValidator(new QDoubleValidator());    
    connect(m_pePrice, SIGNAL(editingFinished()), SLOT(slotPrice()));

    l->addWidget(new QLabel(tr("Amount")), ++i, 0);
    l->addWidget(m_peAmount = new QLineEdit(), i, 1);
    m_peAmount->setValidator(new QDoubleValidator());
    connect(m_peAmount, SIGNAL(editingFinished()), SLOT(slotAmount()));

    m_pbNext = new QPushButton(tr("Next"));
    QPushButton *pbOk = new QPushButton(tr("Ok"));
    QPushButton *pbCancel = new QPushButton(tr("Cancel"));
    connect(m_pbNext, SIGNAL(clicked()), SLOT(slotNext()));
    connect(pbOk, SIGNAL(clicked()), SLOT(slotOk()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(slotCancel()));

    QHBoxLayout *l1 = new QHBoxLayout();
    l1->addStretch(1);
    l1->addWidget(m_pbNext);
    l1->addWidget(pbOk);
    l1->addWidget(pbCancel);
    l1->addStretch(1);
    QWidget *w1 = new QWidget();
    w1->setLayout(l1);

    l->addWidget(w1, ++i, 0, 1, 3);
    setLayout(l);
}

EGoodsForDoc::~EGoodsForDoc()
{
    delete m_peFoodId;
    delete m_peFoodName;
    delete m_peUnit;
    delete m_peQty;
    delete m_pePrice;
    delete m_peAmount;
    delete m_pbNext;
}

bool EGoodsForDoc::checkData()
{
    QString msg = "";
    bool result = true;

    if (m_peFoodName->text().length() == 0)
    {
        msg += tr("The name is not selected\n");
        result = false;
    }

    if (m_peQty->text().toDouble() <= 0)
    {
        msg += tr("The quantity must be greater than 0\n");
        result = false;
    }

    /*
    if (m_pePrice->text().toDouble() <= 0)
    {
        msg += tr("The price must be greater than 0\n");
        result = false;
    }
    */

    if (!result)
    {
        QMessageBox::critical(this, tr("Error"), msg);
    }

    return result;
}

void EGoodsForDoc::slotFood()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT G.ID, G.NAME, U.NAME AS UNAME, G.UNIT_ID "
                "FROM GOODS G, UNITS U "
                "WHERE G.UNIT_ID=U.ID "
                 "ORDER BY G.NAME ",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peFoodId->clear();
        m_peFoodName->clear();
        if (c->m_result.count() > 0)
        {
            m_peFoodName->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
            m_peFoodId->setText(c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString());
            m_peUnit->setText(c->m_pt->m_model.data("UNAME", c->m_result.keys().at(0)).toString());
            m_unit = c->m_pt->m_model.data("UNIT_ID", c->m_result.keys().at(0)).toString();
            m_peQty->setFocus();
        }
    }
    delete c;
}

void EGoodsForDoc::slotFoodByCode()
{
    m_peFoodName->clear();
    m_peUnit->clear();
    m_unit = "";

    Database db;
    EConnection::setConnectionParams(db.db, Data::databaseIndex);
    db.db.open();

    QSqlQuery q(db.db);

    if (q.prepare("SELECT G.NAME, U.NAME, G.UNIT_ID FROM GOODS G, UNITS U WHERE G.UNIT_ID=U.ID AND G.ID=:ID"))
    {
        q.bindValue(":ID", m_peFoodId->text());
        q.exec();
        if (q.next())
        {
            m_peFoodName->setText(q.value(0).toString());
            m_peUnit->setText(q.value(1).toString());
            m_unit = q.value(2).toString();
            m_peQty->setFocus();
        }
    }

    db.db.close();
}

void EGoodsForDoc::slotPrice()
{
    m_peAmount->setText(QString::number(m_peQty->text().toDouble() * m_pePrice->text().toDouble()));
    slotReturnPrice();
}

void EGoodsForDoc::slotQty()
{
    m_peAmount->setText(QString::number(m_peQty->text().toDouble() * m_pePrice->text().toDouble()));
    slotReturnQty();
}

void EGoodsForDoc::slotAmount()
{
    double qty = m_peQty->text().toDouble();
    if (qty == 0)
        return;

    m_pePrice->setText(QString::number(m_peAmount->text().toDouble() / qty));
    slotReturnAmount();
}

void EGoodsForDoc::slotNext()
{
    if (!checkData())
        return;

    next = true;
    accept();
}

void EGoodsForDoc::slotOk()
{
    if (!checkData())
        return;

    next = false;
    accept();
}

void EGoodsForDoc::slotCancel()
{
    next = false;
    reject();
}

void EGoodsForDoc::slotReturnQty()
{
    m_pePrice->setFocus();
}

void EGoodsForDoc::slotReturnPrice()
{
    m_peAmount->setFocus();
}

void EGoodsForDoc::slotReturnAmount()
{
    m_pbNext->setFocus();
}

void EGoodsForDoc::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Escape:
        return;
    default:
        QDialog::keyPressEvent(e);
        break;
    }
}
