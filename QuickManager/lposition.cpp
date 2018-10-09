#include "lposition.h"
#include "eposition.h"

LPosition::LPosition()
{
    setWindowTitle(tr("Positions"));
    getData();
}

void LPosition::getData()
{
    QString sql = "SELECT ID, NAME FROM POSITIONS ";
    table->ExecuteQuery(sql);

    setColumnCaption("ID", tr("ID"));
    setColumnCaption("NAME", tr("Name"));

    setColumnWidth("ID", 100);
    setColumnWidth("NAME", 400);

    setColumnDataType("ID", 496);
}

void LPosition::setData(EPosition *e, const int row)
{
    table->m_model.setData(row, 0, e->pedt_Id->text());
    table->m_model.setData(row, 1, e->pedt_Name->text());
}

void LPosition::slotNew()
{
    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    EPosition *e = new EPosition(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(q, "SELECT GEN_ID(GEN_POSITIONS_ID, 1) FROM RDB$DATABASE"))
        {
            q->next();
            e->pedt_Id->setText(q->value(0).toString());
            if (prepare(q, "INSERT INTO POSITIONS (ID, NAME) VALUES (:ID, :NAME)"))
            {
                q->bindValue(":ID", e->pedt_Id->text());
                q->bindValue(":NAME", e->pedt_Name->text());
                if (exec(q))
                    setData(e, table->addRow());
            }
        }
        updateSalary(e, q);
    }
    delete e;
    table->closeDB(q);
}

void LPosition::slotEdit()
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

    EPosition *e = new EPosition(this);
    e->pedt_Id->setText(table->m_model.data(0, row[0]).toString());
    e->pedt_Name->setText(table->m_model.data(1, row[0]).toString());
    prepare(q, "SELECT QUEUE, OPERATION, PARAMS, CELL FROM SALARY_FORMULA WHERE POSITION_ID=:POSITION_ID");
    q->bindValue(":POSITION_ID", e->pedt_Id->text());
    exec(q);
    while (q->next())
    {
        int newRow = e->m_pSalary->addRow();
        e->m_pSalary->m_model.setData(newRow, "QUEUE", q->value(0));
        e->m_pSalary->m_model.setData(newRow, "OPERATION", q->value(1));
        e->m_pSalary->m_model.setData(newRow, "PARAMS", q->value(2));
        e->m_pSalary->m_model.setData(newRow, "CELL", q->value(3));
    }
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "UPDATE POSITIONS SET NAME=:NAME WHERE ID=:ID"))
        {
            q->bindValue(":NAME", e->pedt_Name->text());
            q->bindValue(":ID", e->pedt_Id->text());
            if (exec(q))
                setData(e, row[0]);
        }
        updateSalary(e, q);
    }
    delete e;
    table->closeDB(q);
}

void LPosition::updateSalary(EPosition *e, QSqlQuery *q)
{
    prepare(q, "DELETE FROM SALARY_FORMULA WHERE POSITION_ID=:POSITION_ID");
    q->bindValue(":POSITION_ID", e->pedt_Id->text());
    exec(q);
    prepare(q, "INSERT INTO SALARY_FORMULA (POSITION_ID, QUEUE, OPERATION, PARAMS, CELL) VALUES (:POSITION_ID, :QUEUE, :OPERATION, :PARAMS, :CELL)");
    for (int i = 0; i < e->m_pSalary->m_model.rowCount(QModelIndex()); i++)
    {
        q->bindValue(":POSITION_ID", e->pedt_Id->text());
        q->bindValue(":QUEUE", e->m_pSalary->m_model.String("QUEUE", i));
        q->bindValue(":OPERATION", e->m_pSalary->m_model.String("OPERATION", i));
        q->bindValue(":PARAMS", e->m_pSalary->m_model.String("PARAMS", i));
        q->bindValue(":CELL", e->m_pSalary->m_model.String("CELL", i));
        exec(q);
    }
}
