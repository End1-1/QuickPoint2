#include "edish.h"
#include "checklist.h"
#include "ecomboitem.h"
#include "amodifiers.h"

EDish::EDish(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Dish"));

    int i = -1;
    QWidget *pmainWidget = new QWidget(this);
    QGridLayout *l = new QGridLayout();
    l->setColumnMinimumWidth(1, 250);
    l->addWidget(new QLabel(tr("Id")), ++i, 0);
    l->addWidget(pedt_Id = new QLineEdit(), i, 1);
    pedt_Id->setEnabled(false);
    l->addWidget(new QLabel(tr("Menu")), ++i, 0);
    l->addWidget(pedt_Menu = new QLineEdit(), i, 1);
    pedt_Menu->setEnabled(false);
    QPushButton *pbtn_Menu = new QPushButton("...");
    pbtn_Menu->setMaximumSize(25, 25);
    connect(pbtn_Menu, SIGNAL(clicked()), SLOT(slotMenu()));
    l->addWidget(pbtn_Menu, i, 2);
    l->addWidget(new QLabel(tr("Group")), ++i, 0);
    l->addWidget(pedt_Group = new QLineEdit(), i, 1);
    pedt_Group->setEnabled(false);
    QPushButton *pbtn_Group = new QPushButton(tr("..."));
    pbtn_Group->setMaximumSize(25, 25);
    connect(pbtn_Group, SIGNAL(clicked()), SLOT(slotGroup()));
    l->addWidget(pbtn_Group, i, 2);
    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(pedt_Name = new QLineEdit(), i, 1);
    l->addWidget(new QLabel(tr("Price")), ++i, 0);
    l->addWidget(pedt_Price = new QLineEdit(), i, 1);
    l->addWidget(new QLabel(tr("Store")), ++i, 0);
    l->addWidget(pedt_Store = new QLineEdit(), i, 1);
    pedt_Store->setEnabled(false);
    QPushButton *pbtn_Store = new QPushButton("...");
    pbtn_Store->setMaximumSize(25, 25);
    connect(pbtn_Store, SIGNAL(clicked()), SLOT(slotStore()));
    l->addWidget(pbtn_Store, i, 2);

    l->addWidget(new QLabel(tr("Printer")), ++i, 0);
    l->addWidget(pedt_Printer = new QLineEdit(), i, 1);
    pedt_Printer->setEnabled(false);
    QPushButton *pbtn_Printer = new QPushButton (tr("..."));
    pbtn_Printer->setMaximumSize(25, 25);
    connect(pbtn_Printer, SIGNAL(clicked()), SLOT(slotPrinter()));
    l->addWidget(pbtn_Printer, i, 2);

    l->addWidget(new QLabel(tr("Visible")), ++i, 0);
    l->addWidget(pchk_Visible = new QCheckBox(), i, 1);

    pbtn_Color = new QPushButton(tr("Color"));
    connect(pbtn_Color, SIGNAL(clicked()), SLOT(slotChangeColor()));
    m_color = new QColor(Qt::white);
    setColor();
    l->addWidget(pbtn_Color, ++i, 0, 1, 3);

    l->addWidget(new QLabel(tr("Queue")), ++i, 0);
    l->addWidget(pedt_Queue = new QLineEdit(), i, 1);
    l->addWidget(new QLabel(tr("Print queue")), ++i, 0);
    l->addWidget(m_pPrintQueue = new QLineEdit(this), i, 1);

    pmainWidget->setLayout(l);

    i = -1;
    QWidget *pcomboWidget = new QWidget(this);
    QGridLayout *lc = new QGridLayout(this);
    lc->addWidget(new QLabel(tr("Combo")), ++i, 0);
    lc->addWidget(m_pchkCombo = new QCheckBox(), i, 1);
    connect(m_pchkCombo, SIGNAL(clicked(bool)), SLOT(slotCheckCombo(bool)));

    QWidget *peditButtons = new QWidget(this);
    QHBoxLayout *leb = new QHBoxLayout(this);
    m_pbNew = new QPushButton(tr("New"), this);
    m_pbEdit = new QPushButton(tr("Edit"), this);
    m_pbDel = new QPushButton(tr("Delete"), this);
    connect(m_pbNew, SIGNAL(clicked()), SLOT(slotNewCombo()));
    connect(m_pbEdit, SIGNAL(clicked()), SLOT(slotEditCombo()));
    connect(m_pbDel, SIGNAL(clicked()), SLOT(slotDelCombo()));
    leb->addWidget(m_pbNew);
    leb->addWidget(m_pbEdit);
    leb->addWidget(m_pbDel);
    peditButtons->setLayout(leb);
    lc->addWidget(peditButtons, ++i, 0, 1, 3);

    m_ptCombo = new TableView(this);
    m_ptCombo->setMinimumSize(380, 300);
    m_ptCombo->m_model.insertColumns(0, 6, QModelIndex());
    m_ptCombo->setColumn(0, "DISH_ID", tr("Dish id"), 0, DATA_INTEGER);
    m_ptCombo->setColumn(1, "NAME", tr("Print name"), 150, DATA_STRING);
    m_ptCombo->setColumn(2, "QTY", tr("Quantity"), 50, DATA_INTEGER);
    m_ptCombo->setColumn(3, "PRICE", tr("Price"), 100, DATA_DOUBLE);
    m_ptCombo->setColumn(4, "PRINTER_ID", tr("Printer code"), 0, DATA_INTEGER);
    m_ptCombo->setColumn(5, "PRINTER", tr("Printer"), 150, DATA_STRING);
    lc->addWidget(m_ptCombo, ++i , 0, 1, 3);

    slotCheckCombo(false);
    pcomboWidget->setLayout(lc);

    QWidget *pModifierWidget = new QWidget(this);
    QVBoxLayout *lm = new QVBoxLayout(this);

    QWidget *peditButtonsMod = new QWidget(this);
    QHBoxLayout *lebm = new QHBoxLayout(this);
    m_pbNewMod = new QPushButton(tr("New"), this);
    m_pbEditMod = new QPushButton(tr("Edit"), this);
    m_pbDelMod = new QPushButton(tr("Delete"), this);
    connect(m_pbNewMod, SIGNAL(clicked()), SLOT(slotNewMod()));
    connect(m_pbEditMod, SIGNAL(clicked()), SLOT(slotEditMod()));
    connect(m_pbDelMod, SIGNAL(clicked()), SLOT(slotDelMod()));
    lebm->addWidget(m_pbNewMod);
    lebm->addWidget(m_pbEditMod);
    lebm->addWidget(m_pbDelMod);
    peditButtonsMod->setLayout(lebm);

    m_ptModifiers = new TableView(this);
    m_ptModifiers->setMinimumSize(380, 300);
    m_ptModifiers->m_model.insertColumns(0, 2, QModelIndex());
    m_ptModifiers->setColumn(0, "ID", tr("Id"), 0, DATA_INTEGER);
    m_ptModifiers->setColumn(1, "NAME", tr("Name"), 300, DATA_STRING);

    lm->addWidget(peditButtonsMod);
    lm->addWidget(m_ptModifiers);
    pModifierWidget->setLayout(lm);

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(pmainWidget, tr("Main"));
    tab->addTab(pcomboWidget, tr("Combo"));
    tab->addTab(pModifierWidget, tr("Modifiers"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tab);
    mainLayout->addWidget(m_pwidget);
}

void EDish::setColor()
{
    QPalette p(pbtn_Color->palette());
    p.setColor(pbtn_Color->backgroundRole(), *m_color);
    pbtn_Color->setPalette(p);
}

bool EDish::checkData()
{
    QString msg = "";
    bool result = true;
    if (pedt_Menu->text().length() == 0)
    {
        msg += tr("The menu cannot be empty\n");
        result = false;
    }
    if (pedt_Group->text().length() == 0)
    {
        msg += tr("The group cannot be empty\n");
        result = false;
    }
    if (pedt_Name->text().length() == 0)
    {
        msg += tr("The name cannot be empty\n");
        result = false;
    }
    if (pedt_Price->text().toDouble() == 0)
    {
        msg += tr("The price cannot be 0\n");
        result = false;
    }
    if (pedt_Store->text().length() == 0)
    {
        msg += tr("The store cannot be empty\n");
        result = false;
    }
    if (pedt_Printer->text().length() == 0)
    {
        msg += tr("The printer cannot be empty\n");
        result = false;
    }

    if (m_pchkCombo->isChecked() && m_ptCombo->m_model.rowCount(QModelIndex()) == 0)
    {
        msg += tr("Combo option checked, but list of combo items empty\n");
        result = false;
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EDish::setComboData(const EComboItem *e, const int &row)
{
    m_ptCombo->m_model.setData(row, "DISH_ID", e->m_pDishId);
    m_ptCombo->m_model.setData(row, "NAME", e->m_pDishName->text());
    m_ptCombo->m_model.setData(row, "QTY", e->m_pQty->text());
    m_ptCombo->m_model.setData(row, "PRICE", e->m_pPrice->text());
    m_ptCombo->m_model.setData(row, "PRINTER_ID", e->m_pPrinterName->itemData(e->m_pPrinterName->currentIndex()));
    m_ptCombo->m_model.setData(row, "PRINTER", e->m_pPrinterName->currentText());
}

void EDish::setModData(const AModifiers *a, const int &row)
{
    m_ptModifiers->m_model.setData(row, "ID", a->m_pName->itemData(a->m_pName->currentIndex()));
    m_ptModifiers->m_model.setData(row, "NAME", a->m_pName->currentText());
}

void EDish::slotMenu()
{
    CheckList *c = new CheckList(false, this);

    if (m_menu.length() > 0)
        c->result.push_back(m_menu);
    c->sql("SELECT ID, NAME FROM MENU ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            pedt_Menu->setText(c->values[c->checked[0]].toString());
            m_menu = c->key[c->checked[0]].toString();
        }
        else
        {
            pedt_Menu->clear();
            m_menu = "0";
        }
    }
    delete c;
}

void EDish::slotGroup()
{
    CheckList *c = new CheckList(false, this);

    if (m_group.length() > 0)
        c->result.push_back(m_group);
    c->sql("SELECT ID, NAME FROM DISHES_GROUPS ORDER BY NAME ", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            pedt_Group->setText(c->values[c->checked[0]].toString());
            m_group = c->key[c->checked[0]].toString();
        }
        else
        {
            pedt_Group->clear();
            m_group = "0";
        }
    }
    delete c;
}

void EDish::slotStore()
{
    CheckList *c = new CheckList(false, this);

    if (m_store.length() > 0)
        c->result.push_back(m_store);
    c->sql("SELECT ID, NAME FROM STORAGE ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        if (c->result.count() > 0)
        {
            pedt_Store->setText(c->values[c->checked[0]].toString());
            m_store = c->key[c->checked[0]].toString();
        }
        else
        {
            pedt_Store->clear();
            m_store = "0";
        }
    }
    delete c;
}

void EDish::slotPrinter()
{
    CheckList *c = new CheckList(false, this);

    if (m_store.length() > 0)
        c->result.push_back(m_store);
    c->sql("SELECT ID, NAME FROM PRINTERS ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Printer->setText("");
        m_printer = "";
        if (c->result.count() > 0)
        {
            pedt_Printer->setText(c->values[c->checked[0]].toString());
            m_printer = c->key[c->checked[0]].toString();
        }
    }
    delete c;
}

void EDish::slotChangeColor()
{
    *m_color = QColorDialog::getColor(*m_color);
    if (!(*m_color).isValid())
        return;
    setColor();
}

void EDish::slotCheckCombo(const bool &value)
{
    m_pbNew->setEnabled(value);
    m_pbEdit->setEnabled(value);
    m_pbDel->setEnabled(value);
    m_ptCombo->setEnabled(value);
}

void EDish::slotNewCombo()
{
    EComboItem *e = new EComboItem(this);
    if (e->exec() == QDialog::Accepted)
    {
        int i = m_ptCombo->addRow();
        setComboData(e, i);
    }
    delete e;
}

void EDish::slotEditCombo()
{
    QVector<int> rows;
    m_ptCombo->getSelectedRows(rows);
    if (rows.count() == 0)
        return;

    EComboItem *e = new EComboItem(this);
    for (int i = 0; i < rows.count(); i++)
    {
        e->m_pDishId = m_ptCombo->m_model.data("DISH_ID", rows[i]).toString();
        e->m_pDishName->setText(m_ptCombo->m_model.data("NAME", rows[i]).toString());
        e->m_pQty->setText(m_ptCombo->m_model.data("QTY", rows[i]).toString());
        e->m_pPrice->setText(m_ptCombo->m_model.data("PRICE", rows[i]).toString());
        for (int j = 0; j < e->m_pPrinterName->count(); j++)
            if (m_ptCombo->m_model.data("PRINTER_ID", rows[i]).toString() == e->m_pPrinterName->itemData(j).toString())
                e->m_pPrinterName->setCurrentIndex(j);
        if (e->exec() == QDialog::Accepted)
            setComboData(e, rows[i]);
    }
    delete e;
}

void EDish::slotDelCombo()
{
    QVector<int> rows;
    m_ptCombo->getSelectedRows(rows);
    if (rows.count() == 0)
        return;

    for (int i = rows.count() - 1; i > -1; i--)
        m_ptCombo->m_model.removeRows(rows[i], 1, QModelIndex());
}

void EDish::slotNewMod()
{
    AModifiers *a = new AModifiers(this);
    if (a->exec() == QDialog::Accepted)
    {
        int row = m_ptModifiers->addRow();
        setModData(a, row);
    }
    delete a;
}

void EDish::slotEditMod()
{
    QVector<int> rows;
    m_ptModifiers->getSelectedRows(rows);
    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    AModifiers *a = new AModifiers;
    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
    {
        for (int j = 0; j < a->m_pName->count(); j++)
            if (m_ptModifiers->m_model.data("ID", *i).toString() == a->m_pName->itemData(j).toString())
            {
                a->m_pName->setCurrentIndex(j);
                break;
            }
        if (a->exec() == QDialog::Accepted)
            setModData(a, *i);
    }
    delete a;
}

void EDish::slotDelMod()
{
    QVector<int> rows;
    m_ptModifiers->getSelectedRows(rows);
    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    std::reverse(rows.begin(), rows.end());
    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
        m_ptModifiers->m_model.removeRows(*i, 1, QModelIndex());
}
