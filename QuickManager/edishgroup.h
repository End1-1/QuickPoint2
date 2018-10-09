#ifndef EDISHGROUP_H
#define EDISHGROUP_H

#include "editwindow.h"
#include <QtGui>

class EDishGroup : public EditWindow
{
public:
    EDishGroup(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
    void slotOk();
};

#endif // EDISHGROUP_H
