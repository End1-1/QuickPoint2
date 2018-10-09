#ifndef ECOMBOITEM_H
#define ECOMBOITEM_H

#include "editwindow.h"

class EComboItem : public EditWindow
{
    Q_OBJECT

public:
    EComboItem(QWidget *parent = 0);
    QString m_pDishId;
    QLineEdit *m_pDishName;
    QLineEdit *m_pQty;
    QLineEdit *m_pPrice;
    QComboBox *m_pPrinterName;

private:
    bool checkData();

private slots:
    void slotDish();
};

#endif // ECOMBOITEM_H
