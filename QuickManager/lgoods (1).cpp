#include "lgoods.h"
#include "egoods.h"

LGoods::LGoods(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Goods"));
    getData();
}

void LGoods::getData()
{
    table->ExecuteQuery("SELECT G.ID, G.GROUP_ID, GR.NAME AS GNAME, G.NAME, G.UNIT_ID, U.NAME AS UNAME "
                        "FROM GOODS G, GOODS_GROUPS GR, UNITS U "
                        "WHERE G.GROUP_ID=GR.ID AND G.UNIT_ID=U.ID "
                        "ORDER BY G.ID ");

    setColumn("ID", tr("Id"), 100, 496);
    setColumn("GROUP_ID", tr("Group code"), 0, 496);
    setColumn("GNAME", tr("Group"), 200, 0);
    setColumn("NAME", tr("Name"), 300, 0);
    setColumn("UNIT_ID", tr("Unit code"), 0, 496);
    setColumn("UNAME", tr("Unit"), 100, 0);
}

void LGoods::setData(EGoods *e, const int row)
{
    table->m_model.setData(row, "ID", e->pedt_Id->text());
    table->m_model.setData(row, "GROUP_ID", e->m_group);
    table->m_model.setData(row, "GNAME", e->pedt_Group->text());
    table->m_model.setData(row, "NAME", e->pedt_Name->text());
    table->m_model.setData(row, "UNIT_ID", e->m_unit);
    table->m_model.setData(row, "UNAME", e->pedt_Unit->text());
}


void LGoods::slotNew()
{
    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    EGoods *e = new EGoods(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(q, "SELECT GEN_ID(GEN_GOODS_ID, 1) FROM RDB$DATABASE"))
        {
            q->next();
            e->pedt_Id->setText(q->value(0).toString());
            if (prepare(q, "INSERT INTO GOODS (ID, GROUP_ID, NAME, UNIT_ID) VALUES (:ID, :GROUP_ID, :NAME, :UNIT_ID)"))
            {
                q->bindValue("ID", e->pedt_Id->text());
                q->bindValue("GROUP_ID", e->m_group);
                q->bindValue("NAME", e->pedt_Name->text());
                q->bindValue("UNIT_ID", e->m_unit);
                exec(q);
                setData(e, table->addRow());
            }
        }
    }
    delete e;
    table->closeDB(q);
}

void LGoods::slotEdit()
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

    EGoods *e = new EGoods(this);
    e->pedt_Id->setText(table->m_model.data("ID", row[0]).toString());
    e->m_group = table->m_model.data("GROUP_ID", row[0]).toString();
    e->pedt_Group->setText(table->m_model.data("GNAME", row[0]).toString());
    e->pedt_Name->setText(table->m_model.data("NAME", row[0]).toString());
    e->m_unit = table->m_model.data("UNIT_ID", row[0]).toString();
    e->pedt_Unit->setText(table->m_model.data("UNAME", row[0]).toString());
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "UPDATE GOODS SET GROUP_ID=:GROUP_ID, NAME=:NAME, UNIT_ID=:UNIT_ID WHERE ID=:ID"))
        {
            q->bindValue("GROUP_ID", e->m_group);
            q->bindValue("NAME", e->pedt_Name->text());
            q->bindValue("UNIT_ID", e->m_unit);
            q->bindValue("ID", e->pedt_Id->text());
            exec(q);
            setData(e, row[0]);
        }
    }
    delete e;
    table->closeDB(q);
}

