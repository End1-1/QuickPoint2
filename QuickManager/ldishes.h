#ifndef LDISHES_H
#define LDISHES_H

#include "gridwindow.h"

class EDish;

class LDishes : public GridWindow
{
    Q_OBJECT

public:
    LDishes(QWidget *parent = 0);

private:
    QString m_fmenu;
    QString m_fgroup;
    QString m_fstore;
    bool m_fvisible;
    void getData();
    void setData(const EDish *e, const int row);
    void setAdditionalData(const EDish *e);
    void slotNew();
    void slotEdit();
    void slotFilter();

private slots:
    void slotRecipe();
    void slotGroupEvent();
};

#endif // LDISHES_H
