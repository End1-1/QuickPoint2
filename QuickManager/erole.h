#ifndef EROLE_H
#define EROLE_H

#include "editwindow.h"
#include <QtGui>

class ERole : public EditWindow
{
    Q_OBJECT

public:
    ERole(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
};

#endif // EROLE_H
