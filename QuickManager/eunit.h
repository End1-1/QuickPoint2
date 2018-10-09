#ifndef EUNIT_H
#define EUNIT_H

#include "editwindow.h"
#include <QtGui>

class EUnit : public EditWindow
{
public:
    EUnit(QWidget *parent = 0);

    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();

private slots:
    void slotOk();
};

#endif // EUNIT_H
