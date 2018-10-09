#ifndef FSALARY_H
#define FSALARY_H

#include "editwindow.h"

class FSalary : public EditWindow
{
    Q_OBJECT

public:
    FSalary(QWidget *parent = 0);
    QLineEdit *m_pDocId;
    QDateEdit *m_pDate1;
    QDateEdit *m_pDate2;
    QLineEdit *m_pOperator;
    QLineEdit *m_pPosition;
    QLineEdit *m_pEmployee;

    QCheckBox *m_pGroupDocId;
    QCheckBox *m_pGroupDate;
    QCheckBox *m_pGroupOperator;
    QCheckBox *m_pGroupPosition;
    QCheckBox *m_pGroupEmployee;

    QString m_operator;
    QString m_position;
    QString m_employee;

private:
    bool checkData();

private slots:
    void operators();
    void positions();
    void employees();
};

#endif // FSALARY_H
