#include "qmoveorderdialog.h"

QMoveOrderDialog::QMoveOrderDialog(QWidget *parent) :
    QBaseDialog(tr("Move dialog"), parent)
{
    QHBoxLayout *plFirst = new QHBoxLayout();
    QWidget *pwdtFirst = new QWidget(this);

    QVBoxLayout *plLeft = new QVBoxLayout();
    QWidget *pwdtLeft = new QWidget(this);
    m_pSrcTable = new QListWidget(this);
    m_pSrcTableName = new QLabel("Source table", this);
    plLeft->addWidget(m_pSrcTableName);
    plLeft->addWidget(m_pSrcTable);
    pwdtLeft->setLayout(plLeft);

    QVBoxLayout *plRight = new QVBoxLayout();
    QWidget *pwdtRight = new QWidget(this);
    m_pDstTableName = new QLabel("Destination table", this);
    m_pDstTable = new QListWidget(this);
    plRight->addWidget(m_pDstTableName);
    plRight->addWidget(m_pDstTable);
    pwdtRight->setLayout(plRight);

    QVBoxLayout *plMoveButtons = new QVBoxLayout();
    QWidget *pwdtMoveButtons = new QWidget(this);
    QPushButton *pbtnLeft = new QPushButton(">", this);
    pbtnLeft->setMinimumSize(QSize(100, 60));
    QPushButton *pbtnRight = new QPushButton("<", this);
    pbtnRight->setMinimumSize(QSize(100, 60));
    QPushButton *pbtnAllLeft = new QPushButton(">>>", this);
    pbtnAllLeft->setMinimumSize(QSize(100, 60));
    QPushButton *pbtnAllRight = new QPushButton("<<<", this);
    pbtnAllRight->setMinimumSize(QSize(100, 60));
    plMoveButtons->addStretch(1);
    plMoveButtons->addWidget(pbtnLeft);
    plMoveButtons->addWidget(pbtnRight);
    plMoveButtons->addWidget(pbtnAllLeft);
    plMoveButtons->addWidget(pbtnAllRight);
    plMoveButtons->addStretch(1);
    pwdtMoveButtons->setLayout(plMoveButtons);

    plFirst->addWidget(pwdtLeft);
    plFirst->addWidget(pwdtMoveButtons);
    plFirst->addWidget(pwdtRight);
    pwdtFirst->setLayout(plFirst);

    QHBoxLayout *plSecond = new QHBoxLayout();
    QWidget *pwdtSecond = new QWidget(this);
    QPushButton *pbtnOk = new QPushButton(tr("Ok"), this);
    pbtnOk->setMinimumSize(QSize(100, 60));
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    pbtnCancel->setMinimumSize(QSize(100, 60));
    plSecond->addStretch(1);
    plSecond->addWidget(pbtnOk);
    plSecond->addWidget(pbtnCancel);
    plSecond->addStretch(1);
    pwdtSecond->setLayout(plSecond);

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(pwdtFirst);
    plMain->addWidget(pwdtSecond);
    setLayout(plMain);

    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pbtnLeft, SIGNAL(clicked()), SLOT(OnMoveLeft()));
    connect(pbtnRight, SIGNAL(clicked()), SLOT(OnMoveRight()));
    connect(pbtnAllLeft, SIGNAL(clicked()), SLOT(OnMoveAllLeft()));
    connect(pbtnAllRight, SIGNAL(clicked()), SLOT(OnMoveAllRight()));
}

void QMoveOrderDialog::moveItem(QListWidget *src, QListWidget *dst)
{
    QList<QListWidgetItem *> l = src->selectedItems();
    if (!l.count())
        return;

    QListWidgetItem *wi = new QListWidgetItem(dst);
    wi->setText(l[0]->data(Qt::DisplayRole).toString());
    wi->setSizeHint(QSize(100, 60));
    wi->setData(Qt::UserRole, l[0]->data(Qt::UserRole));

    delete l[0];
}

void QMoveOrderDialog::OnMoveLeft()
{
    moveItem(m_pSrcTable, m_pDstTable);
}

void QMoveOrderDialog::OnMoveRight()
{
    moveItem(m_pDstTable, m_pSrcTable);
}

void QMoveOrderDialog::OnMoveAllLeft()
{
    while(m_pSrcTable->count())
    {
        m_pSrcTable->setItemSelected(m_pSrcTable->item(0), true);
        moveItem(m_pSrcTable, m_pDstTable);
    }
}

void QMoveOrderDialog::OnMoveAllRight()
{
    while(m_pDstTable->count())
    {
        m_pDstTable->setItemSelected(m_pDstTable->item(0), true);
        moveItem(m_pDstTable, m_pSrcTable);
    }
}
