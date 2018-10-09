#include "qlistdialog.h"

QListDialog::QListDialog(QWidget *parent) :
    QBaseDialog(tr("List"), parent)
{
    QHBoxLayout *plButtons = new QHBoxLayout();
    QWidget *pwdtButtons = new QWidget(this);
    QPushButton *pbtnDown = qpushbutton(tr("Down"));
    QPushButton *pbtnUp = qpushbutton(tr("Up"));
    QPushButton *pbtnClose = qpushbutton(tr("Close"));
    plButtons->addWidget(pbtnDown);
    plButtons->addWidget(pbtnUp);
    plButtons->addStretch(1);
    plButtons->addWidget(pbtnClose);
    pwdtButtons->setLayout(plButtons);

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(m_pList = new QListWidget(this));
    plMain->addWidget(pwdtButtons);
    setLayout(plMain);

    m_selectedIndex = -1;

    connect(pbtnDown, SIGNAL(clicked()), SLOT(OnDown()));
    connect(pbtnUp, SIGNAL(clicked()), SLOT(OnUp()));
    connect(pbtnClose, SIGNAL(clicked()), SLOT(reject()));
    connect(m_pList, SIGNAL(clicked(QModelIndex)), SLOT (OnListClicked(QModelIndex)));
}

void QListDialog::OnDown()
{
    m_pList->verticalScrollBar()->setValue(m_pList->verticalScrollBar()->value() + 500);
}

void QListDialog::OnUp()
{
    m_pList->verticalScrollBar()->setValue(m_pList->verticalScrollBar()->value() - 500);
}

void QListDialog::OnListClicked(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (!m_pList->selectedItems().count())
        return;

    accept();
}
