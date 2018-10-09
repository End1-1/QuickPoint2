#include "fsaleout.h"
#include "checklist2.h"
#include "global.h"
#include "../All/econnection.h"

FSaleOut::FSaleOut(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Sale output paramerters"));
    EConnection::setConnectionParams(db.db, Data::databaseIndex);
    db.db.open();
    QGridLayout *l = new QGridLayout();
    int i = -1;

    l->addWidget(new QLabel(tr("Date range")), ++i, 0);
    l->addWidget(m_pdDate1 = new QDateEdit(QDate::currentDate()), i, 1);
    l->addWidget(m_pdDate2 = new QDateEdit(QDate::currentDate()), i, 2);

    l->addWidget(new QLabel(tr("Document date")), ++i, 0);
    l->addWidget(m_pdDocDate =new QDateEdit(QDate::currentDate()), i, 1);

    QPushButton *pbStore = new QPushButton("...");
    pbStore->setMaximumSize(25, 25);
    connect(pbStore, SIGNAL(clicked()), SLOT(slotStore()));
    l->addWidget(new QLabel(tr("Store")), ++i, 0);
    l->addWidget(m_peStore = new QLineEdit(), i, 1);
    l->addWidget(pbStore, i, 3);
    l->addWidget(m_plStore = new QLabel(""), i, 4);

    l->addWidget(m_pwidget, ++i, 0, 1, 4);
    setLayout(l);
}

FSaleOut::~FSaleOut()
{
    db.db.close();
    delete m_pdDate1;
    delete m_pdDate2;
    delete m_pdDocDate;
    delete m_peStore;
    delete m_plStore;
}

bool FSaleOut::checkData()
{
    if (m_plStore->text().length() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Store is not selected"));
        return false;
    }
    return true;
}

void FSaleOut::slotStore()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_plStore->clear();
        m_peStore->clear();
        if (c->m_result.count() > 0)
        {
            m_plStore->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
            m_peStore->setText(c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString());
        }
    }
    delete c;
}

void FSaleOut::slotCancel()
{
    m_peStore->clear();
    reject();
}

void FSaleOut::slotStoreByCode()
{
    m_plStore->clear();
    QSqlQuery *q = new QSqlQuery(db.db);
    q->prepare("SELECT NAME FROM STORAGE WHERE ID=:ID");
    q->bindValue("ID", m_peStore->text());
    q->exec();
    if (q->next())
        m_plStore->setText(q->value(0).toString());
    delete q;
}
