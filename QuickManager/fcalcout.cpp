#include "fcalcout.h"

FCalcOut::FCalcOut(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Filter"));
    int i = -1;
    QGridLayout *mainLayout = new QGridLayout(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(this);
    QWidget *wdtButton = new QWidget(this);
    QPushButton *pbtnOk = new QPushButton(tr("OK"), this);
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    QPushButton *pbtnStore = new QPushButton("...", this);

    m_pDate1 = new QDateEdit(this);
    m_pDate2 = new QDateEdit(this);
    m_pStore = new QLineEdit(this);

    m_pStore->setReadOnly(true);
    pbtnStore->setMaximumSize(50, 50);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(pbtnStore, SIGNAL(clicked()), SLOT(store()));

    mainLayout->addWidget(new QLabel(tr("Date range"), this), ++i, 0);
    mainLayout->addWidget(m_pDate1, i, 1);
    mainLayout->addWidget(m_pDate2, i, 2);

    mainLayout->addWidget(new QLabel(tr("Store (blank for all)"), this), ++i, 0);
    mainLayout->addWidget(m_pStore, i, 1, 1, 2);
    mainLayout->addWidget(pbtnStore, i, 3);

    btnLayout->addStretch(1);
    btnLayout->addWidget(pbtnOk);
    btnLayout->addWidget(pbtnCancel);
    btnLayout->addStretch(1);
    wdtButton->setLayout(btnLayout);

    mainLayout->addWidget(wdtButton, ++i, 0, 1, 3);
    setLayout(mainLayout);
}

void FCalcOut::store()
{
}
