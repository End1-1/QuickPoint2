#ifndef EGOODS_H
#define EGOODS_H

#include "editwindow.h"
#include <QtGui>

class EGoodsGroup : public EditWindow
{
    Q_OBJECT

public:
    EGoodsGroup(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;

private:
    bool checkData();
};

#endif // EGOODS_H
