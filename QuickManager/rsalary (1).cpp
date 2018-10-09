#include "rsalary.h"
#include "esalarydoc.h"
#include "fsalary.h"

RSalary::RSalary(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Salary report"));
    m_docId = "";
    m_date1 = QDate::currentDate().addDays(-1 * QDate::currentDate().day() + 1);
    m_date2 = QDate::currentDate();
    m_operatorName = "";
    m_operatorId = "";
    m_positionName = "";
    m_positionId = "";
    m_employeeName = "";
    m_employeeId = "";
    m_groupDocId = false;
    m_groupDate = true;
    m_groupOperator = false;
    m_groupPosition = false;
    m_groupEmployee = true;
    getData();
}

void RSalary::getData()
{
    QString where, select, group;

    if (m_groupDocId)
    {
        select += "SH.ID,";
        group += "SH.ID,";
    }

    if (m_groupDate)
    {
        select += "SH.DOC_DATE,";
        group += "SH.DOC_DATE,";
    }

    if (m_groupOperator)
    {
        select += "SH.OPERATOR_ID, E1.NAME AS OPERATOR_NAME,";
        group += "SH.OPERATOR_ID,E1.NAME,";
    }

    if (m_groupPosition)
    {
        select += "SB.POSITION_ID,P.NAME AS POSITION_NAME,";
        group += "SB.POSITION_ID,P.NAME,";
    }

    if (m_groupEmployee)
    {
        select += "SB.EMPLOYEE_ID,E2.NAME AS EMPLOYEE_NAME,";
        group += "SB.EMPLOYEE_ID,E2.NAME,";
    }

    where += QString("WHERE SH.DOC_DATE BETWEEN '%1' AND '%2'").arg(m_date1.toString("dd.MM.yyyy")).arg(m_date2.toString("dd.MM.yyyy"));

    if (m_docId.length())
        where += " AND SH.ID IN (" + m_docId + ") ";

    if (m_operatorId.length())
        where += " AND SH.OPERATOR_ID IN (" + m_operatorId + ") ";

    if (m_positionId.length())
        where += " AND SB.POSITION_ID IN (" + m_positionId + ") ";

    if (m_employeeId.length())
        where += " AND SB.EMPLOYEE_ID IN (" + m_employeeId + ") ";

    select = "SELECT " + select + " SUM(SB.AMOUNT) AS AMOUNT ";
    select += "FROM SALARY_DOC_HEADER SH "
            "LEFT JOIN SALARY_DOC_BODY SB ON SH.ID=SB.DOC_ID "
            "LEFT JOIN POSITIONS P ON SB.POSITION_ID=P.ID "
            "LEFT JOIN EMPLOYEES E1 ON SH.OPERATOR_ID=E1.ID "
            "LEFT JOIN EMPLOYEES E2 ON SB.EMPLOYEE_ID=E2.ID ";
    select += where;

    if (group.length())
        group = "GROUP BY " + group.remove(group.length() - 1, 1);

    select += group;

    table->ExecuteQuery(select);
    table->setColumn("ID", tr("Id"), 100, DATA_INTEGER);
    table->setColumn("DOC_DATE", tr("Date"), 140, DATA_STRING);
    table->setColumn("OPERATOR_ID", tr("Operator id"), 0, DATA_INTEGER);
    table->setColumn("OPERATOR_NAME", tr("Operator"), 200, DATA_STRING);
    table->setColumn("POSITION_ID", tr("Position id"), 0, DATA_INTEGER);
    table->setColumn("POSITION_NAME", tr("Position"), 200, DATA_STRING);
    table->setColumn("EMPLOYEE_ID", tr("Employee id"), 0, DATA_INTEGER);
    table->setColumn("EMPLOYEE_NAME", tr("Employee"), 200, DATA_STRING);
    table->setColumn("AMOUNT", tr("Amount"), 150, DATA_DOUBLE);
}

void RSalary::slotNew()
{
    ESalaryDoc *e = new ESalaryDoc(this);
    e->show();
}

