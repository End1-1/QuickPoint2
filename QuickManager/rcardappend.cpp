#include "rcardappend.h"
#include "ecardappend.h"
#include "dlgnodiscountdishes.h"
#include "rcardstatistic.h"
#include "defines.h"

rcardappend::rcardappend(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Cards"));
    getData();
    topToolBar->addAction(QIcon(":/icons/eraser.png"), "", this, SLOT(noDiscountDishes()));
}

void rcardappend::getData()
{
    table->ExecuteQuery("SELECT ID, CARDNUM, CARDCODE, CUSTOMER, AMOUNT, VAL FROM CARDS_APPEND");
    table->setColumn("ID", tr("Id"), 0, DATA_INTEGER);
    table->setColumn("CARDNUM", tr("Card umber"), 100, DATA_STRING);
    table->setColumn("CARDCODE", tr("Card code"), 200, DATA_STRING);
    table->setColumn("CUSTOMER", tr("Customer"), 300, DATA_STRING);
    table->setColumn("AMOUNT", tr("Amount"), 100, DATA_DOUBLE);
    table->setColumn("VAL", tr("Value"), 100, DATA_DOUBLE);
}

void rcardappend::slotNew()
{
    ECardAppend *e = new ECardAppend(this);
    int id = 0;
    id = e->exec();
    if (id > 0) {
        int row = table->addRow();
        QSqlQuery *q = table->openDB();
        q->prepare("select cardnum, cardcode, customer, amount, val from cards_append where id=:id");
        q->bindValue(":id", id);
        q->exec();
        q->next();
        table->m_model.setData(row, "ID", id);
        table->m_model.setData(row, "CARDNUM", q->value(0));
        table->m_model.setData(row, "CARDCODE", q->value(1));
        table->m_model.setData(row, "CUSTOMER", q->value(2));
        table->m_model.setData(row, "AMOUNT", q->value(3));
        table->m_model.setData(row, "VAL", q->value(4));
        table->closeDB(q);
    }
    delete e;
}

void rcardappend::slotEdit()
{
    QVector<int> row = getSelectedRows();
    if (row.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (row.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }
    ECardAppend *e = new ECardAppend(this);
    int id = table->m_model.data("ID", row[0]).toInt();
    e->load(id);
    id = e->exec();
    if (id > 0) {
        QSqlQuery *q = table->openDB();
        q->prepare("select cardnum, cardcode, customer, amount, val from cards_append where id=:id");
        q->bindValue(":id", id);
        q->exec();
        q->next();
        table->m_model.setData(row.at(0), "ID", id);
        table->m_model.setData(row.at(0), "CARDNUM", q->value(0));
        table->m_model.setData(row.at(0), "CARDCODE", q->value(1));
        table->m_model.setData(row.at(0), "CUSTOMER", q->value(2));
        table->m_model.setData(row.at(0), "AMOUNT", q->value(3));
        table->m_model.setData(row.at(0), "VAL", q->value(4));
        table->closeDB(q);
    }
    delete e;
}

void rcardappend::slotTotal()
{
    double amount = 0;
    int used = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        amount += table->m_model.Double("AMOUNT", i);
        if (table->m_model.Double("AMOUNT", i) > 0.1) {
            used ++;
        }
    }

    const QLocale l = QLocale::c();
    QMessageBox::information(this, tr("Total"),
                             QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                                     "<table>"
                                     "<tr><td>%1</td><td>%2</td></tr>" //total records count
                                     "<tr><td>%3</td><td>%4</td></tr>" //total amount
                                     "<tr><td>%5</td><td>%6</td></tr>" //used
                                     "</table></html>" //total quantity of food
                                     )
                             .arg(tr("Count"))
                             .arg(l.toString(table->m_model.rowCount(QModelIndex())))
                             .arg(tr("Amount"))
                             .arg(l.toString(amount, 'f', 0))
                             .arg(tr("Used"))
                             .arg(used));
}

void rcardappend::doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QString id = table->m_model.String(index.row(), 0);
    RCardStatistic *r = new RCardStatistic(fMainWindow);
    r->fCard = id;
    r->fDate1 = "01.01.2017";
    r->getData();
    fMainWindow->m->addSubWindow(r);
    dynamic_cast<GridWindow *>(r)->setOnToolBar(fMainWindow->m_pToolBar);
    dynamic_cast<GridWindow *>(r)->show();
}

void rcardappend::noDiscountDishes()
{
    DlgNoDiscountDishes *d = new DlgNoDiscountDishes(this);
    d->exec();
    delete d;
}
