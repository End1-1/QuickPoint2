#ifndef LPRINTALIEASES_H
#define LPRINTALIEASES_H

#include "gridwindow.h"

class EPrintAliases;

class LPrintAlieases : public GridWindow
{
    Q_OBJECT

public:
    LPrintAlieases(QWidget *parent = 0);

private:
    void getData();
    void slotNew();
    void slotEdit();
    void setData(EPrintAliases *e, int row);
    void updateMembers(QSqlQuery *q, EPrintAliases *e);
};

#endif // LPRINTALIEASES_H
