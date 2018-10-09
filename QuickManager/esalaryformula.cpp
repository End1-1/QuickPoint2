#include "esalaryformula.h"
#include "global.h"
#include "../All/econnection.h"

ESalaryFormula::ESalaryFormula(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Salary formula"));
    QGridLayout *l = new QGridLayout(this);
    int i = -1;

    m_pQueue = new QLineEdit(this);
    m_pOperation = new QComboBox(this);
    m_pParams = new QLineEdit(this);
    m_pCell = new QComboBox(this);

    getSalaryOperations();
    getMemoryCells();
    m_pQueue->setValidator(new QIntValidator(this));

    l->addWidget(new QLabel(tr("Queue"), this), ++i, 0);
    l->addWidget(m_pQueue, i, 1);
    l->addWidget(new QLabel(tr("Operation"), this), ++i, 0);
    l->addWidget(m_pOperation, i, 1);
    l->addWidget(new QLabel(tr("Params"), this), ++i, 0);
    l->addWidget(m_pParams, i, 1);
    l->addWidget(new QLabel(tr("Result cell"), this), ++i, 0);
    l->addWidget(m_pCell, i, 1);
    l->addWidget(m_pwidget, ++i, 0, 1, 2);

    setLayout(l);
}

bool ESalaryFormula::checkData()
{
    return true;
}

void ESalaryFormula::getSalaryOperations()
{
    QSqlQuery *q = m_db.QuerySelect("SELECT NAME FROM SALARY_OPS ORDER BY NAME");
    while (q->next())
        m_pOperation->addItem(q->value(0).toString());
    m_db.close(q);
}

void ESalaryFormula::getMemoryCells()
{
    for (int i = 1; i < 17; i++)
        m_pCell->addItem(QString("M%1").arg(i));
}

ESalaryFormulaCounter::ESalaryFormulaCounter(QVector<SalaryFormula> &formulas) :
    m_formulas(formulas)
{
    EConnection::setConnectionParams(db, Data::databaseIndex);
   // m_query = openDB();

    for (int i = 0; i < mem_cell_count; i++)
        m[i] = 0;
}

ESalaryFormulaCounter::~ESalaryFormulaCounter()
{
    close(m_query);
}

void ESalaryFormulaCounter::setDateRange(const QDate &date1, const QDate &date2)
{
    m_date1 = date1;
    m_date2 = date2;
}

double ESalaryFormulaCounter::counted()
{
    for (int i = 0; i < m_formulas.count(); i++)
    {
        SalaryFormula op = m_formulas.at(i);
        if (op.operation == "DISH_QTY")
            fn_dish_qty(op.params, op.cell);
        else if (op.operation == "MULTIPLICATION")
            fn_multiplication(op.params, op.cell);
        else if (op.operation == "RETURN")
            return fn_return(op.params);
    }
    return -1;
}

int ESalaryFormulaCounter::indexM(QString cell)
{
    cell = cell.remove(0, 1);
    return cell.toInt() - 1;
}

bool ESalaryFormulaCounter::isMemCell(const QString &cell)
{
    return cell.contains(QRegExp("^(m|M)([1-9]$|[1-9][0-6])"));
}

double ESalaryFormulaCounter::fn_dish_qty(const QString &params, const QString &cell)
{
    QString sql_cond = QString(" WHERE S.DISH_ID=D.ID AND CASH_DATE BETWEEN '%1' AND '%2' ")
            .arg(m_date1.toString("dd.MM.yyyy"))
            .arg(m_date2.toString("dd.MM.yyyy"));
    QStringList p = params.toUpper().split(";");
    for (QStringList::const_iterator i = p.begin(); i != p.end(); i++)
    {
        QStringList pp = i->split("=");
        if (pp[0] == "DISH_ID")
            sql_cond += " AND S.DISH_ID IN (" + pp[1] + ") ";
        else if (pp[0] == "S.CASH_ID")
            sql_cond += " AND CASH_ID IN (" + pp[1] + ") ";
        else if (pp[0] == "STORE_ID")
            sql_cond += " AND D.STORE_ID IN (" + pp[2] + ") ";
        else
            m_errors.push_back(QString("Uknown parametr in dish_qty function: %1").arg(params));
    }
    QString sql_query = "SELECT SUM(S.QTY) FROM SALE S, DISHES D " + sql_cond;
    m_query->exec(sql_query);
    m_query->next();
    m[indexM(cell)] = m_query->value(0).toDouble();
    return m[indexM(cell)];
}

double ESalaryFormulaCounter::fn_multiplication(const QString &params, const QString &cell)
{
    QStringList values = params.split(",");
    if (values.count() < 2)
    {
        m[indexM(cell)] = 0;
        return 0;
    }

    double result = 0;
    bool first = true;
    for (QStringList::const_iterator i = values.begin(); i != values.end(); i++)
    {
        if (first)
        {
            first = false;
            if (isMemCell(*i))
                result = m[indexM(*i)];
            else
                result = i->toDouble();
            i++;
            if (isMemCell(*i))
                result *= m[indexM(*i)];
            else
                result *= i->toDouble();
            continue;
        }

        if (isMemCell(*i))
            result *= m[indexM(*i)];
        else
            result *= i->toDouble();
    }
    m[indexM(cell)] = result;
    return result;
}

double ESalaryFormulaCounter::fn_return(const QString &params)
{

    if (isMemCell(params))
        return m[indexM(params)];
    else
        return params.toDouble();
}
