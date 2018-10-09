#ifndef SIMPLEPASSWORD_H
#define SIMPLEPASSWORD_H

#include <QtGui>
#include <QDialog>
#include "eunit.h"

class SimplePassword : public QDialog
{
    Q_OBJECT

public:
    SimplePassword(const QString &title, QWidget *parent = 0);
    QLineEdit *m_password;

private slots:
    void slotOk();
};

#endif // SIMPLEPASSWORD_H
