#ifndef LMENU_H
#define LMENU_H

#include "gridwindow.h"

class EMenu;

class LMenu : public GridWindow
{
    Q_OBJECT

public:
    LMenu();

private:
    void getData();
    void setData(EMenu *e, const int row);
    void slotNew();
    void slotEdit();
};

#endif // LMENU_H
