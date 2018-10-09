#include "lmodifiers.h"
#include "emodifier.h"

LModifiers::LModifiers(QWidget *parent):
    GridWindow(parent)
{
    setWindowTitle(tr("Modifiers"));
    getData();
}

void LModifiers::getData()
{
    table->ExecuteQuery("SELECT M.ID, M.NAME, M.ACTION_ID, MT.NAME AS MTNAME "
                        "FROM MODIFIERS M, MODIFIERS_TYPES MT "
                        "WHERE MT.ID=M.ACTION_ID");
    table->setColumn("ID", tr("Id"), 0, DATA_INTEGER);
    table->setColumn("NAME", tr("Name"), 400, DATA_STRING);
    table->setColumn("ACTION_ID", tr("Action id"), 0, DATA_INTEGER);
    table->setColumn("MTNAME", tr("Action"), 200, DATA_STRING);
}

void LModifiers::slotNew()
{
    EModifier *e = new EModifier(this);
    if (e->exec() == QDialog::Accepted)
    {
        QSqlQuery *q = table->openDB();
        q->exec("SELECT GEN_ID(GEN_MODIFIERS_ID, 1) FROM RDB$DATABASE");
        q->next();
        e->m_id = q->value(0).toInt();
        q->prepare("INSERT INTO MODIFIERS (ID, NAME, ACTION_ID, PARAMS) VALUES (:ID, :NAME, :ACTION_ID, :PARAMS)");
        q->bindValue(":ID", e->m_id);
        q->bindValue(":NAME", e->m_pName->text());
        q->bindValue(":ACTION_ID", e->m_pAction->itemData(e->m_pAction->currentIndex()));
        q->bindValue(":PARAMS", "");
        q->exec();
        int row = table->addRow();
        setData(e, row);
    }
    delete e;
}

void LModifiers::slotEdit()
{
    QVector<int> rows;
    table->getSelectedRows(rows);

    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    EModifier *e = new EModifier(this);
    QSqlQuery *q = table->openDB();
    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
    {
        e->m_id = table->m_model.data("ID", *i).toInt();
        e->m_pName->setText(table->m_model.data("NAME", *i).toString());
        for (int j = 0; j < e->m_pAction->count(); j++)
            if (e->m_pAction->itemData(j).toInt() == table->m_model.Integer("ACTION_ID", j))
            {
                e->m_pAction->setCurrentIndex(j);
                break;
            }

        if (e->exec() == QDialog::Accepted)
        {
            q->prepare("UPDATE MODIFIERS SET NAME=:NAME, ACTION_ID=:ACTION_ID, PARAMS=:PARAMS WHERE ID=:ID");
            q->bindValue(":NAME", e->m_pName->text());
            q->bindValue(":ACTION_ID", e->m_pAction->itemData(e->m_pAction->currentIndex()));
            q->bindValue(":PARAMS", "");
            q->bindValue(":ID", e->m_id);
            q->exec();
            setData(e, *i);
        }
    }
    table->closeDB(q);
    delete e;
}

void LModifiers::setData(const EModifier *e, const int &row)
{
    table->m_model.setData(row, "ID", e->m_id);
    table->m_model.setData(row, "NAME", e->m_pName->text());
}
