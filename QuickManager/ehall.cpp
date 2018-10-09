#include "ehall.h"

EHall::EHall(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Hall"));
    QGridLayout *layout = new QGridLayout(this);

    int i = -1;
    layout->addWidget(new QLabel(tr("Id"), this), ++i, 0);
    layout->addWidget(m_pId = new QLineEdit(this), i, 1);
    m_pId->setEnabled(false);
    layout->addWidget(new QLabel(tr("Name"), this), ++i, 0);
    layout->addWidget(m_pName = new QLineEdit(this), i, 1);
    m_pName->setFocus();
    layout->addWidget(m_pwidget, ++i, 0, 1, 2);
    setLayout(layout);
}

bool EHall::checkData()
{
    if (m_pName->text().length() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty\n"));
        return false;
    }
    return true;
}
