#include "ltables.h"
#include "etables.h"

LTables::LTables(QWidget *parent):
    GridWindow(parent)
{
    setWindowTitle(tr("List of tables"));
    getData();
}

void LTables::getData()
{
    table->ExecuteQuery("SELECT H.ID AS HALL_ID, H.NAME AS HALL_NAME, T.ID AS TABLE_ID, T.NAME AS TABLE_NAME, T.QUEUE "
                        "FROM HALL H, HALL_TABLES T "
                        "WHERE H.ID=T.HALL_ID "
                        "ORDER BY T.QUEUE ");
    table->setColumn("HALL_ID", tr("Hall id"), 0, DATA_INTEGER);
    table->setColumn("HALL_NAME", tr("Hall name"), 200, DATA_STRING);
    table->setColumn("TABLE_ID", tr("Table id"), 0, DATA_INTEGER);
    table->setColumn("TABLE_NAME", tr("Table name"), 300, DATA_STRING);
    table->setColumn("QUEUE", tr("Queue"), 100, DATA_INTEGER);
}

void LTables::setData(ETables *e, const int &row)
{
    table->m_model.setData(row, "HALL_ID", e->m_HallId);
    table->m_model.setData(row, "HALL_NAME", e->m_pHallName->text());
    table->m_model.setData(row, "TABLE_ID", e->m_pId->text());
    table->m_model.setData(row, "TABLE_NAME", e->m_pTableName->text());
    table->m_model.setData(row, "QUEUE", e->m_pQueue->text());
}

void LTables::slotNew()
{
    ETables *e = new ETables(this);
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        q->exec("SELECT GEN_ID(GEN_HALL_TABLES_ID, 1) FROM RDB$DATABASE");
        q->next();
        e->m_pId->setText(q->value(0).toString());
        q->prepare("INSERT INTO HALL_TABLES (ID, HALL_ID, NAME, QUEUE) VALUES (:ID, :HALL_ID, :NAME, :QUEUE)");
        q->bindValue(":ID", e->m_pId->text());
        q->bindValue(":HALL_ID", e->m_HallId);
        q->bindValue(":NAME", e->m_pTableName->text());
        q->bindValue(":QUEUE", e->m_pQueue->text());
        q->exec();

        int new_row = table->addRow();
        setData(e, new_row);
        table->closeDB(q);
    }
    delete e;
}

void LTables::slotEdit()
{
    QVector<int> rows;
    table->getSelectedRows(rows);
    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing is selected\n"));
        return;
    }

    ETables *e = new ETables(this);
    e->m_pId->setText(table->m_model.String("TABLE_ID", rows[0]));
    e->m_HallId = table->m_model.Integer("HALL_ID", rows[0]);
    e->m_pHallName->setText(table->m_model.String("HALL_NAME", rows[0]));
    e->m_pTableName->setText(table->m_model.String("TABLE_NAME", rows[0]));
    e->m_pQueue->setText(table->m_model.String("QUEUE", rows[0]));
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        q->prepare("UPDATE HALL_TABLES SET HALL_ID=:HALL_ID, NAME=:NAME, QUEUE=:QUEUE WHERE ID=:ID");
        q->bindValue(":HALL_ID", e->m_HallId);
        q->bindValue(":NAME", e->m_pTableName->text());
        q->bindValue(":QUEUE", e->m_pQueue->text().toInt());
        q->bindValue(":ID", e->m_pId->text());
        q->exec();

        setData(e, rows[0]);
        table->closeDB(q);
    }

    delete e;
}
