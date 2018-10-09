#include "aemployeesalary.h"
#include "checklist2.h"
#include "esalaryformula.h"

AEmployeeSalary::AEmployeeSalary(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Employee salary"));
    m_EmployeeFilter = "";

    QGridLayout *layoutMain = new QGridLayout(this);
    QHBoxLayout *layoutBtn = new QHBoxLayout(this);
    QWidget *wdtBtn = new QWidget(this);

    QPushButton *pbDone = new QPushButton(tr("Done"), this);
    QPushButton *pbNext = new QPushButton(tr("Next"), this);
    QPushButton *pbClose = new QPushButton(tr("Close"), this);
    QPushButton *pbEmployeeFilter = newBtn(this);

    m_pEmployeeFilter = new QLineEdit(this);
    m_pEmployee = new QComboBox(this);
    m_pPosition = new QComboBox(this);
    m_pAmount = new QLineEdit(this);

    m_pAmount->setEnabled(false);
    makePositionList();
    makeEmployeesList();

    connect(pbDone, SIGNAL(clicked()), SLOT(done()));
    connect(pbNext, SIGNAL(clicked()), SLOT(next()));
    connect(pbClose, SIGNAL(clicked()), SLOT(close()));
    connect(pbEmployeeFilter, SIGNAL(clicked()), SLOT(employeeFilter()));
    connect(m_pEmployee, SIGNAL(currentIndexChanged(int)), SLOT(employeeChanged(int)));
    connect(m_pPosition, SIGNAL(currentIndexChanged(int)), SLOT(positionChanged(int)));

    layoutBtn->addStretch(1);
    layoutBtn->addWidget(pbDone);
    layoutBtn->addWidget(pbNext);
    layoutBtn->addWidget(pbClose);
    layoutBtn->addStretch(1);
    wdtBtn->setLayout(layoutBtn);

    int i = -1;
    layoutMain->setColumnMinimumWidth(1, 250);
    layoutMain->addWidget(new QLabel(tr("Position filter"), this), ++i, 0);
    layoutMain->addWidget(m_pEmployeeFilter, i, 1);
    layoutMain->addWidget(pbEmployeeFilter, i, 2);
    layoutMain->addWidget(new QLabel(tr("Employee"), this), ++i, 0);
    layoutMain->addWidget(m_pEmployee, i, 1);
    layoutMain->addWidget(new QLabel(tr("Position"), this), ++i, 0);
    layoutMain->addWidget(m_pPosition, i, 1);
    layoutMain->addWidget(new QLabel(tr("Amount"), this), ++i, 0);
    layoutMain->addWidget(m_pAmount, i, 1);
    layoutMain->addWidget(wdtBtn, ++i, 0, 1, 3);
    setLayout(layoutMain);
}

bool AEmployeeSalary::checkData()
{
    bool result = true;
    QString msg;

    if (m_pEmployee->currentIndex() < 0)
    {
        result = false;
        msg += tr("Employee name is not selected\n");
    }

    if (m_pPosition->currentIndex() < 0)
    {
        result = false;
        msg += tr("Position is not selected\n");
    }

    /* to do
    if (m_pPosition->currentIndex() == 0 && m_pAmount->text().toDouble() == 0)
    {
        result = false;
        msg += tr("Selected manual mode, but amount is 0\n");
    }
    */

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void AEmployeeSalary::makePositionList()
{
    QSqlQuery *q = m_db.QuerySelect("SELECT P.ID, P.NAME, SF.OPERATION FROM POSITIONS P, SALARY_FORMULA SF WHERE P.ID=SF.POSITION_ID ORDER BY NAME");
    while(q->next())
    {
        if (m_pPosition->findData(q->value(0)) < 0)
        {
            m_pPosition->addItem(q->value(1).toString(), q->value(0));
            m_salaryFormulas.insert(q->value(1).toString(), QVector<QString>());
        }
        m_salaryFormulas[q->value(1).toString()].push_back(q->value(2).toString());
    }
    m_db.close(q);
    positionChanged(0);
}

void AEmployeeSalary::makeEmployeesList()
{

    QString sql_query = "SELECT ID, NAME, POSITION_ID FROM EMPLOYEES WHERE ACTIVE=1 ";
    if (m_EmployeeFilter.length())
        sql_query += "AND POSITION_ID IN (" + m_EmployeeFilter + ")";
    QSqlQuery *q = m_db.QuerySelect(sql_query);
    m_EmployeePosition.clear();
    m_pEmployee->clear();
    disconnect(m_pEmployee, SIGNAL(currentIndexChanged(int)), this, SLOT(employeeChanged(int)));
    while (q->next())
    {
        m_pEmployee->addItem(q->value(1).toString(), q->value(0));
        m_EmployeePosition.push_back(q->value(2).toInt());
    }
    m_db.close(q);
    m_pEmployee->setCurrentIndex(-1);
    connect(m_pEmployee, SIGNAL(currentIndexChanged(int)), SLOT(employeeChanged(int)));
}

void AEmployeeSalary::employeeFilter()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM POSITIONS ORDER BY NAME", "ID", "NAME");
    if (c->exec())
    {
        QString values;
        c->returnResult(m_EmployeeFilter, values);
        m_pEmployeeFilter->setText(values);
    }
    delete c;
    makeEmployeesList();
}

void AEmployeeSalary::done()
{
    m_showNext = false;
    slotOk();
}

void AEmployeeSalary::next()
{
    m_showNext = true;
    slotOk();
}

void AEmployeeSalary::close()
{
    slotCancel();
}

void AEmployeeSalary::employeeChanged(const int &index)
{
    if (index < 0)
    {
        m_pPosition->setCurrentIndex(-1);
        return;
    }

    int employeePosition = m_EmployeePosition[index];
    m_pPosition->setCurrentIndex(m_pPosition->findData(employeePosition));
}

void AEmployeeSalary::positionChanged(const int &index)
{
    Q_UNUSED(index);
    m_pAmount->setEnabled(m_salaryFormulas[m_pPosition->currentText()].contains(OP_MANUAL));
}
