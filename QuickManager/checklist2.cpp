#include "checklist2.h"
#include "../All/econnection.h"
#include "global.h"

CheckList2::CheckList2(bool multiCheck, QWidget *parent) :
    QDialog(parent), m_multiCheck(multiCheck)
{
    setWindowTitle(tr("List"));

    m_pt = new TableView();
    EConnection::setConnectionParams(m_pt->m_db.db, Data::databaseIndex);

    m_pl = new QListWidget();
    m_pl->setMinimumSize(400, 550);

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(m_pl);

    QPushButton *pbtn_Ok = new QPushButton(tr("Ok"));
    QPushButton *pbtn_Cancel = new QPushButton(tr("Cancel"));
    QHBoxLayout *l1 = new QHBoxLayout();
    QWidget *w1 = new QWidget();
    l1->addStretch(1);
    l1->addWidget(pbtn_Ok);
    l1->addWidget(pbtn_Cancel);
    w1->setLayout(l1);
    l->addWidget(w1);

    connect(pbtn_Ok, SIGNAL(clicked()), SLOT(slotOk()));
    connect(pbtn_Cancel, SIGNAL(clicked()), SLOT(reject()));
    connect(m_pl, SIGNAL(clicked(QModelIndex)), SLOT(slotItemChanged(QModelIndex)));

    setLayout(l);
}

void CheckList2::buildList(const QString &sql, const QString &colKey, const QString &colValue)
{
    m_colKey = colKey;
    m_colValue = colValue;

    m_pt->ExecuteQuery(sql);

    for (int i = 0; i < m_pt->m_model.rowCount(QModelIndex()); i++)
    {
        QListWidgetItem *l = new QListWidgetItem(m_pt->m_model.data(colValue, i).toString(), m_pl);
        l->setCheckState(m_result.values().contains(m_pt->m_model.data(colKey, i).toString()) ? Qt::Checked : Qt::Unchecked);
    }
}

void CheckList2::returnResult(QString &keys, QString &values)
{
    keys = "";
    values = "";
    for (int i = 0; i < m_result.count(); i++)
    {
        keys += + "," + m_pt->m_model.String(m_colKey, m_result.keys().at(i));
        values += "," + m_pt->m_model.String(m_colValue, m_result.keys().at(i));
    }
    if (keys.length() > 0)
    {
        keys = keys.remove(0, 1);
        values = values.remove(0, 1);
    }
}

void CheckList2::Data(const QString &sql, QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    CheckList2 *c = new CheckList2(multiCheck, parent);
    c->buildList(sql, "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        names->clear();
        ids.clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            names->setText(names->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
            ids = ids + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString();
        }
        if (names->text().length() > 0)
        {
            names->setText(names->text().remove(0, 1));
            ids = ids.remove(0, 1);
        }
    }

    delete c;
}

void CheckList2::Store(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM STORAGE ORDER BY NAME ", names, ids, multiCheck, parent);
}

void CheckList2::GroupGoods(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM GOODS_GROUPS ORDER BY NAME", names, ids, multiCheck, parent);
}

void CheckList2::Goods(const QString &group, QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM GOODS " + (group.length() ? "WHERE GROUP_ID IN (" + group + ") " : "") + " ORDER BY NAME", names, ids, multiCheck, parent);
}

void CheckList2::Menu(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM MENU ORDER BY NAME", names, ids, multiCheck, parent);
}

void CheckList2::DishesGroups(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM DISHES_GROUPS ORDER BY NAME", names, ids, multiCheck, parent);
}

void CheckList2::StoreActions(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT ID, NAME FROM STORE_ACTIONS ORDER BY ID", names, ids, multiCheck, parent);
}

void CheckList2::InOut(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent)
{
    Data("SELECT '" + tr("In") + "', 1 FROM RDB$DATABASE UNION SELECT '" + tr("Out") + "', 2 FROM RDB$DATABASE", names, ids, multiCheck, parent);
}

void CheckList2::slotItemChanged(QModelIndex index)
{
    if (!m_multiCheck)
    {
        for (int i = 0; i < m_pl->count(); i++)
            m_pl->item(i)->setCheckState(Qt::Unchecked);
        m_pl->item(index.row())->setCheckState(Qt::Checked);
    }
}

void CheckList2::slotOk()
{
    m_result.clear();
    for (int i = 0; i < m_pt->m_model.rowCount(QModelIndex()); i++)
        if (m_pl->item(i)->checkState() == Qt::Checked)
            m_result[i] = m_pt->m_model.data(m_colKey, i);
    accept();
}

