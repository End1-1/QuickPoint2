#include "fsalary.h"
#include "checklist2.h"

FSalary::FSalary(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Salary filter"));
    int i = -1;

    QGridLayout *layoutMain = new QGridLayout(this);
    m_pDocId = new QLineEdit(this);
    m_pDate1 = new QDateEdit(QDate::currentDate(), this);
    m_pDate2 = new QDateEdit(QDate::currentDate(), this);
    m_pOperator = new QLineEdit(this);
    m_pPosition = new QLineEdit(this);
    m_pEmployee = new QLineEdit(this);
    m_pGroupDocId = new QCheckBox(this);
    m_pGroupDate = new QCheckBox(this);
    m_pGroupOperator = new QCheckBox(this);
    m_pGroupPosition = new QCheckBox(this);
    m_pGroupEmployee = new QCheckBox(this);

    QPushButton *pbOperator = newBtn(this);
    QPushButton *pbPosition = newBtn(this);
    QPushButton *pbEmployee = newBtn(this);

    m_pOperator->setReadOnly(true);
    m_pPosition->setReadOnly(true);
    m_pEmployee->setReadOnly(true);

    connect(pbOperator, SIGNAL(clicked()), SLOT(operators()));
    connect(pbPosition, SIGNAL(clicked()), SLOT(positions()));
    connect(pbEmployee, SIGNAL(clicked()), SLOT(employees()));

    layoutMain->addWidget(new QLabel(tr("Document id"), this), ++i, 0);
    layoutMain->addWidget(m_pDocId, i, 1, 1, 2);
    layoutMain->addWidget(m_pGroupDocId, i, 4);
    layoutMain->addWidget(new QLabel(tr("Date range"), this), ++i, 0);
    layoutMain->addWidget(m_pDate1, i, 1);
    layoutMain->addWidget(m_pDate2, i, 2);
    layoutMain->addWidget(m_pGroupDate, i, 4);
    layoutMain->addWidget(new QLabel(tr("Operator"), this), ++i, 0);
    layoutMain->addWidget(m_pOperator, i, 1, 1, 2);
    layoutMain->addWidget(pbOperator, i, 3);
    layoutMain->addWidget(m_pGroupOperator, i, 4);
    layoutMain->addWidget(new QLabel(tr("Position"), this), ++i, 0);
    layoutMain->addWidget(m_pPosition, i, 1, 1, 2);
    layoutMain->addWidget(pbPosition, i, 3);
    layoutMain->addWidget(m_pGroupPosition, i, 4);
    layoutMain->addWidget(new QLabel(tr("Employee"), this), ++i, 0);
    layoutMain->addWidget(m_pEmployee, i, 1, 1, 2);
    layoutMain->addWidget(pbEmployee, i, 3);
    layoutMain->addWidget(m_pGroupEmployee, i, 4);
    layoutMain->addWidget(m_pwidget, ++i, 0, 1, 4);
    setLayout(layoutMain);
}

bool FSalary::checkData()
{
    return true;
}

void FSalary::operators()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM EMPLOYEES", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_operator = "";
        m_pOperator->clear();
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_operator = QString("%1,'%2'").arg(m_operator).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_pOperator->setText(QString("%1,%2").arg(m_pOperator->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_operator.length() > 0)
        {
            m_operator = m_operator.remove(0, 1);
            m_pOperator->setText(m_pOperator->text().remove(0, 1));
        }
    }
    delete c;
}

void FSalary::positions()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM POSITIONS", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_position = "";
        m_pPosition->clear();
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_position = QString("%1,'%2'").arg(m_position).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_pPosition->setText(QString("%1,%2").arg(m_pPosition->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_position.length() > 0)
        {
            m_position = m_position.remove(0, 1);
            m_pPosition->setText(m_pPosition->text().remove(0, 1));
        }
    }
    delete c;
}

void FSalary::employees()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM EMPLOYEES", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_employee = "";
        m_pEmployee->clear();
        for (QMap<int, QVariant>::const_iterator i = c->m_result.constBegin(); i != c->m_result.constEnd(); i++)
        {
            m_employee = QString("%1,'%2'").arg(m_employee).arg(c->m_pt->m_model.data("ID", i.key()).toString());
            m_pEmployee->setText(QString("%1,%2").arg(m_pEmployee->text()).arg(c->m_pt->m_model.data("NAME", i.key()).toString()));
        }
        if (m_employee.length() > 0)
        {
            m_employee = m_employee.remove(0, 1);
            m_pEmployee->setText(m_pEmployee->text().remove(0, 1));
        }
    }
    delete c;
}
