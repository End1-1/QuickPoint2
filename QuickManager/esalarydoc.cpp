#include "esalarydoc.h"
#include "aemployeesalary.h"
#include "esalaryformula.h"
#include "global.h"
#include "../All/econnection.h"

#include <QtGui>

ESalaryDoc::ESalaryDoc(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Salary"));
    m_isDocSaved = true;

    int i = -1;
    QVBoxLayout *layoutMain = new QVBoxLayout(this);
    QGridLayout *layoutHeader = new QGridLayout(this);
    QVBoxLayout *layoutEmployees = new QVBoxLayout(this);

    QHBoxLayout *l1 = new QHBoxLayout(this);
    QWidget *w1 = new QWidget(this);
    QPushButton *pbNew = new QPushButton(tr("New"), this);
    QPushButton *pbEdit = new QPushButton(tr("Edit"), this);
    QPushButton *pbDelete = new QPushButton(tr("Delete"), this);

    connect(pbNew, SIGNAL(clicked()), SLOT(newEmployee()));
    connect(pbEdit, SIGNAL(clicked()), SLOT(editEmployee()));
    connect(pbDelete, SIGNAL(clicked()), SLOT(deleteEmployee()));

    l1->setMargin(0);
    l1->addWidget(pbNew);
    l1->addWidget(pbEdit);
    l1->addWidget(pbDelete);
    l1->addStretch(1);
    w1->setLayout(l1);

    QWidget *wdtTotal = new QWidget(this);
    QHBoxLayout *lTotal = new QHBoxLayout(this);
    m_pTotal = new QLineEdit(this);
    m_pTotal->setReadOnly(true);

    lTotal->addStretch(1);
    lTotal->addWidget(new QLabel(tr("Total amount:")));
    lTotal->addWidget(m_pTotal);
    wdtTotal->setLayout(lTotal);

    m_pDoc = new TableView(this);
    EConnection::setConnectionParams(m_pDoc->m_db.db, Data::databaseIndex);
    m_pDoc->setMinimumSize(700, 400);
    m_pDoc->m_model.insertColumns(0, 5, QModelIndex());
    m_pDoc->setColumn(++i, "EMPLOYEE_ID", tr("Employee id"), 0, DATA_INTEGER);
    m_pDoc->setColumn(++i, "EMPLOYEE_NAME", tr("Employee"), 300, DATA_STRING);
    m_pDoc->setColumn(++i, "POSITION_ID", tr("Position id"), 0, DATA_INTEGER);
    m_pDoc->setColumn(++i, "POSITION_NAME", tr("Position"), 250, DATA_STRING);
    m_pDoc->setColumn(++i, "AMOUNT", tr("Amount"), 100, DATA_DOUBLE);

    layoutEmployees->addWidget(w1);
    layoutEmployees->addWidget(m_pDoc);
    layoutEmployees->addWidget(wdtTotal);
    QWidget *wEmployees = new QWidget(this);
    wEmployees->setLayout(layoutEmployees);


    QHBoxLayout *l2 = new QHBoxLayout(this);
    QWidget *w2 = new QWidget(this);
    QPushButton *pbCount = new QPushButton(tr("Count"), this);
    QPushButton *pbPrint = new QPushButton(tr("Print"), this);
    QPushButton *pbSave = new QPushButton(tr("Save"), this);
    QPushButton *pbClose = new QPushButton(tr("Close"), this);

    connect(pbCount, SIGNAL(clicked()), SLOT(countSalary()));
    connect(pbPrint, SIGNAL(clicked()), SLOT(printDoc()));
    connect(pbSave, SIGNAL(clicked()), SLOT(saveDoc()));
    connect(pbClose, SIGNAL(clicked()), SLOT(closeDoc()));

    l2->addStretch(1);
    l2->addWidget(pbCount);
    l2->addWidget(pbPrint);
    l2->addWidget(pbSave);
    l2->addWidget(pbClose);
    l2->addStretch(1);
    w2->setLayout(l2);

    QWidget *w3 = new QWidget(this);
    m_pDocId = new QLineEdit(this);
    m_pOperator = new QLineEdit(this);
    m_pDate = new QDateEdit(QDate::currentDate(), this);
    m_pDate1 = new QDateEdit(QDate::currentDate(), this);
    m_pDate2 = new QDateEdit(QDate::currentDate(), this);

    m_pDocId->setReadOnly(true);
    m_pOperator->setReadOnly(true);
    m_pOperator->setText(M::user.Name);

    connect(m_pDate, SIGNAL(dateChanged(QDate)), SLOT(docChanged()));
    connect(m_pDate1, SIGNAL(dateChanged(QDate)), SLOT(docChanged()));
    connect(m_pDate2, SIGNAL(dateChanged(QDate)), SLOT(docChanged()));

    i = -1;
    layoutHeader->setColumnMinimumWidth(1, 200);
    layoutHeader->setColumnMinimumWidth(2, 200);
    layoutHeader->addWidget(new QLabel(tr("Document id"), this), ++i, 0);
    layoutHeader->addWidget(m_pDocId, i, 1);
    layoutHeader->addWidget(new QLabel(tr("Operator"), this), ++i, 0);
    layoutHeader->addWidget(m_pOperator, i, 1);
    layoutHeader->addWidget(new QLabel(tr("Document date"), this), ++i, 0);
    layoutHeader->addWidget(m_pDate, i, 1);
    layoutHeader->addWidget(new QLabel(tr("Date range"), this), ++i, 0);
    layoutHeader->addWidget(m_pDate1, i, 1);
    layoutHeader->addWidget(m_pDate2, i, 2);
    layoutHeader->setRowStretch(++i, 1);
    layoutHeader->setColumnStretch(3, 1);
    w3->setLayout(layoutHeader);

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(w3, tr("Main"));
    tab->addTab(wEmployees, tr("List of employees"));

    layoutMain->addWidget(tab);
    layoutMain->addWidget(w2);
    setLayout(layoutMain);
}

