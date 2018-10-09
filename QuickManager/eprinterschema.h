#ifndef EPRINTERSCHEMA_H
#define EPRINTERSCHEMA_H

#include "editwindow.h"

class EPrinterSchema : public EditWindow
{
    Q_OBJECT

public:
    EPrinterSchema(QWidget *parent = 0);
    QLineEdit *m_pId;
    QLineEdit *m_pName;
    TableView *m_pMembers;

private:
    bool checkData();

private slots:
    void slotNew();
    void slotDel();
};

#endif // EPRINTERSCHEMA_H
