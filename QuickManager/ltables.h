#ifndef LTABLES_H
#define LTABLES_H

#include "gridwindow.h"

class ETables;

class LTables : public GridWindow
{
    Q_OBJECT

public:
    LTables(QWidget *parent = 0);

private:
    void getData();
    void setData(ETables *e, const int &row);
    void slotNew();
    void slotEdit();
};

#endif // LTABLES_H
