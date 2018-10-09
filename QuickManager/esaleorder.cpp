#include "esaleorder.h"

SaleOrder::SaleOrder(QWidget *parent) :
    EditWindow(parent)
{
    setMinimumSize(750, 500);
    setWindowTitle(tr("Order"));
    QVBoxLayout *plMain = new QVBoxLayout();
    QTabWidget *pTab = new QTabWidget(this);

    QWidget *pwdtPageOne = new QWidget(this);
    QVBoxLayout *plPageOne = new QVBoxLayout(this);
    m_pOrder = new QTableWidget(this);
    m_pOrder->setColumnCount(5);
    QVector<int> col_widths;
    col_widths << 100 << 250 << 100 << 100 << 100;
    QStringList col_names;
    col_names << tr("Dish code") << tr("Dish name")
                               << tr("Quantity") << tr("Price")
                               << tr("Total");
    for (int i = 0; i < m_pOrder->columnCount(); i++)
        m_pOrder->setColumnWidth(i, col_widths[i]);
    m_pOrder->setHorizontalHeaderLabels(col_names);

    QPushButton *pbtnOk = new QPushButton(tr("Close"), this);
    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    plPageOne->addWidget(m_pOrder);
    pwdtPageOne->setLayout(plPageOne);
    pTab->addTab(pwdtPageOne, tr("Order"));

    QWidget *pwdtPageTwo = new QWidget(this);
    QVBoxLayout *plPageTwo = new QVBoxLayout();
    m_pAdditional = new QTableWidget(this);
    m_pAdditional->setColumnCount(2);
    col_widths.clear();
    col_widths << 300 << 200;
    col_names.clear();
    col_names << tr("Parameter") << tr("Value");
    for (int i = 0; i < col_widths.count(); i++)
        m_pAdditional->setColumnWidth(i, col_widths[i]);
    m_pAdditional->setHorizontalHeaderLabels(col_names);
    plPageTwo->addWidget(m_pAdditional);
    pwdtPageTwo->setLayout(plPageTwo);
    pTab->addTab(pwdtPageTwo, tr("Additional data"));

    plMain->addWidget(pTab);
    plMain->addWidget(pbtnOk);
    setLayout(plMain);
}

bool SaleOrder::checkData()
{
    return true;
}
