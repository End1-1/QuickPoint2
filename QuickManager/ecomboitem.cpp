#include "ecomboitem.h"
#include "checklist2.h"

EComboItem::EComboItem(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("ComboItem"));
    int i = -1;
    QGridLayout *l = new QGridLayout(this);

    QPushButton *pbDish = newBtn(this);
    connect(pbDish, SIGNAL(clicked()), SLOT(slotDish()));
    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(m_pDishName = new QLineEdit(this), i, 1);
    m_pDishName->setReadOnly(true);
    l->addWidget(pbDish, i, 2);

    l->addWidget(new QLabel(tr("Quantity")), ++i, 0);
    l->addWidget(m_pQty = new QLineEdit(this), i, 1);
    m_pQty->setValidator(new QDoubleValidator(this));

    l->addWidget(new QLabel(tr("Price")), ++i, 0);
    l->addWidget(m_pPrice = new QLineEdit(this), i, 1);
    m_pPrice->setValidator(new QDoubleValidator(this));

    l->addWidget(new QLabel(tr("Printer")), ++i, 0);
    l->addWidget(m_pPrinterName = new QComboBox(this), i, 1);
    QSqlQuery *q = m_db.QuerySelect("SELECT ID, NAME FROM PRINTERS ORDER BY NAME", QStringList(), QVector<QVariant>());
    while (q->next())
        m_pPrinterName->addItem(q->value(1).toString(), q->value(0).toString());
    delete q;
    m_db.db.close();

    l->addWidget(m_pwidget, ++i, 0, 1, 2);
    setLayout(l);
}

bool EComboItem::checkData()
{
    bool result = true;
    QString msg = "";

    if (m_pDishId.length() == 0)
    {
        result = false;
        msg += tr("Name cannot be empty\n");
    }

    if (m_pQty->text().toInt() == 0)
    {
        result = false;
        msg += tr("Quantity cannot be empty\n");
    }

    if (m_pPrinterName->currentIndex() < 0)
    {
        result = false;
        msg += tr("Printer cannot be empty\n");
    }

    m_pPrice->setText(QString::number(m_pPrice->text().toDouble()));

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EComboItem::slotDish()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM DISHES WHERE COMBO=0 ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_pDishId = "";
        m_pDishName->setText("");
        if (c->m_result.count() > 0)
        {
            m_pDishId = c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString();
            m_pDishName->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
        }
    }
    delete c;
}
