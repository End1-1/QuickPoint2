#include "fdish.h"
#include "checklist.h"

FDish::FDish(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Filter for dishes"));
    int i = -1;
    QGridLayout *l = new QGridLayout();
    l->setColumnMinimumWidth(1, 200);
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
    QPushButton *pbtn_Group = new QPushButton("...");
    pbtn_Group->setMaximumSize(25, 25);
    connect(pbtn_Group, SIGNAL(clicked()), SLOT(slotGroup()));
    l->addWidget(pbtn_Group, i, 2);
    l->addWidget(new QLabel(tr("Store")), ++i, 0);
    l->addWidget(pedt_Store = new QLineEdit(), i, 1);
    pedt_Store->setEnabled(false);
    QPushButton *pbtn_Store = new QPushButton("...");
    pbtn_Store->setMaximumSize(25, 25);
    connect(pbtn_Store, SIGNAL(clicked()), SLOT(slotStore()));
    l->addWidget(pbtn_Store, i, 2);
    l->addWidget(new QLabel(tr("Visible")), ++i, 0);
    l->addWidget(pchk_Visible = new QCheckBox(), i, 1);

    l->addWidget(new QLabel(tr("No recipe"), this), ++i, 0);
    l->addWidget(pchk_NoRecipe = new QCheckBox(this), i, 1);

    l->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(l);
}

bool FDish::checkData()
{
    return true;
}

void FDish::slotMenu()
{
    CheckList *c = new CheckList(true, this);

    if (pedt_Menu->text().length() > 0)
        c->splitResult(pedt_Menu->text());
    c->sql("SELECT ID, NAME FROM MENU ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Menu->clear();
        if (c->result.count() > 0)
        {
            pedt_Menu->clear();
            for (QVector<QVariant>::const_iterator i = c->result.begin(); i != c->result.end(); i++)
                pedt_Menu->setText(pedt_Menu->text() + i->toString() + ",");
            pedt_Menu->setText(pedt_Menu->text().remove(pedt_Menu->text().length() - 1, 1));
        }
    }
    delete c;
}

void FDish::slotGroup()
{
    CheckList *c = new CheckList(true, this);

    if (pedt_Group->text().length() > 0)
        c->splitResult(pedt_Group->text());
    c->sql("SELECT ID, NAME FROM DISHES_GROUPS ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Group->clear();
        if (c->result.count() > 0)
        {
            for (QVector<QVariant>::const_iterator i = c->result.begin(); i != c->result.end(); i++)
                pedt_Group->setText(pedt_Group->text() + i->toString() + ",");
            pedt_Group->setText(pedt_Group->text().remove(pedt_Group->text().length() - 1, 1));
        }
    }
    delete c;
}

void FDish::slotStore()
{
    CheckList *c = new CheckList(true, this);

    if (pedt_Store->text().length() > 0)
        c->splitResult(pedt_Store->text());
    c->sql("SELECT ID, NAME FROM STORAGE ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Store->clear();
        if (c->result.count() > 0)
        {
            for (QVector<QVariant>::const_iterator i = c->result.begin(); i != c->result.end(); i++)
                pedt_Store->setText(pedt_Store->text() + i->toString() + ",");
            pedt_Store->setText(pedt_Store->text().remove(pedt_Store->text().length() - 1, 1));
        }
    }
    delete c;
}
