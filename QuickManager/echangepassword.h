#ifndef ECHANGEPASSWORD_H
#define ECHANGEPASSWORD_H

#include "editwindow.h"

class EChangePassword : public EditWindow
{
    Q_OBJECT
public:
    explicit EChangePassword(QWidget *parent = 0);
    QLineEdit *m_pOldPassword;
    QLineEdit *m_pNewPassword;
    QLineEdit *m_pNewPasswordConfirm;

private:
    bool checkData();
    
};

#endif // ECHANGEPASSWORD_H
