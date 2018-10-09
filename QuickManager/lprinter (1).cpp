#include "lprinter.h"
#include "eprinter.h"

LPrinter::LPrinter(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Printers"));
    getData();
}

void LPrinter::getData()
{
    table->ExecuteQuery("SELECT ID, NAME FROM PRINTERS");

    setColumn("ID", tr("Id"), 100, 496);
    setColumn("NAME", tr("Name"), 300, 0);
}

void LPrinter::setData(EPrinter *e, const int row)
{
    table->m_model.setData(row, "ID", e->pedt_Id->text());
    table->m_model.setData(row, "NAME", e->pedt_Name->text());
}

void LPrinter::slotNew()
{
    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    EPrinter *e = new EPrinter(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(q, "SELECT GEN_ID(GEN_PRINTERS_ID, 1) FROM RDB$DATABASE"))
        {
            q->next();
            e->pedt_Id->setText(q->value(0).toString());
            if (prepare(q, "INSERT INTO PRINTERS (ID, NAME) VALUES (:ID, :NAME)"))
            {
                q->bindValue("ID", e->pedt_Id->text());
                q->bindValue("NAME", e->pedt_Name->text());
                if (exec(q))
                    setData(e, table->addRow());
            }
        }
    }
    delete e;
    table->closeDB(q);
}

void LPrinter::slotEdit()
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

    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    EPrinter *e = new EPrinter(this);
    e->pedt_Id->setText(table->m_model.data("ID", row[0]).toString());
    e->pedt_Name->setText(table->m_model.data("NAME", row[0]).toString());
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "UPDATE PRINTERS SET NAME=:NAME WHERE ID=:ID"))
        {
            q->bindValue("NAME", e->pedt_Name->text());
            q->bindValue("ID", e->pedt_Id->text());
            exec(q);
            setData(e, row[0]);
        }
    }
    delete e;
    table->closeDB(q);
}
