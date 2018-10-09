#ifndef EMENU_H
#define EMENU_H

#include "editwindow.h"
#include <QtGui>

class EMenu : public EditWindow
{
    Q_OBJECT

public:
    EMenu(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
    void slotOk();
};

#endif // EMENU_H
