#ifndef EHALL_H
#define EHALL_H

#include "editwindow.h"

class EHall : public EditWindow
{
    Q_OBJECT

public:
    EHall(QWidget *parent = 0);
    QLineEdit *m_pId;
    QLineEdit *m_pName;

private:
    bool checkData();
};

#endif // EHALL_H
