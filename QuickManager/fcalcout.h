#ifndef FCALCOUT_H
#define FCALCOUT_H

#include <QDialog>
#include <QtGui>
#include "eunit.h"

class FCalcOut : public QDialog
{
    Q_OBJECT
public:
    explicit FCalcOut(QWidget *parent = 0);

    QDateEdit *m_pDate1;
    QDateEdit *m_pDate2;
    QLineEdit *m_pStore;
    QString m_store;

private slots:
    void store();
    
};

#endif // FCALCOUT_H
