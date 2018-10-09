#include "lroles.h"
#include "erole.h"
#include "checklist.h"

LRoles::LRoles(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Roles"));
    topToolBar->addAction(tr("Roles set"), this, SLOT(slotRolesSet()));
    getData();
}

void LRoles::getData()
{
    QString sql = "SELECT ID, NAME FROM ROLES";
    table->ExecuteQuery(sql);

    setColumnCaption("ID", tr("Id"));
    setColumnCaption("NAME", tr("Name"));

    setColumnWidth("ID", 100);
    setColumnWidth("NAME", 300);

    setColumnDataType("ID", 496);
}

void LRoles::setData(ERole *e, const int row)
{
    table->m_model.setData(row, 0,  e->pedt_Id->text());
    table->m_model.setData(row, 1, e->pedt_Name->text());
}


void LRoles::slotNew()
{
    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    ERole *e = new ERole(this);
    if (e->exec())
    {
        if (exec(q, "SELECT GEN_ID(GEN_ROLES_ID, 1) FROM RDB$DATABASE"))
        {
            q->next();
            e->pedt_Id->setText(q->value(0).toString());
            if (prepare(q, "INSERT INTO ROLES (ID, NAME) VALUES (:ID, :NAME)"))
            {
                q->bindValue(":ID", e->pedt_Id->text());
                q->bindValue(":NAME", e->pedt_Name->text());
                if (exec(q))
                    setData(e, table->addRow());
            }
        }
    }
    delete e;
    table->closeDB(q);
}

void LRoles::slotEdit()
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

    ERole *e = new ERole(this);
    e->pedt_Id->setText(table->m_model.data(0, row[0]).toString());
    e->pedt_Name->setText(table->m_model.data(1, row[0]).toString());
    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "UPDATE ROLES SET NAME=:NAME WHERE ID=:ID"))
        {
            q->bindValue(":NAME", e->pedt_Name->text());
            q->bindValue(":ID", e->pedt_Id->text());
            if (exec(q))
                setData(e, row[0]);
        }
    }
    delete e;
    table->closeDB(q);
}

void LRoles::slotRolesSet()
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

    CheckList *c = new CheckList(this);
    c->key.push_back(ROLE_EDIT_PARTIAL_DATA);
    c->key.push_back(ROLE_EDIT_ALL_DATA);
    c->key.push_back(ROLE_EDIT_STORE_DOCS);
    c->key.push_back(ROLE_MAKE_SALE);
    c->key.push_back(ROLE_VIEW_SALE_DOCS);
    c->key.push_back(ROLE_DEL_SALE_DOCS);
    c->key.push_back(ROLE_EDIT_SALARY_1);
    c->key.push_back(ROLE_EDIT_SALARY_2);
    c->key.push_back(ROLE_DEL_STORE_DOC);
    c->values.push_back(tr("Edit data (goods, dishes, etc...)"));
    c->values.push_back(tr("Edit all data"));
    c->values.push_back(tr("Edit store documents"));
    c->values.push_back(tr("Make sales"));
    c->values.push_back(tr("View sales"));
    c->values.push_back(tr("Delete sales orders"));
    c->values.push_back(tr("Edit salary with formula"));
    c->values.push_back(tr("Edit salary with any value"));
    c->values.push_back(tr("Delete documents in the store"));

    int role_id = table->m_model.data("ID", row[0]).toInt();

    if (prepare(q, "SELECT ACTION_ID FROM ROLES_SET WHERE ROLE_ID=:ROLE_ID"))
    {
        q->bindValue(":ROLE_ID", role_id);
        if (exec(q))
        {
            while (q->next())
                c->result.push_back(q->value(0).toInt());
            c->makeList();
        }
        if (c->exec() == QDialog::Accepted)
        {
            if (prepare(q, "DELETE FROM ROLES_SET WHERE ROLE_ID=:ROLE_ID"))
            {
                q->bindValue(":ROLE_ID", role_id);
                exec(q);
            }
            if (prepare(q, "INSERT INTO ROLES_SET (ROLE_ID, ACTION_ID) VALUES (:ROLE_ID, :ACTION_ID)"))
            {
                for (int i = 0; i < c->result.count(); i++)
                {
                    q->bindValue(":ROLE_ID", role_id);
                    q->bindValue(":ACTION_ID", c->result[i].toInt());
                    exec(q);
                }
            }
        }
    }
    delete c;
    table->closeDB(q);
}
