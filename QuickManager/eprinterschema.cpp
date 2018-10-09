#include "eprinterschema.h"
#include "checklist2.h"

EPrinterSchema::EPrinterSchema(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Schema of the printer"));

    int i = -1;
    QGridLayout *plFirst = new QGridLayout();
    QWidget *pwdtFirst = new QWidget(this);
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
    m_pMembers->m_model.insertColumns(0, 2, QModelIndex());
    m_pMembers->setMinimumSize(500, 400);
    m_pMembers->setColumn(0, "PRINTER_ID", tr("Printer id"), 0, DATA_INTEGER);
    m_pMembers->setColumn(1, "PRINTER", tr("Printer"), 400, DATA_STRING);

    QWidget *pwdtSecond = new QWidget(this);
    QVBoxLayout *plSecond = new QVBoxLayout();
    plSecond->addWidget(pwdtButtons);
    plSecond->addWidget(m_pMembers);
    pwdtSecond->setLayout(plSecond);

    QTabWidget *pTab = new QTabWidget(this);
    pTab->addTab(pwdtFirst, tr("Main"));
    pTab->addTab(pwdtSecond, tr("Members"));

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(pTab);
    plMain->addWidget(m_pwidget);
    setLayout(plMain);

    connect(pbtnNew, SIGNAL(clicked()), SLOT(slotNew()));
    connect(pbtnDel, SIGNAL(clicked()), SLOT(slotDel()));

}

bool EPrinterSchema::checkData()
{
    bool result = true;
    QString msg = "";

    if (!m_pName->text().length())
    {
        result = false;
        msg += tr("The name cannot be empty\n");
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EPrinterSchema::slotNew()
{
    CheckList2 *c = new CheckList2(true, this);

    c->buildList("SELECT ID, NAME FROM PRINTERS ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        QString keys, values;
        c->returnResult(keys, values);
        if (values.length())
        {
            QStringList list_keys = keys.split(",");
            QStringList list_values = values.split(",");
            for (int i = 0; i < list_keys.count(); i++)
            {
                int new_row = m_pMembers->addRow();
                m_pMembers->m_model.setData(new_row, "PRINTER_ID", list_keys[i]);
                m_pMembers->m_model.setData(new_row, "PRINTER", list_values[i]);
            }
        }
    }
    delete c;
}

void EPrinterSchema::slotDel()
{
}
