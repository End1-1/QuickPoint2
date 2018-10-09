#ifndef AEMPLOYEESALARY_H
#define AEMPLOYEESALARY_H

#include "editwindow.h"

class AEmployeeSalary : public EditWindow
{
    Q_OBJECT

public:
    AEmployeeSalary(QWidget *parent = 0);
    QLineEdit *m_pEmployeeFilter;
    QComboBox *m_pEmployee;
    QVector<int> m_EmployeePosition;
    QMap<QString, QVector<QString> > m_salaryFormulas;
    QComboBox *m_pPosition;
    QLineEdit *m_pAmount;
    bool m_showNext;

private:
    bool checkData();
    QString m_EmployeeFilter;
    void makePositionList();
    void makeEmployeesList();

private slots:
    void employeeFilter();
    void done();
    void next();
    void close();
    void employeeChanged(const int &index);
    void positionChanged(const int &index);
};

#endif // AEMPLOYEESALARY_H
