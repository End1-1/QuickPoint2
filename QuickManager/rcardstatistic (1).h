#ifndef RCARDSTATISTIC_H
#define RCARDSTATISTIC_H

#include "gridwindow.h"

class RCardStatistic : public GridWindow
{
    Q_OBJECT
public:
    RCardStatistic(QWidget *parent = 0);

private:
    QString fCard;
    QString fDate1;
    QString fDate2;
    bool fGroup;
    void getData();

private slots:
    virtual void slotTotal();
    virtual void slotFilter();
};

#endif // RCARDSTATISTIC_H
