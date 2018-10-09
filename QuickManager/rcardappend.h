#ifndef RCARDAPPEND_H
#define RCARDAPPEND_H

#include "gridwindow.h"

class rcardappend : public GridWindow
{
    Q_OBJECT
public:
    rcardappend(QWidget *parent = 0);
private:
    void getData();
protected slots:
    virtual void slotNew();
    virtual void slotEdit();
    virtual void slotTotal();
    virtual void doubleClicked(const QModelIndex &index);
    void noDiscountDishes();
};

#endif // RCARDAPPEND_H
