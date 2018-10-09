#include "fstoredoc.h"
#include "checklist2.h"
#include "global.h"
#include "../All/econnection.h"

FStoreDoc::FStoreDoc(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Filter for documents in the store"));
    int i = -1;
    QGridLayout *l = new QGridLayout();

    l->addWidget(new QLabel(tr("Document id")), ++i, 0);
    l->addWidget(m_pchkId = new QCheckBox(), i, 4);

    l->addWidget(new QLabel(tr("Date")), ++i, 0);
    l->addWidget(m_pDate1 = new QDateEdit(QDate::currentDate()), i, 1);
    l->addWidget(m_pDate2 = new QDateEdit(QDate::currentDate()), i, 2);
    l->addWidget(m_pchkDate = new QCheckBox(), i, 4);

    l->addWidget(new QLabel(tr("Document type")), ++i, 0);
    l->addWidget(m_peDocType = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pbDocType = newBtn(this), i, 3);
    l->addWidget(m_pchkDocType = new QCheckBox(), i, 4);
    l->addWidget(m_plDocType = new QLabel(), i, 5);
    connect(m_peDocType, SIGNAL(editingFinished()), SLOT(slotGetDocTypeByCode()));
    connect(m_pbDocType, SIGNAL(clicked()), SLOT(slotGetDocType()));

    l->addWidget(new QLabel(tr("Document state")), ++i, 0);
    l->addWidget(m_peDocState = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pbDocState = newBtn(this), i, 3);
    l->addWidget(m_pchkDocState = new QCheckBox(), i, 4);
    l->addWidget(m_plDocState = new QLabel(), i, 5);
    connect(m_peDocState, SIGNAL(editingFinished()), SLOT(slotGetDocStateByCode()));
    connect(m_pbDocState, SIGNAL(clicked()), SLOT(slotGetDocState()));

    l->addWidget(new QLabel(tr("Store, in")), ++i, 0);
    l->addWidget(m_peStoreIn = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pbStoreIn = newBtn(this), i, 3);
    l->addWidget(m_pchkStoreIn = new QCheckBox(), i, 4);
    l->addWidget(m_plStoreIn = new QLabel(), i, 5);
    connect(m_peStoreIn, SIGNAL(editingFinished()), SLOT(slotGetStoreInByCode()));
    connect(m_pbStoreIn, SIGNAL(clicked()), SLOT(slotGetStoreIn()));

    l->addWidget(new QLabel(tr("Store, out")), ++i, 0);
    l->addWidget(m_peStoreOut = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pbStoreOut = newBtn(this), i, 3);
    l->addWidget(m_pchkStoreOut = new QCheckBox(), i, 4);
    l->addWidget(m_plStoreOut = new QLabel(), i, 5);
    connect(m_peStoreOut, SIGNAL(editingFinished()), SLOT(slotGetStoreOutByCode()));
    connect(m_pbStoreOut, SIGNAL(clicked()), SLOT(slotGetStoreOut()));

    l->addWidget(new QLabel(tr("Goods")), ++i, 0);
    l->addWidget(m_peFood = new QLineEdit(), i, 1, 1, 2);
    l->addWidget(m_pbFood = newBtn(this), i, 3);
    l->addWidget(m_pchkFood = new QCheckBox(), i, 4);
    l->addWidget(m_plFood = new QLabel(), i, 5);
    connect(m_peFood, SIGNAL(editingFinished()), SLOT(slotGetFoodByCode()));
    connect(m_pbFood, SIGNAL(clicked()), SLOT(slotGetFood()));

    l->addWidget(m_pwidget, ++i, 0, 1, 6);
    setLayout(l);

}

bool FStoreDoc::checkData()
{
    return true;
}

void FStoreDoc::slotGetDocTypeByCode()
{
    m_plDocType->clear();
    m_db = new Database();
    EConnection::setConnectionParams(m_db->db, Data::databaseIndex);
    m_db->db.open();
    QSqlQuery *q = new QSqlQuery(m_db->db);
    q->exec(QString("SELECT NAME FROM STORE_ACTIONS WHERE ID IN (%1)").arg(m_peDocType->text()));
    while (q->next())
        m_plDocType->setText(m_plDocType->text() + "," + q->value(0).toString());
    if (m_plDocType->text().length() > 0)
        m_plDocType->setText(m_plDocType->text().remove(0, 1));
    delete q;
    delete m_db;
}

void FStoreDoc::slotGetDocType()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM STORE_ACTIONS ORDER BY NAME ", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peDocType->clear();
        m_plDocType->clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            m_peDocType->setText(m_peDocType->text() + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString());
            m_plDocType->setText(m_plDocType->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
        }
        if (m_peDocType->text().length() > 0)
        {
            m_plDocType->setText(m_plDocType->text().remove(0, 1));
            m_peDocType->setText(m_peDocType->text().remove(0, 1));
        }
    }
    delete c;
}

