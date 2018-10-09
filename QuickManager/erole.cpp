#include "erole.h"

ERole::ERole(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Roles"));
    int i = -1;
    QGridLayout *l = new QGridLayout();
    l->addWidget(new QLabel(tr("Id")), ++i, 0);
    l->addWidget(pedt_Id = new QLineEdit(), i, 1);
    pedt_Id->setEnabled(false);
    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(pedt_Name = new QLineEdit(), i, 1);
    l->addWidget(m_pwidget, ++i, 0, 1, 2);
    setLayout(l);
}

bool ERole::checkData()
{
    if (pedt_Name->text().length() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty"));
        return false;
    }
    return true;
}
