#ifndef ERECIPEQTY_H
#define ERECIPEQTY_H

#include <QtGui>
#include "editwindow.h"

class ERecipeQty : public EditWindow
{
    Q_OBJECT

private:
    QLineEdit *pedt_Qty;
    bool checkData();

public:
    ERecipeQty(const QString goodsName, const QString &qty, QWidget *parent = 0);
    QString qty();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // ERECIPEQTY_H