void ESalaryDoc::newEmployee()
{
    AEmployeeSalary *a = new AEmployeeSalary(this);
    bool show_next = false;
    do {
        if (a->exec() == QDialog::Accepted)
        {
            m_isDocSaved = false;
            show_next = a->m_showNext;
            int newRow = m_pDoc->addRow();
            setData(a, newRow);
            a->m_pEmployee->setCurrentIndex(-1);
            a->m_pAmount->setText("");
            countTotal();
        }
        else
            show_next = false;
    }
    while (show_next);
    delete a;
}

void ESalaryDoc::editEmployee()
{
    QVector<int> rows;
    if (!m_pDoc->checkRowsCount(rows, false))
        return;
    AEmployeeSalary *a = new AEmployeeSalary(this);
    a->m_pEmployee->setCurrentIndex(a->m_pEmployee->findText(m_pDoc->m_model.String("EMPLOYEE_NAME", rows[0])));
    a->m_pPosition->setCurrentIndex(a->m_pPosition->findText(m_pDoc->m_model.String("POSITION_NAME", rows[0])));
    a->m_pAmount->setText(m_pDoc->m_model.String("AMOUNT", rows[0]));
    if (a->exec() == QDialog::Accepted)
    {
        m_isDocSaved = false;
        setData(a, rows[0]);
        a->m_pEmployee->setCurrentIndex(-1);
        a->m_pAmount->setText("");
        countTotal();
    }
    delete a;
}

