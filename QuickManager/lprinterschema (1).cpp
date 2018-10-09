#include "lprinterschema.h"
#include "eprinterschema.h"

LPrinterSchema::LPrinterSchema(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Schema of the printers"));
    getData();
}

void LPrinterSchema::getData()
{
    table->ExecuteQuery("SELECT ID, NAME FROM PRINTER_SCHEMA ORDER BY NAME");
    table->setColumn("ID", tr("Id"), 100, DATA_INTEGER);
    table->setColumn("NAME", tr("Name"), 400, DATA_STRING);
}

void LPrinterSchema::slotNew()
{
    EPrinterSchema *e = new EPrinterSchema(this);
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        q->exec("SELECT GEN_ID(GEN_PRINTER_SCHEMA_ID, 1) FROM RDB$DATABASE");
        q->next();
        e->m_pId->setText(q->value(0).toString());
        q->prepare("INSERT INTO PRINTER_SCHEMA (ID, NAME) VALUES (:ID, :NAME)");
        q->bindValue("ID", e->m_pId->text());
        q->bindValue("NAME", e->m_pName->text());
        q->exec();

        updateMembers(q, e);
        int new_row = table->addRow();
        setData(e, new_row);
        table->closeDB(q);
    }
    delete e;
}

void LPrinterSchema::slotEdit()
{
    QVector<int> rows;
    table->getSelectedRows(rows);
    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing is selected\n"));
        return;
    }

    EPrinterSchema *e = new EPrinterSchema(this);
    e->m_pId->setText(table->m_model.String("ID", rows[0]));
    e->m_pName->setText(table->m_model.String("NAME", rows[0]));
    QSqlQuery *q = table->openDB();
    q->prepare("SELECT PD.PRINTER_ID, P.NAME AS PRINTER_NAME FROM PRINTER_SCHEMA_DETAILS PD, PRINTERS P WHERE PD.PRINTER_ID=P.ID AND PD.SCHEMA_ID=:SCHEMA_ID");
    q->bindValue("SCHEMA_ID", e->m_pId->text());
    q->exec();
    while (q->next())
    {
        int new_row = e->m_pMembers->addRow();
        e->m_pMembers->m_model.setData(new_row, "PRINTER_ID", q->value(0).toInt());
        e->m_pMembers->m_model.setData(new_row, "PRINTER", q->value(1).toString());
    }

    if (e->exec() == QDialog::Accepted)
    {
        q->prepare("UPDATE PRINTER_SCHEMA SET NAME=:NAME WHERE ID=:ID");
        q->bindValue("NAME", e->m_pName->text());
        q->bindValue("ID", e->m_pId->text());
        q->exec();
        setData(e, rows[0]);
        updateMembers(q, e);
    }

    table->closeDB(q);
    delete e;
}

void LPrinterSchema::setData(EPrinterSchema *e, int row)
{
    table->m_model.setData(row, "ID", e->m_pId->text());
    table->m_model.setData(row, "NAME", e->m_pName->text());
}

void LPrinterSchema::updateMembers(QSqlQuery *q, EPrinterSchema *e)
{
    q->prepare("DELETE FROM PRINTER_SCHEMA_DETAILS WHERE SCHEMA_ID=:SCHEMA_ID");
    q->bindValue("SCHEMA_ID", e->m_pId->text());
    q->exec();

    for (int i = 0; i < e->m_pMembers->m_model.rowCount(QModelIndex()); i++)
    {
        q->prepare("INSERT INTO PRINTER_SCHEMA_DETAILS (SCHEMA_ID, PRINTER_ID) VALUES (:SCHEMA_ID, :PRINTER_ID)");
        q->bindValue("SCHEMA_ID", e->m_pId->text());
        q->bindValue("PRINTER_ID", e->m_pMembers->m_model.Integer("PRINTER_ID", i));
        q->exec();
    }
}
