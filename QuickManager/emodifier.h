#ifndef EMODIFIER_H
#define EMODIFIER_H

#include "editwindow.h"

class EModifier : public EditWindow
{
    Q_OBJECT

public:
    EModifier(QWidget *parent = 0);
    QLineEdit *m_pName;
    int m_id;
    QComboBox *m_pAction;

private:
    bool checkData();
};

#endif // EMODIFIER_H
