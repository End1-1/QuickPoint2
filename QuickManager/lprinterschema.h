#ifndef LPRINTERSCHEMA_H
#define LPRINTERSCHEMA_H

#include "gridwindow.h"

class EPrinterSchema;

class LPrinterSchema : public GridWindow
{
    Q_OBJECT

public:
    LPrinterSchema(QWidget *parent = 0);

private:
    void getData();
    void slotNew();
    void slotEdit();
    void setData(EPrinterSchema *e, int row);
    void updateMembers(QSqlQuery *q, EPrinterSchema *e);
};

#endif // LPRINTERSCHEMA_H
