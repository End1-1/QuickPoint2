#ifndef EEMPLOYEE_H
#define EEMPLOYEE_H

#include "editwindow.h"
#include <QtGui>

class EEmployee : public EditWindow
{
    Q_OBJECT

public:
    EEmployee(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Position;
    QLineEdit *pedt_Name;
    QCheckBox *pchk_DBAccess;
    QLineEdit *pedt_Password;
    QLineEdit *pedt_Role;
    QCheckBox *pchk_Active;
    QString m_password;
    QString m_position;
    QString m_role;

private:
    bool checkData();

private slots:
    void slotPosition();
    void slotRole();
    void slotPassword(const QString &value);
    void slotDBAccess(const int &value);
};

#endif // EEMPLOYEE_H
