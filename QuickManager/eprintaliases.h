#ifndef EPRINTALIASES_H
#define EPRINTALIASES_H

#include "editwindow.h"

class EPrinterAliasRealPair;

class EPrintAliases : public EditWindow
{
    Q_OBJECT

public:
    EPrintAliases(QWidget *parent = 0);
    QLineEdit *m_pId;
    QLineEdit *m_pName;
    TableView *m_pMembers;

private:
    bool checkData();
    void setData(EPrinterAliasRealPair *e, int row);

private slots:
    void slotNew();
    void slotDel();
};

#endif // EPRINTALIASES_H
