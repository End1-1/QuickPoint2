#include "lhall.h"
#include "ehall.h"

LHall::LHall(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("List of halls"));
    getData();
}

void LHall::getData()
{
    table->ExecuteQuery("SELECT ID, NAME FROM HALL ORDER BY NAME");
    table->setColumn("ID", tr("ID"), 100, DATA_INTEGER);
    table->setColumn("NAME", tr("Name"), 400, DATA_STRING);
}

void LHall::slotNew()
{
    EHall *e = new EHall(this);
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *query = table->openDB();
        exec(query, "SELECT GEN_ID(GEN_HALL_ID, 1) FROM RDB$DATABASE");
        query->next();
        e->m_pId->setText(query->value(0).toString());
        prepare(query, "INSERT INTO HALL (ID, NAME) VALUES (:ID, :NAME)");
        query->bindValue(":ID", e->m_pId->text());
        query->bindValue(":NAME", e->m_pName->text());
        query->exec();
        delete query;
        setData(e, table->addRow());
    }
    delete e;
}

void LHall::slotEdit()
{
    QVector<int> rows;
    table->getSelectedRows(rows);
    if (!rows.count())
        return;

    EHall *e = new EHall(this);
    e->m_pId->setText(table->m_model.String("ID", rows[0]));
    e->m_pName->setText(table->m_model.String("NAME", rows[0]));
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        prepare(q, "UPDATE HALL SET NAME=:NAME WHERE ID=:ID");
        q->bindValue(":NAME", e->m_pName->text());
        q->bindValue(":ID", e->m_pId->text());
        q->exec();
        delete q;
        setData(e, rows[0]);
    }
    delete e;
}


void LHall::setData(EHall *h, const int &row)
{
    table->m_model.setData(row, "ID", h->m_pId->text());
    table->m_model.setData(row, "NAME", h->m_pName->text());
}
