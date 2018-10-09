#ifndef SALARY_H
#define SALARY_H

#include <QDialog>
#include <QtGui>
#include <QtSql>

struct Position {
    int id;
    QString name;
};

struct Employee {
    int id;
    int position_id;
    QString name;
    double salary;
};

class Salary;

class EmployeesListDelegate : public QItemDelegate {
private:
    Salary &m_salary;

public:
    EmployeesListDelegate(Salary &salary, QObject *parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class SalaryDocDelegate : public QItemDelegate {
private:
    Salary &m_salary;

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    SalaryDocDelegate(Salary &salary, QObject *parent = 0);
};

class Salary : public QDialog
{
    Q_OBJECT

public:
    explicit Salary(QWidget *parent = 0);
    ~Salary();

private:
    friend class EmployeesListDelegate;
    friend class SalaryDocDelegate;
    QSqlQuery *m_query;
    QListWidget *m_pEmployees;
    QListWidget *m_pDoc;
    QLabel *m_pTotal;
    QVector<Position> m_positions;
    QVector<Employee> m_employees;
    QVector<Employee> m_doc_items;
    bool m_viewPosition;
    int m_doc_id;

    void makePositionsList();
    void makeEmployeesList();
    void selectPosition(const QModelIndex &index);
    void selectEmployee(const QModelIndex &index);
    void countTotal();
    void loadDoc();

private slots:
    void editSalaryAmount();
    void deleteRow();
    void positions();
    void save();
    void tryClose();
    void employeesClick(const QModelIndex &index);

};

#endif // SALARY_H
