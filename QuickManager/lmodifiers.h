#ifndef LMODIFIERS_H
#define LMODIFIERS_H

#include "gridwindow.h"

class EModifier;

class LModifiers : public GridWindow
{
    Q_OBJECT

public:
    LModifiers(QWidget *parent = 0);

private:
    void getData();
    void slotNew();
    void slotEdit();
    void setData(const EModifier *e, const int &row);
};

#endif // LMODIFIERS_H
