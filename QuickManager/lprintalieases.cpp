#include "lprintalieases.h"
#include "eprintaliases.h"

LPrintAlieases::LPrintAlieases(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Aliases of the printers"));
    getData();
}

void LPrintAlieases::getData()
{
    table->ExecuteQuery("SELECT ID, NAME FROM PRINTER_ALIASES ORDER BY NAME");
    table->setColumn("ID", tr("Id"), 100, DATA_INTEGER);
    table->setColumn("NAME", tr("Name"), 500, DATA_STRING);
}

void LPrintAlieases::slotNew()
{
    EPrintAliases *e = new EPrintAliases(this);
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        q->exec("SELECT GEN_ID(GEN_PRINTER_ALIASES_ID, 1) FROM RDB$DATABASE");
        q->next();
        e->m_pId->setText(q->value(0).toString());
        q->prepare("INSERT INTO PRINTER_ALIASES (ID, NAME) VALUES (:ID, :NAME)");
        q->bindValue(":ID", e->m_pId->text());
        q->bindValue(":NAME", e->m_pName->text());
        q->exec();

        int new_row = table->addRow();
        setData(e, new_row);
        updateMembers(q, e);
        table->closeDB(q);
    }
    delete e;
}

void LPrintAlieases::slotEdit()
{
    QVector<int> rows;
    table->getSelectedRows(rows);
    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing is selected"));
        return;
    }

    EPrintAliases *e = new EPrintAliases(this);
    e->m_pId->setText(table->m_model.String("ID", rows[0]));
    e->m_pName->setText(table->m_model.String("NAME", rows[0]));

    QSqlQuery *q = table->openDB();
    q->prepare("SELECT PD.ALIAS_PRINTER_ID, P.NAME AS ALIAS_NAME, PD.PRINTER_NAME FROM PRINTER_ALIASES_DETAILS PD, PRINTERS P "
               "WHERE P.ID=PD.ALIAS_PRINTER_ID AND PD.ALIAS_SCHEMA_ID=:ALIAS_SCHEMA_ID");
    q->bindValue(":ALIAS_SCHEMA_ID", table->m_model.String("ID", rows[0]));
    q->exec();
    while (q->next())
    {
        int new_row = e->m_pMembers->addRow();
        e->m_pMembers->m_model.setData(new_row, "ALIAS_ID", q->value(0).toInt());
        e->m_pMembers->m_model.setData(new_row, "ALIAS", q->value(1).toString());
        e->m_pMembers->m_model.setData(new_row, "PRINTER", q->value(2).toString());
    }

    if (e->exec() == QDialog::Accepted)
    {
        q->prepare("UPDATE PRINTER_ALIASES SET NAME=:NAME WHERE ID=:ID");
        q->bindValue(":NAME", e->m_pName->text());
        q->bindValue(":ID", e->m_pId->text());
        q->exec();

        setData(e, rows[0]);
        updateMembers(q, e);
    }

    table->closeDB(q);
    delete e;
}

void LPrintAlieases::setData(EPrintAliases *e, int row)
{
    table->m_model.setData(row, "ID", e->m_pId->text());
    table->m_model.setData(row, "NAME", e->m_pName->text());
}

void LPrintAlieases::updateMembers(QSqlQuery *q, EPrintAliases *e)
{
    q->prepare("DELETE FROM PRINTER_ALIASES_DETAILS WHERE ALIAS_SCHEMA_ID=:ALIAS_SCHEMA_ID");
    q->bindValue(":ALIAS_SCHEMA_ID", e->m_pId->text());
    q->exec();

    for (int i = 0; i < e->m_pMembers->m_model.rowCount(QModelIndex()); i++)
    {
        q->prepare("INSERT INTO PRINTER_ALIASES_DETAILS (ALIAS_SCHEMA_ID, ALIAS_PRINTER_ID, PRINTER_NAME) VALUES (:ALIAS_SCHEMA_ID, :ALIAS_PRINTER_ID, :PRINTER_NAME)");
        q->bindValue(":ALIAS_SCHEMA_ID", e->m_pId->text());
        q->bindValue(":ALIAS_PRINTER_ID", e->m_pMembers->m_model.Integer("ALIAS_ID", i));
        q->bindValue(":PRINTER", e->m_pMembers->m_model.String("PRINTER", i));
        q->exec();
    }
}
