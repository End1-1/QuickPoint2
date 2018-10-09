#ifndef RSALARY_H
#define RSALARY_H

#include "gridwindow.h"

class RSalary : public GridWindow
{
    Q_OBJECT
public:
    RSalary(QWidget *parent = 0);

private:
    QString m_docId;
    QDate m_date1;
    QDate m_date2;
    QString m_operatorName;
    QString m_operatorId;
    QString m_positionName;
    QString m_positionId;
    QString m_employeeName;
    QString m_employeeId;
    bool m_groupDocId;
    bool m_groupDate;
    bool m_groupOperator;
    bool m_groupPosition;
    bool m_groupEmployee;

    void getData();
    void slotNew();
    void slotEdit();
    void slotDelete();
    void slotFilter();
    void slotTotal();
};

#endif // RSALARY_H
