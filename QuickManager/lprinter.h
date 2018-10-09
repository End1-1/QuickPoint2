#ifndef LPRINTER_H
#define LPRINTER_H

#include "gridwindow.h"

class EPrinter;

class LPrinter : public GridWindow
{
    Q_OBJECT

public:
    LPrinter(QWidget *parent = 0);

private:
    void getData();
    void setData(EPrinter *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LPRINTER_H
