#include "femployee.h"
#include "checklist.h"

FEmployee::FEmployee(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Employee filter"));
    int i = -1;
    QGridLayout *l = new QGridLayout();

    l->addWidget(new QLabel(tr("Position")), ++i, 0);
    l->addWidget(pedt_Position = new QLineEdit(), i, 1);
    pedt_Position->setEnabled(false);
    QPushButton *pbtn_Position = new QPushButton("...");
    pbtn_Position->setMaximumSize(25, 25);
    connect(pbtn_Position, SIGNAL(clicked()), SLOT(slotPosition()));
    l->addWidget(pbtn_Position, i, 2);

    l->addWidget(new QLabel(tr("Active")), ++i, 0);
    l->addWidget(pchk_Active = new QCheckBox(), i, 1);

    l->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(l);
}

bool FEmployee::checkData()
{
    return true;
}

void FEmployee::slotPosition()
{
    CheckList *c = new CheckList(true, this);

    if (pedt_Position->text().length() > 0)
        c->splitResult(pedt_Position->text());

    c->sql("SELECT ID, NAME FROM POSITIONS ORDER BY NAME", 1);
    if (c->exec() == QDialog::Accepted)
    {
        pedt_Position->clear();
        if (c->result.count() > 0)
            pedt_Position->setText(pedt_Position->text() + c->key[c->checked[0]].toString() + ",");
        pedt_Position->setText(pedt_Position->text().remove(pedt_Position->text().length() - 1, 1));
    }
    delete c;
}