void ESalaryDoc::deleteEmployee()
{
    QVector<int> rows;
    if (!m_pDoc->checkRowsCount(rows))
        return;
    if (QMessageBox::question(this, tr("Confirm action"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
    for (int i = rows.count() - 1; i > -1; i--)
        m_pDoc->m_model.removeRows(rows[i], 1, QModelIndex());
    m_isDocSaved = false;
}

void ESalaryDoc::countSalary()
{
    m_isDocSaved = false;
    QMap<int, QVector<SalaryFormula> > salaryFormula;

    for (int i = 0; i < m_pDoc->m_model.rowCount(QModelIndex()); i++)
    {
        //Check exists formula
        int positionID = m_pDoc->m_model.Integer("POSITION_ID", i);
        if (!salaryFormula.contains(positionID))
        {
            QSqlQuery *q = m_pDoc->openDB();
            QVector<SalaryFormula> s;
            q->prepare("SELECT OPERATION, PARAMS, CELL FROM SALARY_FORMULA WHERE POSITION_ID=:POSITION_ID ORDER BY QUEUE");
            q->bindValue(":POSITION_ID", positionID);
            q->exec();
            while (q->next())
            {
                SalaryFormula sf;
                sf.operation = q->value(0).toString();
                if (q->value(0).toString() == OP_MANUAL)
                {
                    s.push_back(sf);
                    break;
                }
                sf.params = q->value(1).toString();
                sf.cell = q->value(2).toString();
                s.push_back(sf);
            }
            m_pDoc->closeDB(q);
            salaryFormula.insert(positionID, s);
        }
        //Get a set of formulas
        QVector<SalaryFormula> &t = salaryFormula[positionID];
        if (t.count() > 0 && t.at(0).operation != OP_MANUAL)
        {
            ESalaryFormulaCounter *e = new ESalaryFormulaCounter(t);
            e->setDateRange(m_pDate1->date(), m_pDate2->date());
            double d = e->counted();
            m_pDoc->m_model.setData(i, "AMOUNT", d);
            delete e;
        }
    }
    countTotal();
}

void ESalaryDoc::printDoc()
{
    QMessageBox::information(this, "Error", "Not implemented");
}

void ESalaryDoc::saveDoc()
{
    if (!checkDoc())
        return;

    QSqlQuery *q = m_pDoc->openDB();

    if (!m_pDocId->text().length())
    {
        q->exec("SELECT GEN_ID(GEN_SALARY_DOC_HEADER_ID,1) FROM RDB$DATABASE");
        q->next();
        m_pDocId->setText(q->value(0).toString());
    }

    q->prepare("DELETE FROM SALARY_DOC_HEADER WHERE ID=:ID");
    q->bindValue(":ID", m_pDocId->text());
    q->exec();

    q->prepare("DELETE FROM SALARY_DOC_BODY WHERE DOC_ID=:DOC_ID");
    q->bindValue(":DOC_ID", m_pDocId->text());
    q->exec();

    q->prepare("INSERT INTO SALARY_DOC_HEADER (ID, DOC_DATE, DATE_RANGE_1, DATE_RANGE_2, OPERATOR_ID) VALUES (:ID, :DOC_DATE, :DATE_RANGE_1, :DATE_RANGE_2, :OPERATOR)");
    q->bindValue(":ID", m_pDocId->text());
    q->bindValue(":DOC_DATE", m_pDate->date());
    q->bindValue(":DATE_RANGE_1", m_pDate1->date());
    q->bindValue(":DATE_RANGE_2", m_pDate2->date());
    q->bindValue(":OPERATOR", M::user.Id);
    q->exec();

    q->prepare("INSERT INTO SALARY_DOC_BODY (DOC_ID, POSITION_ID, EMPLOYEE_ID, AMOUNT) VALUES (:DOC_ID, :POSITION_ID, :EMPLOYEE_ID, :AMOUNT)");
    for (int i = 0; i < m_pDoc->m_model.rowCount(QModelIndex()); i++)
    {
        q->bindValue(":DOC_ID", m_pDocId->text());
        q->bindValue(":POSITION_ID", m_pDoc->m_model.Integer("POSITION_ID", i));
        q->bindValue(":EMPLOYEE_ID", m_pDoc->m_model.Integer("EMPLOYEE_ID", i));
        q->bindValue(":AMOUNT", m_pDoc->m_model.Double("AMOUNT", i));
        q->exec();
    }

    m_pDoc->closeDB(q);

    QMessageBox::information(this, tr("Information"), tr("Document saved"));
    m_isDocSaved = true;
}

void ESalaryDoc::closeDoc()
{
    if (!m_isDocSaved)
        if (QMessageBox::question(this, tr("Confirm exit"), tr("Document is not saved, continiue?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
    close();
}

void ESalaryDoc::docChanged()
{
    m_isDocSaved = false;
}

void ESalaryDoc::setData(AEmployeeSalary *a, const int &row)
{
    m_pDoc->m_model.setData(row, "EMPLOYEE_ID", a->m_pEmployee->itemData(a->m_pEmployee->currentIndex()));
    m_pDoc->m_model.setData(row, "EMPLOYEE_NAME", a->m_pEmployee->currentText());
    m_pDoc->m_model.setData(row, "POSITION_ID", a->m_pPosition->itemData(a->m_pPosition->currentIndex()));
    m_pDoc->m_model.setData(row, "POSITION_NAME", a->m_pPosition->currentText());
    m_pDoc->m_model.setData(row, "AMOUNT", a->m_pAmount->text().toDouble());
}

bool ESalaryDoc::checkDoc()
{
    bool result = true;
    QString msg;

    for (int i = 0; i < m_pDoc->m_model.rowCount(QModelIndex()); i++)
        if (m_pDoc->m_model.Double("AMOUNT", i) <= 0)
        {
            msg += tr("Salary of some employees is 0\n");
            result = false;
        }

    if (!m_pDoc->m_model.rowCount(QModelIndex()))
    {
        msg += tr("Empty document\n");
        result = false;
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void ESalaryDoc::countTotal()
{
    double total = 0;
    for (int i = 0; i < m_pDoc->m_model.rowCount(QModelIndex()); i++)
        total += m_pDoc->m_model.Double("AMOUNT", i);
    m_pTotal->setText(QString::number(total, 'f', 2));
}
