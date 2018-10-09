#ifndef EPRINTERALIASREALPAIR_H
#define EPRINTERALIASREALPAIR_H

#include "editwindow.h"

class EPrinterAliasRealPair : public EditWindow
{
    Q_OBJECT

public:
    EPrinterAliasRealPair(QWidget *parent = 0);
    int m_id;
    QLineEdit *m_pAlias;
    QLineEdit *m_pReal;

private slots:
    void slotAlias();
    bool checkData();
};

#endif // EPRINTERALIASREALPAIR_H
