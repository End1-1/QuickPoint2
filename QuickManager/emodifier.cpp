#include "emodifier.h"

EModifier::EModifier(QWidget *parent)
    :EditWindow(parent)
{
    setWindowTitle(tr("Modifier"));
    int i = -1;
    QGridLayout *l = new QGridLayout(this);

    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(m_pName = new QLineEdit(), i, 1);

    l->addWidget(new QLabel(tr("Action")), ++i, 0);
    l->addWidget(m_pAction = new QComboBox(), i, 1);
    QSqlQuery *q = m_db.QuerySelect("SELECT ID, NAME FROM MODIFIERS_TYPES ORDER BY NAME");
    while (q->next())
        m_pAction->addItem(q->value(1).toString(), q->value(0).toInt());
    delete q;
    m_db.db.close();

    l->addWidget(m_pwidget, ++i, 0, 1, 2);
}

bool EModifier::checkData()
{
    if (m_pName->text().length() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty"));
        return false;
    }
    return true;
}
