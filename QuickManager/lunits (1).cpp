#include "lunits.h"
#include "eunit.h"

LUnits::LUnits(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Units"));
    getData();
}

void LUnits::getData()
{
    QString sql = "SELECT * FROM UNITS";
    table->ExecuteQuery(sql);
    setColumnCaption("ID", tr("Id"));
    setColumnCaption("NAME", tr("Name"));
    setColumnWidth("NAME", 300);
}

void LUnits::setData(EUnit *e, int row)
{
    table->m_model.setData(row, 0, e->pedt_Id->text());
    table->m_model.setData(row, 1, e->pedt_Name->text());
}

void LUnits::slotNew()
{
    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    EUnit *e = new EUnit(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(query, "SELECT GEN_ID(GEN_UNITS_ID, 1) FROM RDB$DATABASE"))
        {
            query->next();
            e->pedt_Id->setText(query->value(0).toString());

            if (prepare(query, "INSERT INTO UNITS (ID, NAME) VALUES (:ID, :NAME)"))
            {
                query->bindValue("ID", e->pedt_Id->text());
                query->bindValue("NAME", e->pedt_Name->text());
                if (exec(query))
                    setData(e, table->addRow());
            }
        }
    }
    delete e;
    table->closeDB(query);
}

void LUnits::slotEdit()
{
    QVector<int> &row = getSelectedRows();
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

    QSqlQuery *query;
    if ((query = table->openDB()) == 0)
        return;

    EUnit *e = new EUnit(this);
    e->pedt_Id->setText(table->m_model.data(0, row[0]).toString());
    e->pedt_Name->setText(table->m_model.data(1, row[0]).toString());
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(query, "UPDATE UNITS SET NAME=:NAME WHERE ID=:ID"))
        {
            query->bindValue("NAME", e->pedt_Name->text());
            query->bindValue("ID", e->pedt_Id->text());
            if (exec(query))
                setData(e, row[0]);
        }
    }
    delete e;
    table->closeDB(query);
}
