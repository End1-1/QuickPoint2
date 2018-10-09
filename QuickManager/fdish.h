#ifndef FDISH_H
#define FDISH_H

#include "editwindow.h"
#include <QtGui>

class FDish : public EditWindow
{
    Q_OBJECT

public:
    FDish(QWidget *parent = 0);
    QLineEdit *pedt_Menu;
    QLineEdit *pedt_Group;
    QLineEdit *pedt_Store;
    QCheckBox *pchk_Visible;
    QCheckBox *pchk_NoRecipe;

private:
    bool checkData();

private slots:
    void slotMenu();
    void slotGroup();
    void slotStore();
};

#endif // FDISH_H
