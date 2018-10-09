#include "amodifiers.h"

AModifiers::AModifiers(QWidget *parent):
    EditWindow(parent)
{
    setWindowTitle(tr("Modifier"));
    int i = -1;
    QGridLayout *l = new QGridLayout(this);

    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(m_pName = new QComboBox(), i, 1);
    l->addWidget(m_pwidget, ++i, 0, 1, 2);

    QSqlQuery *q = m_db.QuerySelect("SELECT ID, NAME FROM MODIFIERS ORDER BY NAME");
    while (q->next())
        m_pName->addItem(q->value(1).toString(), q->value(0).toInt());
    delete q;
    m_db.db.close();

    setLayout(l);
}

bool AModifiers::checkData()
{
    if (m_pName->currentIndex() < 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return false;
    }
    return true;
}
