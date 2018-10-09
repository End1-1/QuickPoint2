#ifndef ESETTINGS_H
#define ESETTINGS_H

#include "editwindow.h"

class ESettings : public EditWindow
{
    Q_OBJECT

public:
    ESettings(QWidget *parent = 0);

private:
    bool checkData();
};

#endif // ESETTINGS_H
