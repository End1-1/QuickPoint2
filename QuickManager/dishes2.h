#ifndef DISHES2_H
#define DISHES2_H

#include "gridwindow2.h"

class EDish;

class Dishes2 : public GridWindow2
{
    Q_OBJECT

public:
    explicit Dishes2(QWidget *parent = 0);

private:
    void setDataFromDialog(const int &row, const QDialog *dialog);
    void slotNew();
    void slotEdit();
    void slotRefresh();
    void slotFilter();
    void setDefaultFilter();
    void slotDelete();
    void setAdditionalData(const EDish *e);

private slots:
    void slotRecipe();
    void slotGroupEvent();
};

#endif // DISHES2_H