void RSalary::slotEdit()
{
    if (!m_groupDocId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Group by document id option must be enabled in the filter"));
        return;
    }

    QVector<int> rows;
    if (!table->checkRowsCount(rows, false))
        return;

    ESalaryDoc *e = new ESalaryDoc(this);

    QSqlQuery *q = table->openDB();
    q->prepare("SELECT SH.DOC_DATE, SH.DATE_RANGE_1, SH.DATE_RANGE_2, SH.OPERATOR_ID, E.NAME AS ENAME "
               "FROM SALARY_DOC_HEADER SH, EMPLOYEES E "
               "WHERE SH.OPERATOR_ID=E.ID AND SH.ID=:ID");
    q->bindValue("ID", table->m_model.Integer("ID", rows[0]));
    q->exec();
    q->next();
    e->m_pDocId->setText(table->m_model.String("ID", rows[0]));
    e->m_pDate->setDate(q->value(0).toDate());
    e->m_pDate1->setDate(q->value(1).toDate());
    e->m_pDate2->setDate(q->value(2).toDate());
    e->m_pOperator->setText(q->value(4).toString());

    q->prepare("SELECT SB.POSITION_ID, P.NAME AS PNAME, SB.EMPLOYEE_ID, E.NAME AS ENAME, AMOUNT "
               "FROM SALARY_DOC_BODY SB "
               "LEFT JOIN POSITIONS P ON SB.POSITION_ID=P.ID "
               "LEFT JOIN EMPLOYEES E ON SB.EMPLOYEE_ID=E.ID "
               "WHERE SB.DOC_ID=:DOC_ID");
    q->bindValue("DOC_ID", table->m_model.Integer("ID", rows[0]));
    q->exec();
    while (q->next())
    {
        int row = e->m_pDoc->addRow();
        e->m_pDoc->m_model.setData(row, "POSITION_ID", q->value(0));
        e->m_pDoc->m_model.setData(row, "POSITION_NAME", q->value(1));
        e->m_pDoc->m_model.setData(row, "EMPLOYEE_ID", q->value(2));
        e->m_pDoc->m_model.setData(row, "EMPLOYEE_NAME", q->value(3));
        e->m_pDoc->m_model.setData(row, "AMOUNT", q->value(4));
    }
    table->closeDB(q);

    e->countTotal();
    e->m_isDocSaved = true;
    e->show();
}

void RSalary::slotDelete()
{
    if (!m_groupDocId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Group by document id option must be enabled in the filter"));
        return;
    }

    QVector<int> rows;
    if (!table->checkRowsCount(rows))
        return;

    if (QMessageBox::warning(this, tr("Waring"), tr("Are you sure to delete selected documents?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QSqlQuery *q = table->openDB();
    for (int i = rows.count() - 1; i > -1; i--)
    {
        q->prepare("DELETE FROM SALARY_DOC_BODY WHERE DOC_ID=:DOC_ID");
        q->bindValue("DOC_ID", table->m_model.Integer("ID", rows[i]));
        q->exec();
        q->prepare("DELETE FROM SALARY_DOC_HEADER WHERE ID=:ID");
        q->bindValue("ID", table->m_model.Integer("ID", rows[i]));
        q->exec();
    }
    getData();
}

void RSalary::slotFilter()
{
    FSalary *f = new FSalary(this);
    f->m_pDocId->setText(m_docId);
    f->m_pDate1->setDate(m_date1);
    f->m_pDate2->setDate(m_date2);
    f->m_pOperator->setText(m_operatorName);
    f->m_operator = m_operatorId;
    f->m_pPosition->setText(m_positionName);
    f->m_position = m_positionId;
    f->m_pEmployee->setText(m_employeeName);
    f->m_employee = m_employeeId;
    f->m_pGroupDocId->setChecked(m_groupDocId);
    f->m_pGroupDate->setChecked(m_groupDate);
    f->m_pGroupOperator->setChecked(m_groupOperator);
    f->m_pGroupPosition->setChecked(m_groupPosition);
    f->m_pGroupEmployee->setChecked(m_groupEmployee);

    if (f->exec() == QDialog::Accepted)
    {
        m_docId = f->m_pDocId->text();
        m_date1 = f->m_pDate1->date();
        m_date2 = f->m_pDate2->date();
        m_operatorName = f->m_pOperator->text();
        m_operatorId = f->m_operator;
        m_positionName = f->m_pPosition->text();
        m_positionId = f->m_position;
        m_employeeName = f->m_pEmployee->text();
        m_employeeId = f->m_employee;
        m_groupDocId = f->m_pGroupDocId->isChecked();
        m_groupDate = f->m_pGroupDate->isChecked();
        m_groupOperator = f->m_pGroupOperator->isChecked();
        m_groupPosition = f->m_pGroupPosition->isChecked();
        m_groupEmployee = f->m_pGroupEmployee->isChecked();
        getData();
    }
    delete f;
}

void RSalary::slotTotal()
{
    double total_amount = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
        total_amount += table->m_model.Double("AMOUNT", i);
    QString msg = QString("%1: %2\n%3: %4")
            .arg(tr("Total records"))
            .arg(table->m_model.rowCount(QModelIndex()))
            .arg(tr("Total amount"))
            .arg(total_amount);

    QMessageBox::information(this, tr("Total"), msg);
}
