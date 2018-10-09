#include "erecipeqty.h"

ERecipeQty::ERecipeQty(const QString goodsName, const QString &qty, QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Quantity"));
    int i = -1;
    QGridLayout *l = new QGridLayout();

    l->addWidget(new QLabel(tr("Goods name")), ++i, 0);
    QLineEdit *pedt_Goods = new QLineEdit(goodsName);
    pedt_Goods->setEnabled(false);
    l->addWidget(pedt_Goods, i, 1);

    l->addWidget(new QLabel(tr("Quantity")), ++i, 0);
    l->addWidget(pedt_Qty = new QLineEdit(qty), i, 1);
    pedt_Qty->setSelection(0, 2);

    l->addWidget(m_pwidget, ++i, 0, 1, 2);
    setLayout(l);
}

QString ERecipeQty::qty()
{
    return QString::number(pedt_Qty->text().toDouble(), 'f', 3);
}


bool ERecipeQty::checkData()
{
    if (pedt_Qty->text().toDouble() <= 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("the quantity must be more then 0\n"));
        return false;
    }
    return true;
}

void ERecipeQty::keyPressEvent(QKeyEvent *e)
{
    QDialog::keyPressEvent(e);
}
