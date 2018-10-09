#ifndef LSETTINGS_H
#define LSETTINGS_H

#include "gridwindow.h"

class LSettings : public GridWindow
{
    Q_OBJECT

public:
    LSettings(QWidget *parent = 0);

private:
    void getData();
    void slotNew();
};

#endif // LSETTINGS_H
