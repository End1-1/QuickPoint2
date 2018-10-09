#ifndef LSTORAGE_H
#define LSTORAGE_H

#include "gridwindow.h"

class EStorage;

class LStorage : public GridWindow
{
    Q_OBJECT

public:
    LStorage(QWidget *parent = 0);

private:
    void getData();
    void setData(EStorage *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LSTORAGE_H
