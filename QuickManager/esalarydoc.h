#ifndef ESALARYDOC_H
#define ESALARYDOC_H

#include <QDialog>
#include <QtGui>
#include "tablemodel.h"
#include "eunit.h"

class AEmployeeSalary;
class PaintCommand;

class ESalaryDoc : public QDialog
{
    Q_OBJECT
public:
    explicit ESalaryDoc(QWidget *parent = 0);
    QLineEdit *m_pDocId;
    QLineEdit *m_pTotal;
    QLineEdit *m_pOperator;
    QDateEdit *m_pDate;
    QDateEdit *m_pDate1;
    QDateEdit *m_pDate2;
    TableView *m_pDoc;
    bool show_next;
    void countTotal();
    bool m_isDocSaved;
    
private slots:
    void newEmployee();
    void editEmployee();
    void deleteEmployee();
    void countSalary();
    void printDoc();
    void saveDoc();
    void closeDoc();
    void docChanged();

private:
    void setData(AEmployeeSalary *a, const int &row);
    bool checkDoc();
    int printY(int &y, const int &delta, QVector<PaintCommand *> &pc);
    void printVertTableLine(QVector<int> &x, QVector<PaintCommand *> &pc, const QPen &pen, const int &y);
};

#endif // ESALARYDOC_H
