#ifndef ESTORAGE_H
#define ESTORAGE_H

#include "editwindow.h"
#include <QtGui>

class EStorage : public EditWindow
{
public:
    EStorage(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
};

#endif // ESTORAGE_H
