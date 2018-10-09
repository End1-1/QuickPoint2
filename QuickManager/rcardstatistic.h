#ifndef RCARDSTATISTIC_H
#define RCARDSTATISTIC_H

#include "gridwindow.h"

class RCardStatistic : public GridWindow
{
    Q_OBJECT
public:
    RCardStatistic(QWidget *parent = 0);
    QString fCard;
    QString fDate1;
    QString fDate2;
    void getData();
private:

    bool fGroup;

private slots:
    virtual void slotTotal();
    virtual void slotFilter();
};

#endif // RCARDSTATISTIC_H
