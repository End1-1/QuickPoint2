#ifndef FSALE_H
#define FSALE_H

#include "editwindow.h"
#include <QtGui>

class FSale : public EditWindow
{
    Q_OBJECT

public:
    FSale(QWidget *parent = 0);
    QLineEdit *m_peOrderId;
    QCheckBox *m_pchOrderId;
    QDateEdit *m_pd1;
    QDateEdit *m_pd2;
    QCheckBox *m_pchDate;
    QCheckBox *m_pchFullDate;
    QDateTimeEdit *m_pfullDate1;
    QDateTimeEdit *m_pfullDate2;
    QLineEdit *m_pTable;
    QString m_table;
    QCheckBox *m_pchTable;
    QLineEdit *m_pWaiter;
    QString m_waiter;
    QCheckBox *m_pchWaiter;
    QLineEdit *m_peDishGroup;
    QString m_dishGroup;
    QCheckBox *m_pchDishGroup;
    QLineEdit *m_peDish;
    QString m_dish;
    QCheckBox *m_pchDish;

private:
    bool checkData();

private slots:
    void slotTable();
    void slotWaiter();
    void slotDishGroup();
    void slotDish();
};

#endif // FSALE_H
