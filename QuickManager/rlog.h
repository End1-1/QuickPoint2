#ifndef RLOG_H
#define RLOG_H

#include "gridwindow.h"

class RLog : public GridWindow
{
    Q_OBJECT

public:
    RLog(QWidget *parent = 0);
    void setDate(const QDate date1, const QDate date2);

private:
    QDate m_date1;
    QDate m_date2;

    void getData();
    void slotTotal();
};

#endif // RLOG_H