void FStoreDoc::slotGetDocStateByCode()
{
    m_plDocState->clear();
    m_db = new Database();
    EConnection::setConnectionParams(m_db->db, Data::databaseIndex);
    m_db->db.open();
    QSqlQuery *q = new QSqlQuery(m_db->db);
    q->exec(QString("SELECT NAME FROM DOC_STATE WHERE ID IN (%1)").arg(m_peDocState->text()));
    while (q->next())
        m_plDocState->setText(m_plDocState->text() + "," + q->value(0).toString());
    if (m_plDocState->text().length() > 0)
        m_plDocState->setText(m_plDocState->text().remove(0, 1));
    delete q;
    delete m_db;
}

void FStoreDoc::slotGetDocState()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM DOC_STATE ORDER BY NAME ", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peDocState->clear();
        m_plDocState->clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            m_peDocState->setText(m_peDocState->text() + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString());
            m_plDocState->setText(m_plDocState->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
        }
        if (m_peDocState->text().length() > 0)
        {
            m_plDocState->setText(m_plDocState->text().remove(0, 1));
            m_peDocState->setText(m_peDocState->text().remove(0, 1));
        }
    }
    delete c;
}

void FStoreDoc::slotGetStoreInByCode()
{
    m_plStoreIn->clear();
    m_db = new Database();
    EConnection::setConnectionParams(m_db->db, Data::databaseIndex);
    m_db->db.open();
    QSqlQuery *q = new QSqlQuery(m_db->db);
    q->exec(QString("SELECT NAME FROM STORAGES WHERE ID IN (%1)").arg(m_peStoreIn->text()));
    while (q->next())
        m_plStoreIn->setText(m_plStoreIn->text() + "," + q->value(0).toString());
    if (m_plStoreIn->text().length() > 0)
        m_plStoreIn->setText(m_plStoreIn->text().remove(0, 1));
    delete q;
    delete m_db;
}

void FStoreDoc::slotGetStoreIn()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME ", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peStoreIn->clear();
        m_plStoreIn->clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            m_peStoreIn->setText(m_peStoreIn->text() + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString());
            m_plStoreIn->setText(m_plStoreIn->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
        }
        if (m_peStoreIn->text().length() > 0)
        {
            m_peStoreIn->setText(m_peStoreIn->text().remove(0, 1));
            m_plStoreIn->setText(m_plStoreIn->text().remove(0, 1));
        }
    }
    delete c;
}

void FStoreDoc::slotGetStoreOutByCode()
{
    m_plStoreOut->clear();
    m_db = new Database();
    EConnection::setConnectionParams(m_db->db, Data::databaseIndex);
    m_db->db.open();
    QSqlQuery *q = new QSqlQuery(m_db->db);
    q->exec(QString("SELECT NAME FROM STORAGES WHERE ID IN (%1)").arg(m_peStoreOut->text()));
    while (q->next())
        m_plStoreOut->setText(m_plStoreOut->text() + "," + q->value(0).toString());
    if (m_plStoreOut->text().length() > 0)
        m_plStoreOut->setText(m_plStoreOut->text().remove(0, 1));
    delete q;
    delete m_db;
}

void FStoreDoc::slotGetStoreOut()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME ", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peStoreOut->clear();
        m_plStoreOut->clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            m_peStoreOut->setText(m_peStoreOut->text() + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString());
            m_plStoreOut->setText(m_plStoreOut->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
        }
        if (m_peStoreOut->text().length() > 0)
        {
            m_peStoreOut->setText(m_peStoreOut->text().remove(0, 1));
            m_plStoreOut->setText(m_plStoreOut->text().remove(0, 1));
        }
    }
    delete c;
}

void FStoreDoc::slotGetFoodByCode()
{    
    m_plFood->clear();
    m_db = new Database();
    EConnection::setConnectionParams(m_db->db, Data::databaseIndex);
    m_db->db.open();
    QSqlQuery *q = new QSqlQuery(m_db->db);
    q->exec(QString("SELECT NAME FROM GOODS WHERE ID IN (%1)").arg(m_peFood->text()));
    while (q->next())
        m_plFood->setText(m_plFood->text() + "," + q->value(0).toString());
    if (m_plFood->text().length() > 0)
        m_plFood->setText(m_plFood->text().remove(0, 1));
    delete q;
    delete m_db;
}

void FStoreDoc::slotGetFood()
{
    CheckList2 *c = new CheckList2(true, this);
    c->buildList("SELECT ID, NAME FROM GOODS ORDER BY NAME ", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_peFood->clear();
        m_plFood->clear();
        for (int i = 0; i < c->m_result.count(); i++)
        {
            m_peFood->setText(m_peFood->text() + "," + c->m_pt->m_model.data("ID", c->m_result.keys().at(i)).toString());
            m_plFood->setText(m_plFood->text() + "," + c->m_pt->m_model.data("NAME", c->m_result.keys().at(i)).toString());
        }
        if (m_peFood->text().length() > 0)
        {
            m_peFood->setText(m_peFood->text().remove(0, 1));
            m_plFood->setText(m_plFood->text().remove(0, 1));
        }
    }
    delete c;
}
