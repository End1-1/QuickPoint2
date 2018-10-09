#ifndef ETABLES_H
#define ETABLES_H

#include "editwindow.h"

class ETables : public EditWindow
{
    Q_OBJECT

public:
    ETables(QWidget *parent = 0);
    QLineEdit *m_pId;
    QLineEdit *m_pHallName;
    QLineEdit *m_pTableName;
    QLineEdit *m_pQueue;
    int m_HallId;
    QPushButton *m_pbtnHall;

private slots:
    bool checkData();
    void slotHall();
};

#endif // ETABLES_H
