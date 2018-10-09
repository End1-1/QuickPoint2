#include "eprinteraliasrealpair.h"
#include "checklist2.h"

EPrinterAliasRealPair::EPrinterAliasRealPair(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Alias - printer pair"));

    m_id = 0;
    int i = -1;
    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 300);
    plMain->addWidget(new QLabel(tr("Alias"), this), ++i, 0);
    plMain->addWidget(m_pAlias = new QLineEdit(this), i, 1);
    m_pAlias->setEnabled(false);
    QPushButton *pbtnAlias = newBtn(this);
    plMain->addWidget(pbtnAlias, i, 2);
    plMain->addWidget(new QLabel(tr("Printer"), this), ++i, 0);
    plMain->addWidget(m_pReal = new QLineEdit(this), i, 1);
    plMain->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(plMain);

    connect(pbtnAlias, SIGNAL(clicked()), SLOT(slotAlias()));
}

void EPrinterAliasRealPair::slotAlias()
{
    CheckList2 *c = new CheckList2(false, this);

    c->buildList("SELECT ID, NAME FROM PRINTERS ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        QString keys, values;
        c->returnResult(keys, values);
        if (values.length())
        {
            m_pAlias->setText(values);
            m_id = keys.toInt();
        }
    }
    delete c;
}

bool EPrinterAliasRealPair::checkData()
{
    bool result = true;
    QString msg = "";

    if (!m_id)
    {
        result = false;
        msg += tr("The alias is not selected\n");
    }

    if (!m_pReal->text().length())
    {
        result = false;
        msg += tr("The real name cannot be empty\n");
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}
