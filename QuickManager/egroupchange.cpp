#include "egroupchange.h"
#include "checklist.h"

EGroupChange::EGroupChange(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Grouping changes"));

    int i = -1;
    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 300);

    plMain->addWidget(new QLabel(tr("Menu"), this), ++i, 0);
    plMain->addWidget(m_pMenuName = new QLineEdit(this), i, 1);
    m_pMenuName->setEnabled(false);
    plMain->addWidget(m_pbtnSelectMenu = new QPushButton("...", this), i, 2);
    m_pbtnSelectMenu->setMaximumSize(50, 50);
    connect(m_pbtnSelectMenu, SIGNAL(clicked()), SLOT(slotMenu()));

    plMain->addWidget(new QLabel(tr("Group"), this), ++i, 0);
    plMain->addWidget(m_pGroupName = new QLineEdit(this), i, 1);
    m_pGroupName->setEnabled(false);
    plMain->addWidget(m_pbtnSelectGroup = new QPushButton("...", this), i, 2);
    m_pbtnSelectGroup->setMaximumSize(50, 50);
    connect(m_pbtnSelectGroup, SIGNAL(clicked()), SLOT(slotGroup()));

    plMain->addWidget(new QLabel(tr("Store"), this), ++i, 0);
    plMain->addWidget(m_pStoreName = new QLineEdit(this), i, 1);
    m_pStoreName->setEnabled(false);
    plMain->addWidget(m_pbtnSelectStore = new QPushButton("...", this), i, 2);
    m_pbtnSelectStore->setMaximumSize(50, 50);
    connect(m_pbtnSelectStore, SIGNAL(clicked()), SLOT(slotStore()));

    plMain->addWidget(new QLabel(tr("Printer"), this), ++i, 0);
    plMain->addWidget(m_pPrintName = new QLineEdit(this), i, 1);
    m_pPrintName->setEnabled(false);
    plMain->addWidget(m_pbtnSelectPrint = new QPushButton("...", this), i, 2);
    m_pbtnSelectPrint->setMaximumSize(50, 50);
    connect(m_pbtnSelectPrint, SIGNAL(clicked()), SLOT(slotPrint()));

    plMain->addWidget(new QLabel(tr("Color"), this), ++i, 0);
    plMain->addWidget(m_pColor = new QLineEdit(this), i, 1);
    m_pColor->setEnabled(false);
    plMain->addWidget(m_pbtnSelectColor = new QPushButton("...", this), i, 2);
    m_pbtnSelectColor->setMaximumSize(50, 50);
    connect(m_pbtnSelectColor, SIGNAL(clicked()), SLOT(slotColor()));

    plMain->addWidget(new QLabel(tr("Queue"), this), ++i, 0);
    plMain->addWidget(m_pQueue = new QLineEdit(this), i, 1);
    m_pQueue->setValidator(new QIntValidator(this));


    plMain->addWidget(new QLabel(tr("Visibility"), this), ++i, 0);
    plMain->addWidget(m_pVisible = new QCheckBox("", this), i, 1);
    m_pVisible->setChecked(true);

    plMain->addWidget(new QLabel(tr("I now, what i'm doing"), this), ++i, 0);
    plMain->addWidget(m_pImNotIdiot = new QCheckBox("", this), i, 1);
    connect(m_pImNotIdiot, SIGNAL(clicked(bool)), SLOT(slotImNotIdiot(bool)));

    QHBoxLayout *plOkCancel = new QHBoxLayout();
    QWidget *wdtOkCancel = new QWidget(this);
    plOkCancel->addStretch(1);
    plOkCancel->addWidget(m_pbtnOk = new QPushButton(tr("Ok"), this));
    m_pbtnOk->setEnabled(false);
    connect(m_pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    plOkCancel->addWidget(m_pbtnCancel = new QPushButton(tr("Cancel"), this));
    connect(m_pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
    plOkCancel->addStretch(1);
    wdtOkCancel->setLayout(plOkCancel);

    plMain->addWidget(wdtOkCancel, ++i, 0, 1, 3);
    setLayout(plMain);
}

void EGroupChange::slotMenu()
{
    CheckList *c = new CheckList(false, this);
    c->sql("SELECT ID, NAME FROM MENU ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            m_pMenuName->setText(c->values[c->checked[0]].toString());
            m_menu = c->key[c->checked[0]].toString();
        }
        else
        {
            m_pMenuName->clear();
            m_menu = "0";
        }
    }
    delete c;
}

void EGroupChange::slotGroup()
{
    CheckList *c = new CheckList(false, this);
    c->sql("SELECT ID, NAME FROM DISHES_GROUPS ORDER BY NAME ", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            m_pGroupName->setText(c->values[c->checked[0]].toString());
            m_group = c->key[c->checked[0]].toString();
        }
        else
        {
            m_pGroupName->clear();
            m_group = "0";
        }
    }
    delete c;
}

void EGroupChange::slotStore()
{
    CheckList *c = new CheckList(false, this);
    c->sql("SELECT ID, NAME FROM STORAGE ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            m_pStoreName->setText(c->values[c->checked[0]].toString());
            m_store = c->key[c->checked[0]].toString();
        }
        else
        {
            m_pStoreName->clear();
            m_store = "0";
        }
    }
    delete c;
}

void EGroupChange::slotPrint()
{    CheckList *c = new CheckList(false, this);
     c->sql("SELECT ID, NAME FROM PRINTERS ORDER BY NAME", 1);

     if (c->exec() == QDialog::Accepted)
     {
         m_pPrintName->setText("");
         m_print = "";
         if (c->result.count() > 0)
         {
             m_pPrintName->setText(c->values[c->checked[0]].toString());
             m_print = c->key[c->checked[0]].toString();
         }
     }
     delete c;
}

void EGroupChange::slotColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (!color.isValid())
        return;

    m_pColor->setText(color.name());
    QPalette p(m_pColor->palette());
    p.setColor(m_pColor->backgroundRole(), color);
    m_pColor->setPalette(p);
}


void EGroupChange::slotImNotIdiot(bool checked)
{
    m_pbtnOk->setEnabled(checked);
}
