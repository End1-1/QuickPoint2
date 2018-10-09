#ifndef LEMPLOYEE_H
#define LEMPLOYEE_H

#include "gridwindow.h"

class EEmployee;

class LEmployee : public GridWindow
{
    Q_OBJECT

public:
    LEmployee(QWidget *parent = 0);

private:
    QString m_fposition;
    bool m_factive;
    void getData();
    void setData(EEmployee *e, const int row);
    void slotNew();
    void slotEdit();
    void slotFilter();
};

#endif // LEMPLOYEE_H
