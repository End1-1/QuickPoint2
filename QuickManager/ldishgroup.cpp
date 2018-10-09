#include "ldishgroup.h"
#include "edishgroup.h"

LDishGroup::LDishGroup(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Dishes groups"));
    getData();
}

void LDishGroup::getData()
{
    QString sql = "SELECT * FROM DISHES_GROUPS";
    table->ExecuteQuery(sql);
    setColumnCaption("ID", tr("Id"));
    setColumnCaption("NAME", tr("Name"));
    setColumnWidth("NAME", 300);
}

void LDishGroup::setData(EDishGroup *e, const int row)
{
    table->m_model.setData(row, 0, e->pedt_Id->text());
    table->m_model.setData(row, 1, e->pedt_Name->text());
}

void LDishGroup::slotNew()
{
    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    EDishGroup *e = new EDishGroup(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(query, "SELECT GEN_ID(GEN_DISHES_GROUPS_ID, 1) FROM RDB$DATABASE"))
        {
            query->next();
            e->pedt_Id->setText(query->value(0).toString());
            if (prepare(query, "INSERT INTO DISHES_GROUPS (ID, NAME) VALUES (:ID, :NAME)"))
            {
                query->bindValue(":ID", e->pedt_Id->text());
                query->bindValue(":NAME", e->pedt_Name->text());
                if (exec(query))
                    setData(e, table->addRow());
            }
        }
    }
    delete e;
    table->closeDB(query);
}

void LDishGroup::slotEdit()
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

    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    EDishGroup *e = new EDishGroup(this);
    e->pedt_Id->setText(table->m_model.data(0, row[0]).toString());
    e->pedt_Name->setText(table->m_model.data(1, row[0]).toString());
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(query, "UPDATE DISHES_GROUPS SET NAME=:NAME WHERE ID=:ID"))
        {
            query->bindValue(":NAME", e->pedt_Name->text());
            query->bindValue(":ID", e->pedt_Id->text());
            if (exec(query))
                setData(e, row[0]);
        }
    }
    delete e;
    table->closeDB(query);
}
