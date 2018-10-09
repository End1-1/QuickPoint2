#ifndef DLGNODISCOUNTDISHES_H
#define DLGNODISCOUNTDISHES_H

#include "edialog.h"

namespace Ui {
class DlgNoDiscountDishes;
}

class DlgNoDiscountDishes : public EDialog
{
    Q_OBJECT

public:
    explicit DlgNoDiscountDishes(QWidget *parent = 0);
    ~DlgNoDiscountDishes();

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::DlgNoDiscountDishes *ui;
};

#endif // DLGNODISCOUNTDISHES_H
