#ifndef RCARDBYUSE_H
#define RCARDBYUSE_H

#include "gridwindow.h"

class rcardbyuse : public GridWindow
{
    Q_OBJECT
public:
    rcardbyuse(QWidget *parent = 0);
    virtual void slotFilter();

protected slots:
    virtual void doubleClicked(const QModelIndex &index);
    virtual void slotTotal();
    virtual void slotPrint();
private:
    void getData();
    QDate fDate1;
    QDate fDate2;
};

#endif // RCARDBYUSE_H
