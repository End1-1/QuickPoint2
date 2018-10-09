#ifndef AMODIFIERS_H
#define AMODIFIERS_H

#include "editwindow.h"

class AModifiers : public EditWindow
{
    Q_OBJECT

public:
    AModifiers(QWidget *parent = 0);
    QComboBox *m_pName;

private:
    bool checkData();
};

#endif // AMODIFIERS_H
