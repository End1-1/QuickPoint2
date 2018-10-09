#ifndef ESALARYFORMULA_H
#define ESALARYFORMULA_H

#include "editwindow.h"
#include "tablemodel.h"

#define OP_RETURN "RETURN"
#define OP_DISH_QTY "DISH_QTY"
#define OP_MANUAL "MANUAL"

struct SalaryFormula {
    QString operation;
    QString params;
    QString cell;
};

class ESalaryFormulaCounter : public Database
{
private:
#define mem_cell_count 16
    double m[mem_cell_count];
    QDate m_date1;
    QDate m_date2;
    QVector<SalaryFormula> &m_formulas;
    QSqlQuery *m_query;
    QStringList m_errors;

public:
    ESalaryFormulaCounter(QVector<SalaryFormula> &formulas);
    ~ESalaryFormulaCounter();
    void setDateRange(const QDate &date1, const QDate &date2);
    double counted();

private:
    int indexM(QString cell);
    bool isMemCell(const QString &cell);
    double fn_dish_qty(const QString &params, const QString &cell);
    double fn_multiplication(const QString &params, const QString &cell);
    double fn_return(const QString &params);
};

class ESalaryFormula : public EditWindow
{
    Q_OBJECT
public:
    ESalaryFormula(QWidget *parent = 0);
    QLineEdit *m_pQueue;
    QComboBox *m_pOperation;
    QLineEdit *m_pParams;
    QComboBox *m_pCell;

private:
    bool checkData();
    void getSalaryOperations();
    void getMemoryCells();
};

#endif // ESALARYFORMULA_H
