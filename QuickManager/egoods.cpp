#include "egoods.h"
#include "checklist.h"

EGoods::EGoods(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Goods"));
    int i = -1;
    QGridLayout *l = new QGridLayout();
    l->setColumnMinimumWidth(1, 300);

    l->addWidget(new QLabel(tr("Id")), ++i, 0);
    l->addWidget(pedt_Id = new QLineEdit(), i, 1);
    pedt_Id->setEnabled(false);

    l->addWidget(new QLabel(tr("Group")), ++i, 0);
    l->addWidget(pedt_Group = new QLineEdit(), i, 1);
    pedt_Group->setEnabled(false);
    QPushButton *pbtn_Group = new QPushButton("...");
    pbtn_Group->setMaximumSize(25, 25);
    connect(pbtn_Group, SIGNAL(clicked()), SLOT(slotGroup()));
    l->addWidget(pbtn_Group, i, 2);

    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(pedt_Name = new QLineEdit(), i, 1);

    l->addWidget(new QLabel(tr("Unit")), ++i, 0);
    l->addWidget(pedt_Unit = new QLineEdit(), i, 1);
    pedt_Unit->setEnabled(false);
    QPushButton *pbtn_Unit = new QPushButton("...");
    pbtn_Unit->setMaximumSize(25, 25);
    connect(pbtn_Unit, SIGNAL(clicked()), SLOT(slotUnit()));
    l->addWidget(pbtn_Unit, i, 2);

    l->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(l);
}

bool EGoods::checkData()
{
    QString msg = "";
    bool result = true;

    if (pedt_Name->text().length() == 0)
    {
        msg += tr("The name cannot be empty\n");
        result = false;
    }
    if (pedt_Group->text().length() == 0)
    {
        msg += tr("The group cannot be empty\n");
        result = false;
    }
    if (pedt_Unit->text().length() == 0)
    {
        msg += tr("Then unit name cannot be empty\n");
        result = false;
    }
    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);
    return result;
}

void EGoods::slotGroup()
{
    CheckList *c = new CheckList(false, this);

    if (m_group.length() > 0)
        c->result.push_back(m_group);
    c->sql("SELECT ID, NAME FROM GOODS_GROUPS ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Group->setText("");
        m_group = "";
        if (c->result.count() > 0)
        {
            pedt_Group->setText(c->values[c->checked[0]].toString());
            m_group = c->key[c->checked[0]].toString();
        }
    }
    delete c;
}

void EGoods::slotUnit()
{
    CheckList *c = new CheckList(false, this);

    if (m_unit.length() > 0)
        c->result.push_back(m_unit);
    c->sql("SELECT ID, NAME FROM UNITS ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Unit->setText("");
        m_unit = "";
        if (c->result.count() > 0)
        {
            pedt_Unit->setText(c->values[c->checked[0]].toString());
            m_unit = c->key[c->checked[0]].toString();
        }
    }
    delete c;
}
