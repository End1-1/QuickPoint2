#ifndef FEMPLOYEE_H
#define FEMPLOYEE_H

#include "editwindow.h"
#include <QtGui>

class FEmployee : public EditWindow
{
    Q_OBJECT

public:
    FEmployee(QWidget *parent = 0);
    QLineEdit *pedt_Position;
    QCheckBox *pchk_Active;

private:
    bool checkData();

private slots:
    void slotPosition();
};

#endif // FEMPLOYEE_H
