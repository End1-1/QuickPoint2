#ifndef ESALEORDER_H
#define ESALEORDER_H

#include "editwindow.h"

class SaleOrder : public EditWindow
{
    Q_OBJECT

public:
    explicit SaleOrder(QWidget *parent = 0);
    QTableWidget *m_pOrder;
    QTableWidget *m_pAdditional;
    
private:
    bool checkData();
    
};

#endif // SALEORDER_H
