#ifndef FSALEOUT_H
#define FSALEOUT_H

#include "editwindow.h"
#include <QtGui>
#include "tablemodel.h"

class FSaleOut : public EditWindow
{
    Q_OBJECT

public:
    FSaleOut(QWidget *parent = 0);
    ~FSaleOut();
    QDateEdit *m_pdDate1;
    QDateEdit *m_pdDate2;
    QDateEdit *m_pdDocDate;
    QLineEdit *m_peStore;
    QString  m_store;
    QLabel *m_plStore;

private:
    bool checkData();
    Database db;

private slots:
    void slotStore();
    void slotCancel();

public slots:
    void slotStoreByCode();

};

#endif // FSALEOUT_H
