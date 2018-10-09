#include "eprintaliases.h"
#include "eprinteraliasrealpair.h"

EPrintAliases::EPrintAliases(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Aliases of the printers"));

    QWidget *pwdtFirst = new QWidget(this);
    QGridLayout *plFirst = new QGridLayout();
    int i = -1;
    plFirst->addWidget(new QLabel(tr("Id"), this), ++i, 0);
    plFirst->addWidget(m_pId = new QLineEdit(this), i, 1);
    m_pId->setEnabled(false);
    plFirst->addWidget(new QLabel(tr("Name"), this), ++i, 0);
    plFirst->addWidget(m_pName = new QLineEdit(this), i, 1);
    plFirst->setRowStretch(++i, 1);
    pwdtFirst->setLayout(plFirst);

    QWidget *pwdtButtons = new QWidget(this);
    QHBoxLayout *plButtons = new QHBoxLayout();
    plButtons->setMargin(0);
    QPushButton *pbtnNew = newBtn(this);
    pbtnNew->setText("");
    pbtnNew->setIcon(QIcon(":/icons/new.png"));
    QPushButton *pbtnDel = newBtn(this);
    pbtnDel->setText("");
    pbtnDel->setIcon(QIcon(":/icons/delete.png"));
    plButtons->addWidget(pbtnNew);
    plButtons->addWidget(pbtnDel);
    plButtons->addStretch(1);
    pwdtButtons->setLayout(plButtons);

    m_pMembers = new TableView(this);
    m_pMembers->setMinimumSize(500, 400);
    m_pMembers->m_model.insertColumns(0, 3, QModelIndex());
    m_pMembers->setColumn(0, "ALIAS_ID", tr("Alias id"), 0, DATA_INTEGER);
    m_pMembers->setColumn(1, "ALIAS", tr("Alias"), 200, DATA_STRING);
    m_pMembers->setColumn(2, "PRINTER", tr("Printer"), 250, DATA_STRING);

    QWidget *pwdtSecond = new QWidget(this);
    QVBoxLayout *plSecond = new QVBoxLayout();
    plSecond->setMargin(0);
    plSecond->addWidget(pwdtButtons);
    plSecond->addWidget(m_pMembers);
    pwdtSecond->setLayout(plSecond);

    QVBoxLayout *plMain = new QVBoxLayout();
    QTabWidget *pTab = new QTabWidget(this);
    pTab->addTab(pwdtFirst, tr("Main"));
    pTab->addTab(pwdtSecond, tr("Members"));
    plMain->addWidget(pTab);
    plMain->addWidget(m_pwidget);
    setLayout(plMain);

    connect(pbtnNew, SIGNAL(clicked()), SLOT(slotNew()));
    connect(pbtnDel, SIGNAL(clicked()), SLOT(slotDel()));
}

bool EPrintAliases::checkData()
{
    bool result = true;
    QString msg = "";

    if (!m_pName->text().length())
    {
        result = false;
        msg = tr("The name cannot be empty\n");
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EPrintAliases::setData(EPrinterAliasRealPair *e, int row)
{
    m_pMembers->m_model.setData(row, "ALIAS_ID", e->m_id);
    m_pMembers->m_model.setData(row, "ALIAS", e->m_pAlias->text());
    m_pMembers->m_model.setData(row, "PRINTER", e->m_pReal->text());
}

void EPrintAliases::slotNew()
{
    EPrinterAliasRealPair *e = new EPrinterAliasRealPair(this);
    if (e->exec() == QDialog::Accepted)
    {
        int new_row = m_pMembers->addRow();
        setData(e, new_row);
    }
    delete e;
}

void EPrintAliases::slotDel()
{
    QVector<int> rows;
    m_pMembers->getSelectedRows(rows);
    if (!rows.count())
        return;

    std::reverse(rows.begin(), rows.end());
    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
        m_pMembers->m_model.removeRows(*i, 1, QModelIndex());
}
