#include "qdbpair.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

QDBPair::QDBPair(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Database pair"));
    setMinimumSize(300, 300);

    m_pName = new QLineEdit(this);
    m_pDstDb = new QLineEdit(this);
    m_pDstUser = new QLineEdit(this);
    m_pDstPass = new QLineEdit(this);
    m_pDstPass->setEchoMode(QLineEdit::Password);
    m_pSrcDb = new QLineEdit(this);
    m_pSrcConnStr = new QLineEdit(this);
    m_pSrcUser = new QLineEdit(this);
    m_pSrcPass = new QLineEdit(this);
    m_pSrcPass->setEchoMode(QLineEdit::Password);
    m_pstoreConverter = new QComboBox(this);
    m_pfoodConverter = new QComboBox(this);
    m_pOperatorId = new QLineEdit(this);

    QGridLayout *l = new QGridLayout(this);
    int i = -1;

    l->setColumnMinimumWidth(1, 400);
    l->addWidget(new QLabel(tr("Name"), this), ++i, 0);
    l->addWidget(m_pName, i, 1);
    l->addWidget(new QLabel(tr("Database name"), this), ++i, 0);
    l->addWidget(m_pSrcDb, i, 1);
    l->addWidget(new QLabel(tr("Source database connection string"), this), ++i, 0);
    l->addWidget(m_pSrcConnStr, i, 1);
    l->addWidget(new QLabel(tr("Source username"), this), ++i, 0);
    l->addWidget(m_pSrcUser, i, 1);
    l->addWidget(new QLabel(tr("Source password"), this), ++i, 0);
    l->addWidget(m_pSrcPass, i, 1);

    l->addWidget(new QLabel(tr("Destination database"), this), ++i, 0);
    l->addWidget(m_pDstDb, i, 1);
    l->addWidget(new QLabel(tr("Destination username"), this), ++i, 0);
    l->addWidget(m_pDstUser, i, 1);
    l->addWidget(new QLabel(tr("Destination password"), this), ++i, 0);
    l->addWidget(m_pDstPass);

    l->addWidget(new QLabel(tr("Storage converter"), this), ++i, 0);
    l->addWidget(m_pstoreConverter, i, 1);
    l->addWidget(new QLabel(tr("Food converter"), this), ++i, 0);
    l->addWidget(m_pfoodConverter, i, 1);

    l->addWidget(new QLabel(tr("Operator id")), ++i, 0);
    l->addWidget(m_pOperatorId, i, 1);

    QPushButton *pbOK = new QPushButton(tr("OK"), this);
    QPushButton *pbCancel = new QPushButton(tr("Cancel"), this);
    l->addWidget(pbOK, ++i, 0, 1, 1, Qt::AlignRight);
    l->addWidget(pbCancel, i, 1, 1, 1, Qt::AlignLeft);

    connect(pbOK, SIGNAL(clicked()), SLOT(OK()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(reject()));

    setLayout(l);
}

void QDBPair::makeStoreMap(const QMap<QString, QMap<QString, int> > &map)
{
    for (QMap<QString, QMap<QString, int> >::const_iterator i = map.begin(); i != map.end(); i++)
        m_pstoreConverter->addItem(i.key());
}

void QDBPair::makeFoodMap(const QMap<QString, QMap<QString, int> > &map)
{
    for (QMap<QString, QMap<QString, int> >::const_iterator i = map.begin(); i != map.end(); i++)
        m_pfoodConverter->addItem(i.key());
}

void QDBPair::OK()
{
    accept();
}
