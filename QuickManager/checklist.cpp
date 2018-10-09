#include "checklist.h"
#include "global.h"
#include "../All/econnection.h"

CheckList::CheckList(bool multiCheck, QWidget *parent) :
    QDialog(parent), m_multiCheck(multiCheck)
{
    setWindowTitle(tr("List"));
    plist_Values = new QListWidget();

    table = new TableView();
    EConnection::setConnectionParams(table->m_db.db, Data::databaseIndex);

    plist_Values->setMinimumSize(290, 400);
    connect(plist_Values, SIGNAL(clicked(QModelIndex)), SLOT(slotItemChanged(QModelIndex)));
    QPushButton *pbtnOk = new QPushButton(tr("OK"));
    connect(pbtnOk, SIGNAL(clicked()), SLOT(slotOk()));
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(plist_Values);
    QHBoxLayout *layoutButtons = new QHBoxLayout();
    layoutButtons->addStretch(1);
    layoutButtons->addWidget(pbtnOk);
    layoutButtons->addWidget(pbtnCancel);
    QWidget *wdtButtons = new QWidget();
    wdtButtons->setLayout(layoutButtons);
    layoutMain->addStretch(1);
    layoutMain->addWidget(wdtButtons);
    setLayout(layoutMain);
    resize(300, 430);
}

void CheckList::sql(const QString sql, const int column)
{
    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    if (!query->exec(sql))
    {
        QMessageBox::critical(this, "SQL error", query->lastError().databaseText());
        return;
    }

    while (query->next())
    {
        key.push_back(query->value(0));
        values.push_back(query->value(column));
    }

    table->closeDB(query);

    makeList();
}

void CheckList::splitResult(const QString str)
{
    result.clear();
    QStringList l = str.split(",");
    for (QStringList::const_iterator i = l.begin(); i != l.end(); i++)
        result.push_back(*i);
}

void CheckList::makeList()
{
    for (int i = 0; i < key.size(); i++)
    {
        QListWidgetItem *pItem = new QListWidgetItem(values.at(i).toString(), plist_Values);
        pItem->setCheckState(result.contains(key.at(i)) ? Qt::Checked : Qt::Unchecked);
    }
}

void CheckList::slotItemChanged(QModelIndex index)
{
    if (!m_multiCheck)
    {
        for (int i = 0; i < plist_Values->count(); i++)
            plist_Values->item(i)->setCheckState(Qt::Unchecked);
        plist_Values->item(index.row())->setCheckState(Qt::Checked);
    }
}

void CheckList::slotOk()
{
    result.clear();
    checked.clear();
    for (int i = 0; i < plist_Values->count(); i++)
        if (plist_Values->item(i)->checkState() == Qt::Checked)
        {
            result.push_back(key.at(i));
            checked.push_back(i);
        }
    accept();
}

