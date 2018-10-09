#ifndef EPOSTITION_H
#define EPOSTITION_H

#include "editwindow.h"
#include "esalaryformula.h"
#include <QtGui>

class TableView;

class EPosition : public EditWindow
{
    Q_OBJECT

public:
    EPosition(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;
    TableView *m_pSalary;

private:
    bool checkData();
    void setSalaryData(ESalaryFormula *e, const int &row);
    void markQueue();

private slots:
    void newFormula();
    void editFormula();
    void deleteFormula();
};

#endif // EPOSTITION_H
