#include "fsale.h"
#include "checklist2.h"

FSale::FSale(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Sale filter"));
    QGridLayout *l = new QGridLayout();
    int i = -1;

    l->addWidget(new QLabel(tr("Order id")), ++i, 0);
    l->addWidget(m_peOrderId = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pchOrderId = new QCheckBox(), i, 3);


    l->addWidget(new QLabel(tr("Date range")), ++i, 0);
    /*
    l->addWidget(m_pd1 = new QDateEdit(QDate::currentDate()), i, 1);
    l->addWidget(m_pd2 = new QDateEdit(QDate::currentDate()), i, 2);
    */
    l->addWidget(m_pchDate = new QCheckBox(), i, 3);

    m_pd1 = new QDateEdit(QDate::currentDate());
    m_pd2 = new QDateEdit(QDate::currentDate());


    l->addWidget(new QLabel(tr("Full date"), this), ++i, 0);
    l->addWidget(m_pfullDate1 = new QDateTimeEdit(QDateTime::currentDateTime()), i, 1);
    l->addWidget(m_pfullDate2 = new QDateTimeEdit(QDateTime::currentDateTime()), i, 2);
    l->addWidget(m_pchFullDate = new QCheckBox(this), i, 3);

    l->addWidget(new QLabel(tr("Table"), this), ++i, 0);
    l->addWidget(m_pTable = new QLineEdit(this), i, 1, 1, 2);
    m_pTable->setEnabled(false);
    l->addWidget(m_pchTable = new QCheckBox(this), i, 3);
    QPushButton *pbTable = new QPushButton("...");
    pbTable->setMaximumSize(25, 25);
    connect(pbTable, SIGNAL(clicked()), SLOT(slotTable()));
    l->addWidget(pbTable, i, 4);

    l->addWidget(new QLabel(tr("Waiter"), this), ++i, 0);
    l->addWidget(m_pWaiter = new QLineEdit(this), i, 1, 1, 2);
    m_pWaiter->setEnabled(false);
    l->addWidget(m_pchWaiter = new QCheckBox(this), i, 3);
    QPushButton *pbWaiter = new QPushButton("...");
    pbWaiter->setMaximumSize(25, 25);
    connect(pbWaiter, SIGNAL(clicked()), SLOT(slotWaiter()));
    l->addWidget(pbWaiter, i, 4);

    l->addWidget(new QLabel(tr("Dishes group")), ++i, 0);
    l->addWidget(m_peDishGroup = new QLineEdit(), i, 1, 1, 2);
    m_peDishGroup->setEnabled(false);
    l->addWidget(m_pchDishGroup = new QCheckBox(), i, 3);
    QPushButton *pbDishGroup = new QPushButton("...");
    pbDishGroup->setMaximumSize(25, 25);
    connect(pbDishGroup, SIGNAL(clicked()), SLOT(slotDishGroup()));
    l->addWidget(pbDishGroup, i, 4);

    l->addWidget(new QLabel(tr("Dish")), ++i, 0);
    l->addWidget(m_peDish = new QLineEdit(), i, 1, 1, 2);
    m_peDish->setEnabled(false);
    l->addWidget(m_pchDish = new QCheckBox(), i, 3);
    QPushButton *pbDish = new QPushButton("...");
    pbDish->setMaximumSize(25, 25);
    connect(pbDish, SIGNAL(clicked()), SLOT(slotDish()));
    l->addWidget(pbDish, i, 4);

    l->addWidget(m_pwidget, ++i, 0, 1, 5);
    setLayout(l);
}

bool FSale::checkData()
{
    return true;
}

void FSale::slotTable()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM HALL_TABLES ORDER BY HALL_ID, ID",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_table = "";
        m_pTable->setText("");
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_table = QString("%1,%2").arg(m_table).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_pTable->setText(QString("%1,%2").arg(m_pTable->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_table.length())
        {
            m_table = m_table.remove(0, 1);
            m_pTable->setText(m_pTable->text().remove(0, 1));
        }
    }
    delete c;
}

void FSale::slotWaiter()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM HALL_TABLES ORDER BY HALL_ID, ID",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_waiter = "";
        m_pWaiter->setText("");
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_waiter = QString("%1,%2").arg(m_waiter).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_pWaiter->setText(QString("%1,%2").arg(m_pWaiter->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_waiter.length())
        {
            m_waiter = m_waiter.remove(0, 1);
            m_pWaiter->setText(m_pWaiter->text().remove(0, 1));
        }
    }
    delete c;
}

void FSale::slotDishGroup()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM DISHES_GROUPS ORDER BY NAME",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_dishGroup = "";
        m_peDishGroup->setText("");
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_dishGroup = QString("%1,%2").arg(m_dishGroup).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_peDishGroup->setText(QString("%1,%2").arg(m_peDishGroup->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_dishGroup.length() > 0)
        {
            m_dishGroup = m_dishGroup.remove(0, 1);
            m_peDishGroup->setText(m_peDishGroup->text().remove(0, 1));
        }
    }
    delete c;
}

void FSale::slotDish()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList(QString("SELECT ID, NAME FROM DISHES %1 ORDER BY NAME")
                .arg(m_dishGroup.length() == 0 ? "" : QString("WHERE GROUP_ID IN (%1)").arg(m_dishGroup)),
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_dish = "";
        m_peDish->setText("");
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_dish = QString("%1,'%2'").arg(m_dish).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_peDish->setText(QString("%1,%2").arg(m_peDish->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_dish.length() > 0)
        {
            m_dish = m_dish.remove(0, 1);
            m_peDish->setText(m_peDish->text().remove(0, 1));
        }
    }
    delete c;
}
