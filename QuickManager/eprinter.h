#ifndef EPRINTER_H
#define EPRINTER_H

#include "editwindow.h"
#include <QtGui>

class EPrinter : public EditWindow
{
    Q_OBJECT

public:
    EPrinter(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
};

#endif // EPRINTER_H
