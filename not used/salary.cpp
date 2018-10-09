#include "salary.h"
#include "digits.h"
#include "../All/global.h"

Salary::Salary(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
{
    m_viewPosition = true;
    m_doc_id = 0;

    QWidget *wdtEmployeesList = new QWidget(this);
    QWidget *wdtToolbarEmployeesList = new QWidget(this);
    QWidget *wdtDoc = new QWidget(this);
    QWidget *wdtDocUpToolbar = new QWidget(this);
    QWidget *wdtDocToolbarBottom = new QWidget(this);
    QWidget *wdtBottomToolbar = new QWidget(this);

    QGridLayout *layoutMain = new QGridLayout(this);
    QVBoxLayout *layoutEmployeesList = new QVBoxLayout(wdtEmployeesList);
    QHBoxLayout *layoutEmployeesListToolbar = new QHBoxLayout(wdtToolbarEmployeesList);
    QVBoxLayout *layoutDoc = new QVBoxLayout(wdtDoc);
    QHBoxLayout *layoutDocUpToolbar = new QHBoxLayout(wdtDocUpToolbar);
    QHBoxLayout *layoutDocToolbar = new QHBoxLayout(wdtDocToolbarBottom);
    QHBoxLayout *layoutBottom = new QHBoxLayout(wdtBottomToolbar);

    m_query = new QSqlQuery();
    m_pEmployees = new QListWidget(this);
    m_pDoc = new QListWidget(this);
    m_pTotal = new QLabel(tr("Total"), this);

    QPushButton *pbSetSalary = new QPushButton(tr("Set salary"), this);
    QPushButton *pbDeleteRow = new QPushButton(tr("Delete row"), this);
    QPushButton *pbPositions = new QPushButton(tr("Position"), this);
    QPushButton *pbClose = new QPushButton(tr("Close"), this);
    QPushButton *pbSave = new QPushButton(tr("Save"), this);

    m_pEmployees->setItemDelegate(new EmployeesListDelegate(*this, this));
    m_pDoc->setItemDelegate(new SalaryDocDelegate(*this, this));
    QSize s(80, 50);
    pbSetSalary->setMinimumSize(s);
    pbSetSalary->setStyleSheet("font: bold 22px");
    pbDeleteRow->setMinimumSize(s);
    pbDeleteRow->setStyleSheet("font: bold 22px");
    pbPositions->setMinimumSize(s);
    pbPositions->setStyleSheet("font: bold 22px");
    pbClose->setMinimumSize(s);
    pbClose->setStyleSheet("font: bold 22px");
    pbSave->setMinimumSize(s);
    pbSave->setStyleSheet("font: bold 22px");
    m_pTotal->setStyleSheet("font: bold 22px");

    layoutDocUpToolbar->setMargin(0);
    layoutEmployeesListToolbar->setMargin(0);
    layoutDocToolbar->setMargin(0);
    wdtDocToolbarBottom->setMinimumSize(s);

    connect(pbSetSalary, SIGNAL(clicked()), SLOT(editSalaryAmount()));
    connect(pbDeleteRow, SIGNAL(clicked()), SLOT(deleteRow()));
    connect(pbPositions, SIGNAL(clicked()), SLOT(positions()));
    connect(pbClose, SIGNAL(clicked()), SLOT(tryClose()));
    connect(pbSave, SIGNAL(clicked()), SLOT(save()));
    connect(m_pEmployees, SIGNAL(clicked(QModelIndex)), SLOT(employeesClick(QModelIndex)));
    makePositionsList();
    makeEmployeesList();
    positions();
    loadDoc();

    layoutEmployeesListToolbar->addWidget(pbPositions);
    wdtToolbarEmployeesList->setLayout(layoutEmployeesListToolbar);

    layoutEmployeesList->addWidget(m_pEmployees);
    layoutEmployeesList->addWidget(wdtToolbarEmployeesList);
    wdtEmployeesList->setLayout(layoutEmployeesList);

    layoutDocUpToolbar->addWidget(pbSetSalary);
    layoutDocUpToolbar->addWidget(pbDeleteRow);
    layoutDocUpToolbar->addStretch(1);
    wdtDocUpToolbar->setLayout(layoutDocUpToolbar);

    layoutDocToolbar->addWidget(m_pTotal);
    wdtDocToolbarBottom->setLayout(layoutDocToolbar);

    layoutDoc->addWidget(wdtDocUpToolbar);
    layoutDoc->addWidget(m_pDoc);
    layoutDoc->addWidget(wdtDocToolbarBottom);
    wdtDoc->setLayout(layoutDoc);

    layoutBottom->addWidget(pbClose);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(pbSave);
    wdtBottomToolbar->setLayout(layoutBottom);

    layoutMain->addWidget(wdtEmployeesList, 0, 0);
    layoutMain->addWidget(wdtDoc, 0, 1);
    layoutMain->addWidget(wdtBottomToolbar, 1, 0, 1, 2);
    setLayout(layoutMain);
}

Salary::~Salary()
{
    delete m_query;
}

void Salary::makePositionsList()
{
    m_query->exec("SELECT ID, NAME FROM POSITIONS ORDER BY NAME");
    while (m_query->next())
    {
        Position p;
        p.id = m_query->value(0).toInt();
        p.name = m_query->value(1).toString();
        m_positions.push_back(p);
    }
}

void Salary::positions()
{
    m_pEmployees->clear();
    for (int i = 0; i < m_positions.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(m_positions.at(i).name, m_pEmployees);
        item->setSizeHint(QSize(0, 50));
    }
    m_viewPosition = true;
}

void Salary::save()
{
    if (!m_doc_items.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Empty document"));
        return;
    }

    for (QVector<Employee>::const_iterator i = m_doc_items.begin(); i != m_doc_items.end(); i++)
        if (i->salary <= 0)
        {
            QMessageBox::critical(this, tr("Error"), tr("Some salary values is not set"));
            return;
        }

    QSqlQuery *q = new QSqlQuery();

    if (!m_doc_id)
    {
        q->exec("SELECT GEN_ID(GEN_SALARY_DOC_HEADER_ID,1) FROM RDB$DATABASE");
        q->next();
        m_doc_id = q->value(0).toInt();
    }

    q->prepare("DELETE FROM SALARY_DOC_BODY WHERE DOC_ID=:DOC_ID");
    q->bindValue("DOC_ID", m_doc_id);
    q->exec();

    q->prepare("DELETE FROM SALARY_DOC_HEADER WHERE ID=:ID");
    q->bindValue("ID",m_doc_id);
    q->exec();

    q->prepare("INSERT INTO SALARY_DOC_HEADER (ID, DOC_DATE, DATE_RANGE_1, DATE_RANGE_2, OPERATOR_ID) VALUES (:ID, :DOC_DATE, :DATE_RANGE_1, :DATE_RANGE_2, :OPERATOR)");
    q->bindValue("ID", m_doc_id);
    q->bindValue("DOC_DATE", Data::cashDate());
    q->bindValue("DATE_RANGE_1", Data::cashDate());
    q->bindValue("DATE_RANGE_2", Data::cashDate());
    q->bindValue("OPERATOR", Data::operatorID);
    q->exec();

    q->prepare("INSERT INTO SALARY_DOC_BODY (DOC_ID, POSITION_ID, EMPLOYEE_ID, AMOUNT) VALUES (:DOC_ID, :POSITION_ID, :EMPLOYEE_ID, :AMOUNT)");
    for (int i = 0; i < m_doc_items.count(); i++)
    {
        q->bindValue("DOC_ID", m_doc_id);
        q->bindValue("POSITION_ID", m_doc_items[i].position_id);
        q->bindValue("EMPLOYEE_ID", m_doc_items[i].id);
        q->bindValue("AMOUNT", m_doc_items[i].salary);
        q->exec();
    }

    QMessageBox::information(this, tr("Information"), tr("Document saved"));
}

void Salary::tryClose()
{
    accept();
}

void Salary::employeesClick(const QModelIndex &index)
{
    if (m_viewPosition)
        selectPosition(index);
    else
        selectEmployee(index);
}


void Salary::makeEmployeesList()
{
    m_query->exec("SELECT ID, POSITION_ID, NAME FROM EMPLOYEES ORDER BY NAME");
    while (m_query->next())
    {
        Employee e;
        e.id = m_query->value(0).toInt();
        e.position_id = m_query->value(1).toInt();
        e.name = m_query->value(2).toString();
        e.salary = 0;
        m_employees.push_back(e);
    }
}

void Salary::selectPosition(const QModelIndex &index)
{
    if (index.row() < 0)
        return;
    int position_id = m_positions.at(index.row()).id;
    m_pEmployees->clear();
    for (int i = 0; i < m_employees.count(); i++)
        if ( position_id == m_employees[i].position_id)
        {
            QListWidgetItem *item = new QListWidgetItem(m_employees[i].name, m_pEmployees);
            item->setData(Qt::UserRole, i);
            item->setSizeHint(QSize(0, 50));
        }
    m_viewPosition = false;
}

void Salary::selectEmployee(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int employee_index = m_pEmployees->item(index.row())->data(Qt::UserRole).toInt();
    QListWidgetItem *item = new QListWidgetItem(m_employees[employee_index].name, m_pDoc);
    item->setSizeHint(QSize(0, 50));
    m_doc_items.push_back(m_employees[employee_index]);
    countTotal();
    m_pDoc->setCurrentRow(m_pDoc->count() - 1);
}

void Salary::countTotal()
{
    double total = 0;
    for (int i = 0; i < m_doc_items.count(); i++)
        total += m_doc_items[i].salary;
    m_pTotal->setText(QString("%1: %2").arg(tr("Total amount")).arg(QString::number(total, 'f', 0)));
}

void Salary::loadDoc()
{
    QSqlQuery *q = new QSqlQuery();

    q->prepare("SELECT ID FROM SALARY_DOC_HEADER WHERE DOC_DATE=:DOC_DATE AND OPERATOR_ID=:OPERATOR_ID");
    q->bindValue("DOC_DATE", Data::cashDate());
    q->bindValue("OPERATOR_ID", Data::operatorID);
    q->exec();

    int doc_count = 0;
    while (q->next())
    {
        doc_count++;
        m_doc_id = q->value(0).toInt();

        if (doc_count > 1)
        {
            QMessageBox::critical(this, tr("Error"), tr("Several documents exits. You will create a new create document."));
            delete q;
            return;
        }
    }

    if (!m_doc_id)
        return;

    q->prepare("SELECT SB.POSITION_ID, SB.EMPLOYEE_ID, E.NAME, SB.AMOUNT "
               "FROM SALARY_DOC_BODY SB, EMPLOYEES E "
               "WHERE SB.EMPLOYEE_ID=E.ID AND SB.DOC_ID=:DOC_ID");
    q->bindValue("DOC_ID", m_doc_id);
    q->exec();
    while (q->next())
    {
        Employee e;
        e.position_id = q->value(0).toInt();
        e.id = q->value(1).toInt();
        e.name = q->value(2).toString();
        e.salary = q->value(3).toDouble();
        m_doc_items.push_back(e);
        QListWidgetItem *item = new QListWidgetItem(e.name, m_pDoc);
        item->setSizeHint(QSize(0, 50));
    }
    delete q;

    countTotal();
}

void Salary::editSalaryAmount()
{
    int row = m_pDoc->currentIndex().row();
    if (row < 0)
        return;

    double amount = m_doc_items[row].salary;
    if (Digits::value(amount))
    {
        m_doc_items[row].salary = amount;
        m_pDoc->update(m_pDoc->currentIndex());
        countTotal();
    }
}

void Salary::deleteRow()
{
    int row = m_pDoc->currentIndex().row();
    if (row < 0)
        return;

    QListWidgetItem *item = m_pDoc->item(row);
    delete (item);
    m_doc_items.remove(row);
    countTotal();
}


EmployeesListDelegate::EmployeesListDelegate(Salary &salary, QObject *parent) :
    QItemDelegate(parent), m_salary(salary)
{
}

void EmployeesListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.row() < 0)
        return;

    QRect rect(option.rect);

    QBrush brush;
    QColor col_selected = QColor::fromRgb(122, 174, 205);
    QColor col_unselected = Qt::white;
    brush.setColor(option.state & QStyle::State_Selected ? col_selected : col_unselected);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);

    QPen pen;
    pen.setWidth(1);
    painter->setPen(pen);

    QFont font(Data::settings[SETTINGS_FONT_NAME].toString(), Data::settings[SETTINGS_FONT_SIZE].toInt());
    painter->setFont(font);

    QString str = m_salary.m_pEmployees->item(index.row())->text();

    painter->drawRect(rect);
    rect.adjust(5, 4, 0, 0);
    painter->drawText(rect, str);
}

void SalaryDocDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QRect rect = option.rect;

    QPen pen;
    pen.setWidth(1);
    painter->setPen(pen);

    QBrush brush;
    QColor col_selected = QColor::fromRgb(122, 174, 205);
    QColor col_unselected = Qt::white;
    brush.setColor(option.state & QStyle::State_Selected ? col_selected : col_unselected);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);

    QFont font(Data::settings[SETTINGS_FONT_NAME].toString(), Data::settings[SETTINGS_FONT_SIZE].toInt() + 5);
    QFontMetrics m(font);
    painter->setFont(font);

    QString employeeName = m_salary.m_pDoc->item(index.row())->text();
    QString employeeSalary = QString::number(m_salary.m_doc_items[index.row()].salary, 'f', 0);

    painter->drawRect(rect);
    rect.adjust(5, 4, 0, 0);
    painter->drawText(rect, employeeName);
    rect.setLeft(rect.right() - m.width(employeeSalary));
    painter->drawText(rect, employeeSalary);
}

SalaryDocDelegate::SalaryDocDelegate(Salary &salary, QObject *parent) :
    QItemDelegate(parent), m_salary(salary)
{
}
