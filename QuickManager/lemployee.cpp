#include "lemployee.h"
#include "femployee.h"
#include "eemployee.h"

LEmployee::LEmployee(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Employees"));
    m_fposition = "";
    m_factive = true;
    getData();
}

void LEmployee::getData()
{
    QString sql = "SELECT E.ID, E.POSITION_ID, P.NAME AS PNAME, "
            "E.NAME, L.ROLE_ID, R.NAME AS RNAME, L.PASSWORD, E.ACTIVE "
            "FROM EMPLOYEES E "
            "LEFT JOIN POSITIONS P ON E.POSITION_ID=P.ID "
            "LEFT JOIN LOGINS L ON E.ID=L.EMPLOYEE_ID "
            "LEFT JOIN ROLES R ON L.ROLE_ID=R.ID "
            "WHERE E.ACTIVE=";
    sql += (m_factive ? "1" : "0");

    if(m_fposition.length() > 0)
        sql += QString(" AND E.POSITION_ID IN (%1)").arg(m_fposition);

    table->ExecuteQuery(sql);

    setColumn("ID", tr("ID"), 100, 496);
    setColumn("POSITION_ID", tr("Position code"), 0, 496);
    setColumn("PNAME", tr("Position"), 200, 0);
    setColumn("NAME", tr("Name"), 300, 0);
    setColumn("ROLE_ID", tr("Role code"), 0, 496);
    setColumn("RNAME", tr("Role name"), 200, 0);
    setColumn("PASSWORD", tr("Password"), 0, 0);
    setColumn("ACTIVE", tr("Active"), 0, 496);
}

void LEmployee::setData(EEmployee *e, const int row)
{
    table->m_model.setData(row, "ID", e->pedt_Id->text());
    table->m_model.setData(row, "POSITION_ID", e->m_position);
    table->m_model.setData(row, "PNAME", e->pedt_Position->text());
    table->m_model.setData(row, "NAME", e->pedt_Name->text());
    table->m_model.setData(row, "ROLE_ID", e->m_role);
    table->m_model.setData(row, "RNAME", e->pedt_Role->text());
    table->m_model.setData(row, "PASSWORD", e->m_password);
    table->m_model.setData(row, "ACTIVE", e->pchk_Active->isChecked());
}

void LEmployee::slotNew()
{
    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    EEmployee *e = new EEmployee(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(q, "SELECT GEN_ID(GEN_EMPLOYEES_ID, 1) FROM RDB$DATABASE"))
        {
            q->next();
            e->pedt_Id->setText(q->value(0).toString());
            if (prepare(q, "INSERT INTO EMPLOYEES (ID, POSITION_ID, NAME, ACTIVE) VALUES (:ID, :POSITION_ID, :NAME, :ACTIVE)"))
            {
                q->bindValue(":ID", e->pedt_Id->text());
                q->bindValue(":POSITION_ID", e->m_position);
                q->bindValue(":NAME", e->pedt_Name->text());
                q->bindValue(":ACTIVE", (e->pchk_Active->isChecked() ? 1 : 0));
                exec(q);
            }
            if (e->pchk_DBAccess->isChecked())
            {
                if (prepare(q, "INSERT INTO LOGINS (EMPLOYEE_ID, ROLE_ID, PASSWORD) VALUES (:EMPLOYEE_ID, :ROLE_ID, :PASSWORD)"))
                {
                    q->bindValue(":EMPLOYEE_ID", e->pedt_Id->text());
                    q->bindValue(":ROLE_ID", e->m_role);
                    q->bindValue(":PASSWORD", e->m_password);
                    exec(q);
                }
            }
            if (m_factive == e->pchk_Active->isChecked())
                setData(e, table->addRow());
        }
    }
    delete e;
    table->closeDB(q);
}

void LEmployee::slotEdit()
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

    EEmployee *e = new EEmployee(this);
    e->pedt_Id->setText(table->m_model.data("ID", row[0]).toString());
    e->m_position = table->m_model.data("POSITION_ID", row[0]).toString();
    e->pedt_Position->setText(table->m_model.data("PNAME", row[0]).toString());
    e->pedt_Name->setText(table->m_model.data("NAME", row[0]).toString());
    e->pchk_DBAccess->setChecked(table->m_model.data("ROLE_ID", row[0]).toInt() != 0);
    e->m_password = table->m_model.data("PASSWORD", row[0]).toString();
    e->m_role = table->m_model.data("ROLE_ID", row[0]).toString();
    e->pedt_Role->setText(table->m_model.data("RNAME", row[0]).toString());
    e->pchk_Active->setChecked(table->m_model.data("ACTIVE", row[0]).toInt() != 0);

    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "UPDATE EMPLOYEES SET POSITION_ID=:POSITION_ID, NAME=:NAME, ACTIVE=:ACTIVE WHERE ID=:ID"))
        {
            q->bindValue(":POSITION_ID", e->m_position);
            q->bindValue(":NAME", e->pedt_Name->text());
            q->bindValue(":ACTIVE", e->pchk_Active->isChecked());
            q->bindValue(":ID", e->pedt_Id->text());
            exec(q);
        }
        if (prepare(q, "DELETE FROM LOGINS WHERE EMPLOYEE_ID=:EMPLOYEE_ID"))
        {
            q->bindValue(":EMPLOYEE_ID", e->pedt_Id->text());
            exec(q);
        }
        if (e->pchk_DBAccess->isChecked())
        {
            if (prepare(q, "INSERT INTO LOGINS (EMPLOYEE_ID, ROLE_ID, PASSWORD) VALUES (:EMPLOYEE_ID, :ROLE_ID, :PASSWORD)"))
            {
                q->bindValue(":EMPLOYEE_ID", e->pedt_Id->text());
                q->bindValue(":ROLE_ID", e->m_role.toInt());
                q->bindValue(":PASSWORD", e->m_password);
                exec(q);
            }
        }
        if (m_factive == e->pchk_Active->isChecked())
            setData(e, row[0]);
        else
            table->m_model.removeRows(row[0], 1, QModelIndex());
    }
    delete e;
    table->closeDB(q);
}

void LEmployee::slotFilter()
{
    FEmployee *f = new FEmployee(this);
    f->pedt_Position->setText(m_fposition);
    f->pchk_Active->setChecked(m_factive);
    if (f->exec() == QDialog::Accepted)
    {
        m_fposition = f->pedt_Position->text();
        m_factive = f->pchk_Active->isChecked();
        getData();
    }
    delete f;
}
