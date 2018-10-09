#include "lsettings.h"
#include "esettings.h"

LSettings::LSettings(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Settings"));
    getData();
}

void LSettings::getData()
{
    table->ExecuteQuery("SELECT DISTINCT(COMPUTER_NAME) AS COMPUTER_NAME FROM SETTINGS");
    table->setColumn("COMPUTER_NAME", tr("Name"), 400, DATA_STRING);
}

void LSettings::slotNew()
{
    ESettings *e = new ESettings(this);
    if (e->exec() == QDialog::Accepted)
    {

    }
    delete e;
}
