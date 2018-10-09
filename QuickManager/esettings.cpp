#include "esettings.h"

ESettings::ESettings(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Settings"));
}

bool ESettings::checkData()
{
    return true;
}
