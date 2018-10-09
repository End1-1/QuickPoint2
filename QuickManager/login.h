#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>
#include <QDialog>
#include "data.h"
#include "eunit.h"

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    
private:
    QComboBox *m_databases;
    QLineEdit *m_password;
    void readConnections();

private slots:
    void slotOk();
    void slotPlusDB();
    void slotRemoveDB();
};

#endif // LOGIN_H
