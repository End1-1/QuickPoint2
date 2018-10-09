#include "estoredoc.h"
#include <QKeyEvent>
#include <QtGui>
#include "tablemodel.h"
#include "checklist2.h"
#include "egoodsfordoc.h"
#include "global.h"
#include "../All/econnection.h"
#include <QPrinter>

EStoreDoc::EStoreDoc(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Store document"));

    EConnection::setConnectionParams(m_db.db, Data::databaseIndex);

    int i = -1;
    QGridLayout *l1 = new QGridLayout();
    QGridLayout *l2 = new QGridLayout();
    QHBoxLayout *l3 = new QHBoxLayout();
    QVBoxLayout *layoutMain = new QVBoxLayout();
    QWidget *w1 = new QWidget();
    QWidget *w2 = new QWidget();
    QWidget *w3 = new QWidget();

    //first page
    l1->addWidget(new QLabel(tr("Id")), ++i, 0);
    l1->addWidget(m_peId = new QLineEdit(), i, 1);
    m_peId->setEnabled(false);

    l1->addWidget(new QLabel(tr("State")), ++i, 0);
    l1->addWidget(m_peState = new QLineEdit(), i, 1);
    l1->addWidget(m_plState = new QLabel(), i, 3);
    m_peState->setEnabled(false);
    connect(m_peState, SIGNAL(textChanged(QString)), SLOT(slotDocStateChanged(QString)));

    l1->addWidget(new QLabel(tr("Date")), ++i, 0);
    l1->addWidget(m_pdDate = new QDateEdit(QDate::currentDate()), i, 1);
    connect(m_pdDate, SIGNAL(dateChanged(QDate)), SLOT(slotDateChanged(QDate)));

    QPushButton *pbAction = new QPushButton("...");
    pbAction->setMaximumSize(25, 25);
    l1->addWidget(new QLabel(tr("Action")), ++i, 0);
    l1->addWidget(m_peAction = new QLineEdit(), i, 1);
    l1->addWidget(pbAction, i, 2);
    l1->addWidget(m_plAction = new QLabel(), i, 3);
    connect(m_peAction, SIGNAL(editingFinished()), SLOT(slotActionGetName()));
    connect(pbAction, SIGNAL(clicked()), SLOT(slotAction()));

    QPushButton *pbStoreOut = new QPushButton("...");
    connect(pbStoreOut, SIGNAL(clicked()), SLOT(slotStoreOut()));
    pbStoreOut->setMaximumSize(25, 25);
    l1->addWidget(new QLabel(tr("Store out")), ++i, 0);
    l1->addWidget(m_peStoreOut = new QLineEdit(), i, 1);
    m_peStoreOut->setEnabled(false);
    connect(m_peStoreOut, SIGNAL(editingFinished()), SLOT(slotStoreOutGetName()));
    connect(m_peStoreOut, SIGNAL(textChanged(QString)), SLOT(slotStoreOutChanged(QString)));
    l1->addWidget(pbStoreOut, i, 2);
    l1->addWidget(m_plStoreOut = new QLabel(""), i, 3);

    l1->addWidget(new QLabel(tr("Store in")), ++i, 0);
    l1->addWidget(m_peStoreIn = new QLineEdit(), i, 1);
    connect(m_peStoreIn, SIGNAL(editingFinished()), SLOT(slotStoreInGetName()));
    connect(m_peStoreIn, SIGNAL(textChanged(QString)), SLOT(slotStoreInChanged(QString)));
    m_peStoreIn->setEnabled(false);
    QPushButton *pbStoreIn = new QPushButton("...");
    pbStoreIn->setMaximumSize(25, 25);
    connect(pbStoreIn, SIGNAL(clicked()), SLOT(slotStoreIn()));
    l1->addWidget(pbStoreIn, i, 2);
    l1->addWidget(m_plStoreIn = new QLabel(""), i, 3);

    l1->addWidget(new QLabel(tr("Operator")), ++i, 0);
    l1->addWidget(m_peOperator = new QLineEdit(), i, 1);
    m_peOperator->setEnabled(false);

    l1->addWidget(new QLabel(tr("Comment")), ++i, 0);
    l1->addWidget(m_peComment = new QLineEdit(), i, 1);
    connect(m_peComment, SIGNAL(textChanged(QString)), SLOT(slotCommentChanged(QString)));

    l1->setRowStretch(++i, 1);    
    w1->setLayout(l1);

    //second page
    i = -1;
    QPushButton *pbNew = new QPushButton(tr("New"));
    QPushButton *pbEdit = new QPushButton(tr("Edit"));
    QPushButton *pbDel = new QPushButton(tr("Delete"));
    connect(pbNew, SIGNAL(clicked()), SLOT(slotNew()));
    connect(pbEdit, SIGNAL(clicked()), SLOT(slotEdit()));
    connect(pbDel, SIGNAL(clicked()), SLOT(slotDelete()));

    m_ptGoods = new TableView();

    m_ptGoods->setMinimumSize(700, 400);
    EConnection::setConnectionParams(m_ptGoods->m_db.db, Data::databaseIndex);
    m_ptGoods->m_model.insertColumns(0, 7, QModelIndex());
    m_ptGoods->setColumn(0, "ID", tr("Goods id"), 0, DATA_INTEGER);
    m_ptGoods->setColumn(1, "NAME", tr("Goods"), 250, DATA_STRING);
    m_ptGoods->setColumn(2, "QTY", tr("Quantity"), 100, DATA_DOUBLE, 3);
    m_ptGoods->setColumn(3, "UNIT", tr("Unit"), 100, DATA_STRING);
    m_ptGoods->setColumn(4, "UNIT_ID", tr("Unit id"), 0, DATA_INTEGER);
    m_ptGoods->setColumn(5, "PRICE", tr("Price"), 150, DATA_DOUBLE, 2);
    m_ptGoods->setColumn(6, "AMOUNT", tr("Amount"), 150, DATA_DOUBLE, 2);


    l2->addWidget(pbNew, ++i, 0);
    l2->addWidget(pbEdit, i, 1);
    l2->addWidget(pbDel, i, 2);
    l2->setColumnMinimumWidth(4, 500);
    l2->addWidget(m_ptGoods, ++i, 0, 1, 6);
    l2->addWidget(new QLabel(tr("Amount")), ++i, 0, 1, 4, Qt::AlignRight);
    l2->addWidget(m_peAmount = new QLineEdit(), i, 5, Qt::AlignLeft);
    m_peAmount->setEnabled(false);
    m_peAmount->setMaximumWidth(150);

    w2->setLayout(l2);

    //Action buttons
    QPushButton *pbWrite = new QPushButton(tr("Write"), this);
    QPushButton *pbDraft = new QPushButton(tr("Draft"), this);
    QPushButton *pbPrint = new QPushButton(tr("Print"), this);
    QPushButton *pbClose = new QPushButton(tr("Close"), this);
    connect(pbWrite, SIGNAL(clicked()), SLOT(slotWrite()));
    connect(pbDraft, SIGNAL(clicked()), SLOT(slotDraft()));
    connect(pbPrint, SIGNAL(clicked()), SLOT(slotPrint()));
    connect(pbClose, SIGNAL(clicked()), SLOT(slotClose()));

    l3->addStretch(1);
    l3->addWidget(pbWrite);
    l3->addWidget(pbDraft);
    l3->addWidget(pbPrint);
    l3->addWidget(pbClose);
    l3->addStretch(1);
    w3->setLayout(l3);

    //Tabpage control
    QTabWidget *t = new QTabWidget();
    t->addTab(w1, tr("Main"));
    t->addTab(w2, tr("Goods"));

    layoutMain->addWidget(t);
    layoutMain->addWidget(w3);
    m_peState->setText(QString::number(STATE_DRAFT));
    setLayout(layoutMain);
    docSaved = true;
    m_peOperator->setText(M::user.Name);
}

EStoreDoc::~EStoreDoc()
{
    delete m_peId;
    delete m_peState;
    delete m_plState;
    delete m_pdDate;
    delete m_peAction;
    delete m_plAction;
    delete m_peStoreIn;
    delete m_plStoreIn;
    delete m_peStoreOut;
    delete m_plStoreOut;
    delete m_peOperator;
    delete m_peComment;
    delete m_peAmount;
    delete m_ptGoods;
}

void EStoreDoc::getActionName()
{
    QSqlQuery *q = m_ptGoods->openDB();
    q->prepare("SELECT NAME FROM STORE_ACTIONS WHERE ID=:ID");
    q->bindValue("ID", m_peAction->text());
    q->exec();
    q->next();
    m_plAction->setText(q->value(0).toString());
    m_ptGoods->closeDB(q);
}

void EStoreDoc::getStoreOutName()
{
    QSqlQuery *q = m_ptGoods->openDB();
    q->prepare("SELECT NAME FROM STORAGE WHERE ID=:ID");
    q->bindValue("ID", m_peStoreOut->text());
    q->exec();
    q->next();
    m_plStoreOut->setText(q->value(0).toString());
    m_ptGoods->closeDB(q);
}

void EStoreDoc::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
        return;
    default:
        QDialog::keyPressEvent(e);
        break;
    }
}

void EStoreDoc::setData(EGoodsForDoc *e, const int &row)
{
    m_ptGoods->m_model.setData(row, "ID", e->m_peFoodId->text());
    m_ptGoods->m_model.setData(row, "NAME", e->m_peFoodName->text());
    m_ptGoods->m_model.setData(row, "QTY", e->m_peQty->text());
    m_ptGoods->m_model.setData(row, "UNIT", e->m_peUnit->text());
    m_ptGoods->m_model.setData(row, "UNIT_ID", e->m_unit);
    m_ptGoods->m_model.setData(row, "PRICE", e->m_pePrice->text());
    m_ptGoods->m_model.setData(row, "AMOUNT", e->m_peAmount->text());
    countAmount();
}

void EStoreDoc::countAmount()
{
    double amount = 0;
    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
        amount += m_ptGoods->m_model.Double("AMOUNT", i);
    m_peAmount->setText(QString::number(amount, 'f', 2));
}

bool EStoreDoc::checkData()
{
    QString msg = "";
    bool result = true;

    if (m_plAction->text().length() == 0)
    {
        msg += tr("The action must be specified\n");
        result = false;
    }
    else
    {
        switch (m_peAction->text().toInt())
        {
        case STORE_INPUT:
        case STORE_INVENTORY:
        {
            if (m_plStoreIn->text().length() == 0)
            {
                msg += tr("The store in must be specified\n");
                result = false;
            }
            break;
        }
        case STORE_OUTPUT:
        case STORE_SALE:
        {
            if (m_plStoreOut->text().length() == 0)
            {
                msg += "The store out must be specified\n";
                result = false;
            }
            break;
        }
        case STORE_MOVEMENT:
        {
            if (m_plStoreIn->text().length() == 0)
            {
                msg += "The store in must be specified\n";
                result = false;
            }
            if (m_plStoreOut->text().length() == 0)
            {
                msg += "The store out must be specified\n";
                result = false;
            }
            if (m_plStoreIn->text().length() > 0 && m_plStoreIn->text() == m_plStoreOut->text())
            {
                msg += "The both stores are identical\n";
                result = false;
            }
            break;
        }
        default:
            msg += "The action must be specified\n";
            result = false;
            break;
        }
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EStoreDoc::enableStoreInputs()
{
    m_peStoreIn->setEnabled(m_peAction->text().toInt() == STORE_INPUT
                            || m_peAction->text().toInt() == STORE_MOVEMENT
                            || m_peAction->text().toInt() == STORE_INVENTORY);
    m_peStoreOut->setEnabled(m_peAction->text().toInt() == STORE_OUTPUT
                             || m_peAction->text().toInt() == STORE_SALE
                             || m_peAction->text().toInt() == STORE_MOVEMENT);
}

bool EStoreDoc::writeDraft()
{
    QSqlQuery q(m_db.db);
    if (m_peId->text().length() == 0)
    {
        q.prepare("SELECT GEN_ID(GEN_STORE_DOC_ID, 1) FROM RDB$DATABASE");
        q.exec();
        q.next();
        m_peId->setText(q.value(0).toString());
    }
    else
    {
        QStringList docs;
        q.prepare("SELECT DISTINCT(DOC_ID) "
                  "FROM STORE_GOODS "
                  "WHERE RECORD_GUID IN (SELECT RECORD_GUID FROM STORE_GOODS WHERE DOC_ID=:DOC_ID1 AND QTY>0) "
                  "AND DOC_ID <> :DOC_ID2 AND QTY<0");
        q.bindValue("DOC_ID1", m_peId->text());
        q.bindValue("DOC_ID2", m_peId->text());
        q.exec();
        while (q.next())
            docs.push_back(q.value(0).toString());
        if (docs.count())
        {
            QString docs_str;
            for (int i = 0; i < docs.count(); i++)
            {
                docs_str += docs[i] + ", ";
                if ((i > 0) && (!(i % 5)))
                    docs_str += "\n";
            }
            docs_str = docs_str.remove(docs_str.length() - 2, 2) + ".";
            QMessageBox::critical(this, tr("Error"), tr("The next documents have depending from this document\n\n") + docs_str);
            return false;
        }

        q.prepare("DELETE FROM STORE_DOC WHERE ID=:ID");
        q.bindValue("ID", m_peId->text().toInt());
        q.exec();
        q.prepare("DELETE FROM STORE_DRAFT WHERE DOC_ID=:DOC_ID");
        q.bindValue("DOC_ID", m_peId->text().toInt());
        q.exec();
        q.prepare("DELETE FROM STORE_GOODS WHERE DOC_ID=:DOC_ID");
        q.bindValue("DOC_ID", m_peId->text().toInt());
        q.exec();
    }

    q.prepare("INSERT INTO STORE_DOC (ID, DOC_TYPE, DOC_STATE, DOC_DATE, STORE_IN_ID, STORE_OUT_ID, OPERATOR, COMMENT) "
               "VALUES (:ID, :DOC_TYPE, :DOC_STATE, :DOC_DATE, :STORE_IN_ID, :STORE_OUT_ID, :OPERATOR, :COMMENT)");
    q.bindValue("ID", m_peId->text());
    q.bindValue("DOC_TYPE", m_peAction->text());
    q.bindValue("DOC_STATE", m_peState->text());
    q.bindValue("DOC_DATE", m_pdDate->date());
    q.bindValue("STORE_IN_ID", m_peStoreIn->text().toInt());
    q.bindValue("STORE_OUT_ID", m_peStoreOut->text().toInt());
    q.bindValue("OPERATOR", M::user.Id);
    q.bindValue("COMMENT", m_peComment->text());
    q.exec();

    m_peState->setText("0");

    return true;
}

void EStoreDoc::writeGoodsDraft()
{
    QSqlQuery q(m_db.db);
    q.prepare("INSERT INTO STORE_DRAFT (DOC_ID, GOODS_ID, QTY, PRICE) VALUES (:DOC_ID, :GOODS_ID, :QTY, :PRICE)");
    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
    {
        q.bindValue("DOC_ID", m_peId->text());
        q.bindValue("GOODS_ID", m_ptGoods->m_model.Integer("GOODS_ID", i));
        q.bindValue("QTY", m_ptGoods->m_model.Double("QTY", i));
        q.bindValue("PRICE", m_ptGoods->m_model.Double("PRICE", i));
        q.exec();
    }
}

bool EStoreDoc::writeGoodsOutput()
{
    //Get goods list in store
    QString goods_code;
    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
    {
        goods_code += m_ptGoods->m_model.String("GOODS_ID", i) + ",";
    }
    if (goods_code.length())
        goods_code = goods_code.remove(goods_code.length() - 1, 1);

    QString sql = "SELECT SG.GOODS_ID, SG.RECORD_GUID, SG.PRICE, SG.SORT_DATE, SUM(SG.QTY)  "
            "FROM STORE_GOODS SG "
            "WHERE SG.STORE_ID=:STORE_ID "
            "AND SORT_DATE<=:SORT_DATE "
            "GROUP BY 1, 2, 3, 4 "
            "HAVING SUM(SG.QTY) > 0 "
            "ORDER BY SG.SORT_DATE ";

    QMap<int, QVector<OutputGoods> > goods;
    QSqlQuery q(m_db.db);
    q.prepare(sql);
    q.bindValue("STORE_ID", m_peStoreOut->text().toInt());
    q.bindValue("SORT_DATE", m_pdDate->date());
    q.exec();
    while (q.next())
    {
        if (!goods.contains(q.value(0).toInt()))
            goods.insert(q.value(0).toInt(), QVector<OutputGoods>());

        OutputGoods o;
        o.goods = q.value(0).toInt();
        o.guid = q.value(1).toInt();
        o.price = q.value(2).toDouble();
        o.date = q.value(3).toDate();
        o.qty = q.value(4).toDouble();

        goods[o.goods].push_back(o);
    }

    //make goods list for output
    QString incompleteGoods;
    QVector<OutputGoods> goods_output;
    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
    {
        double qty_left = m_ptGoods->m_model.Double("QTY", i);
        m_ptGoods->m_model.setData(i, "AMOUNT", 0);
        QVector<OutputGoods> &g = goods[m_ptGoods->m_model.Integer("GOODS_ID", i)];

        while ((qty_left > 0) && (g.count()))
        {
            OutputGoods &oStore = g[0], oCurr;
            oCurr.goods = m_ptGoods->m_model.Integer("GOODS_ID", i);
            oCurr.guid = oStore.guid;
            oCurr.date = oStore.date;

            if (qty_left <= oStore.qty)
            {
                oCurr.qty = qty_left;
                oCurr.price = oStore.price;
                qty_left = 0;
                oStore.qty -= qty_left;
            }
            else
            {
                oCurr.qty = oStore.qty;
                oCurr.price = oStore.price;
                qty_left -= oStore.qty;
                oStore.qty = 0;
            }

            m_ptGoods->m_model.setData(i, "AMOUNT", (oCurr.qty * oCurr.price) + m_ptGoods->m_model.Double("AMOUNT", i));

            goods_output.push_back(oCurr);
            if (!oStore.qty)
                g.remove(0);
        }

        m_ptGoods->m_model.setData(i, "PRICE", m_ptGoods->m_model.Double("AMOUNT", i) / m_ptGoods->m_model.Double("QTY", i));
        if (qty_left > 0)
            incompleteGoods += QString("%1: %2\n").arg(m_ptGoods->m_model.String("NAME", i)).arg(qty_left);
    }

    //finally, insert into table or quit, because not enough quantity in the store
    if (incompleteGoods.length())
    {
        QMessageBox::critical(this, tr("Error"), QString("%1\n%2").arg(tr("The goods in the store not enough")).arg(incompleteGoods));
        return false;
    }

    q.prepare("INSERT INTO STORE_GOODS (DOC_ID, STORE_ID, GOODS_ID, QTY, PRICE, RECORD_GUID, SORT_DATE) VALUES (:DOC_ID, :STORE_ID, :GOODS_ID, :QTY, :PRICE, :RECORD_GUID, :SORT_DATE)");
    for (QVector<OutputGoods>::const_iterator i = goods_output.begin(); i != goods_output.end(); i++)
    {
        q.bindValue("DOC_ID", m_peId->text());
        q.bindValue("STORE_ID", m_peStoreOut->text());
        q.bindValue("GOODS_ID", i->goods);
        q.bindValue("QTY", i->qty * -1);
        q.bindValue("PRICE", i->price);
        q.bindValue("RECORD_GUID", i->guid);
        q.bindValue("SORT_DATE", i->date);
        q.exec();
    }
    return true;
}

void EStoreDoc::writeDocIn()
{
    QSqlQuery q(m_db.db);
    q.prepare("INSERT INTO STORE_GOODS(DOC_ID, STORE_ID, GOODS_ID, QTY, PRICE, SORT_DATE) VALUES (:DOC_ID, :STORE_ID, :GOODS_ID, :QTY, :PRICE, :SORT_DATE)");
    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
    {
        q.bindValue("DOC_ID", m_peId->text());
        q.bindValue("STORE_ID", m_peStoreIn->text());
        q.bindValue("GOODS_ID", m_ptGoods->m_model.data("GOODS_ID", i).toString());
        q.bindValue("QTY", m_ptGoods->m_model.Double("QTY", i));
        q.bindValue("PRICE", m_ptGoods->m_model.Double("PRICE", i));
        q.bindValue("SORT_DATE", m_pdDate->date());
        q.exec();
    }
}

void EStoreDoc::loadDoc()
{
    QSqlQuery *q = m_ptGoods->openDB();

    if (!q->prepare("SELECT SD.DOC_DATE, SD.DOC_STATE, DS.NAME AS DOCSTATENAME, "
               "SD.STORE_IN_ID, STIN.NAME AS SINNAME, "
               "SD.STORE_OUT_ID, SOUT.NAME AS SOUNAME, SD.OPERATOR, "
               "E.NAME AS ENAME, SD.COMMENT, SD.DOC_TYPE, "
               "SA.NAME AS SANAME "
               "FROM STORE_DOC SD "
               "LEFT JOIN DOC_STATE DS ON SD.DOC_STATE=DS.ID "
               "LEFT JOIN STORAGE STIN ON SD.STORE_IN_ID=STIN.ID "
               "LEFT JOIN STORAGE SOUT ON SD.STORE_OUT_ID=SOUT.ID "
               "LEFT JOIN EMPLOYEES E ON SD.OPERATOR=E.ID "
               "LEFT JOIN STORE_ACTIONS SA ON SD.DOC_TYPE=SA.ID "
               "WHERE SD.ID=:ID"))
        QMessageBox::critical(this, tr("SQL error"), q->lastError().databaseText());

    q->bindValue("ID", m_peId->text());
    q->exec();
    if (q->next())
    {
        m_pdDate->setDate(q->value(0).toDate());
        m_peState->setText(q->value(1).toString());
        m_plState->setText(q->value(2).toString());
        m_peStoreIn->setText(q->value(3).toString());
        m_plStoreIn->setText(q->value(4).toString());
        m_peStoreOut->setText(q->value(5).toString());
        m_plStoreOut->setText(q->value(6).toString());
        m_peOperator->setText(M::user.Name);
        m_peComment->setText(q->value(9).toString());
        m_peAction->setText(q->value(10).toString());
        m_plAction->setText(q->value(11).toString());
    }

    m_db.close(q);
    q = m_ptGoods->openDB();
    if (!q->prepare("SELECT SD.GOODS_ID, G.NAME AS GNAME, SD.QTY, U.NAME AS UNAME, U.ID, SD.PRICE "
                    "FROM STORE_DRAFT SD "
                    "LEFT JOIN GOODS G ON SD.GOODS_ID=G.ID "
                    "LEFT JOIN UNITS U ON G.UNIT_ID=U.ID "
                    "WHERE SD.DOC_ID=:DOC_ID "))
        QMessageBox::critical(this, tr("SQL error"), q->lastError().databaseText());

    q->bindValue("DOC_ID", m_peId->text());
    q->exec();
    while (q->next())
    {
        int row = m_ptGoods->addRow();
        m_ptGoods->m_model.setData(row, "ID", q->value(0).toString());
        m_ptGoods->m_model.setData(row, "NAME", q->value(1).toString());
        m_ptGoods->m_model.setData(row, "QTY", q->value(2).toString());
        m_ptGoods->m_model.setData(row, "UNIT", q->value(3).toString());
        m_ptGoods->m_model.setData(row, "UNIT_ID", q->value(4).toString());
        m_ptGoods->m_model.setData(row, "PRICE", q->value(5).toString());
        m_ptGoods->m_model.setData(row, "AMOUNT", q->value(2).toDouble() * q->value(5).toDouble());
    }

    countAmount();
    m_ptGoods->closeDB(q);
    docSaved = true;
}

void EStoreDoc::slotAction()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT ID, NAME FROM STORE_ACTIONS ORDER BY NAME",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_plAction->clear();
        m_peAction->clear();
        if (c->m_result.count() > 0)
        {
            m_plAction->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
            m_peAction->setText(c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString());
        }
    }
    delete c;
    enableStoreInputs();
}

void EStoreDoc::slotStoreIn()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_plStoreIn->clear();
        m_peStoreIn->clear();
        docSaved = false;
        if (c->m_result.count() > 0)
        {
            m_plStoreIn->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
            m_peStoreIn->setText(c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString());
        }
        enableStoreInputs();
    }
    delete c;
}

void EStoreDoc::slotStoreOut()
{
    CheckList2 *c = new CheckList2(false, this);
    c->buildList("SELECT ID, NAME FROM STORAGE ORDER BY NAME",
                "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        m_plStoreOut->clear();
        m_peStoreOut->clear();
        docSaved = false;
        if (c->m_result.count() > 0)
        {
            m_plStoreOut->setText(c->m_pt->m_model.data("NAME", c->m_result.keys().at(0)).toString());
            m_peStoreOut->setText(c->m_pt->m_model.data("ID", c->m_result.keys().at(0)).toString());
        }
        enableStoreInputs();
    }
    delete c;
}

void EStoreDoc::slotStoreInGetName()
{
    QSqlQuery *q = m_ptGoods->openDB();
    m_plStoreIn->clear();
    if (q->prepare("SELECT NAME FROM STORAGE WHERE ID=:ID"))
    {
        q->bindValue("ID", m_peStoreIn->text());
        q->exec();
        if (q->next())
            m_plStoreIn->setText(q->value(0).toString());
    }
    delete q;
    docSaved = false;
}

void EStoreDoc::slotStoreOutGetName()
{
    QSqlQuery *q = m_ptGoods->openDB();
    m_plStoreOut->clear();
    if (q->prepare("SELECT NAME FROM STORAGE WHERE ID=:ID"))
    {
        q->bindValue("ID", m_peStoreOut->text());
        q->exec();
        if (q->next())
            m_plStoreOut->setText(q->value(0).toString());
    }
    m_ptGoods->closeDB(q);
    docSaved = false;
}

void EStoreDoc::slotActionGetName()
{
    QSqlQuery *q = m_ptGoods->openDB();
    m_plAction->clear();
    if (q->prepare("SELECT NAME FROM STORE_ACTIONS WHERE ID=:ID"))
    {
        q->bindValue("ID", m_peAction->text());
        q->exec();
        if (q->next())
            m_plAction->setText(q->value(0).toString());
    }
    m_ptGoods->closeDB(q);
    enableStoreInputs();
    docSaved = false;
}

void EStoreDoc::slotNew()
{
    bool e_result = false;
    do
    {
        EGoodsForDoc *e = new EGoodsForDoc(this);
        if (e->exec() == QDialog::Accepted)
        {
            int row = m_ptGoods->addRow();
            setData(e, row);
            docSaved = false;
        }
        e_result = e->next;
        delete e;
    }
    while (e_result);

    countAmount();
}

void EStoreDoc::slotEdit()
{
    QModelIndexList list = m_ptGoods->selectionModel()->selectedRows();
    if (list.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    if (list.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    EGoodsForDoc *e = new EGoodsForDoc(this);
    e->m_peFoodId->setText(m_ptGoods->m_model.data("ID", list[0].row()).toString());
    e->m_peFoodName->setText(m_ptGoods->m_model.data("NAME", list[0].row()).toString());
    e->m_peQty->setText(m_ptGoods->m_model.data("QTY", list[0].row()).toString());
    e->m_peUnit->setText(m_ptGoods->m_model.data("UNAME", list[0].row()).toString());
    e->m_unit = m_ptGoods->m_model.data("UNIT_ID", list[0].row()).toString();
    e->m_pePrice->setText(m_ptGoods->m_model.data("PRICE", list[0].row()).toString());
    e->m_peAmount->setText(m_ptGoods->m_model.data("AMOUNT", list[0].row()).toString());
    if (e->exec() == QDialog::Accepted)
    {
        setData(e, list[0].row());
        docSaved = false;
    }
    delete e;

    countAmount();
}

void EStoreDoc::slotDelete()
{
    QModelIndexList list = m_ptGoods->selectionModel()->selectedRows();
    if (list.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    for (int i = list.count() - 1; i > -1; i--)
        m_ptGoods->m_model.removeRows(list[i].row(), 1, QModelIndex());

    docSaved = false;
    countAmount();
}

void EStoreDoc::slotWrite()
{
    if (!checkData())
        return;

    bool result = startDoc();
    QSqlQuery q(m_db.db);
    result = q.exec("SELECT * FROM STORE_DOC WITH LOCK");
    result = q.next();

    result = writeDraft();

    if (!result)
    {
        endDoc();
        return;
    }

    switch(m_peAction->text().toInt())
    {
    case STORE_INVENTORY:
        break;
    case STORE_INPUT:
        writeDocIn();
        break;
    case STORE_OUTPUT:
    case STORE_SALE:
        result = writeGoodsOutput();
        break;
    case STORE_MOVEMENT:
        result = writeGoodsOutput();
        if (result)
            writeDocIn();
        break;
    }

    if (result)
        writeGoodsDraft();

    if (result)
    {
        docSaved = true;
        m_peState->setText("1"); //State writed
        if (!q.prepare("UPDATE STORE_DOC SET DOC_STATE=1 WHERE ID=:ID"))
            QMessageBox::critical(this, "SQL error", q.lastError().databaseText());
        q.bindValue("ID", m_peId->text());
        if (!q.exec())
            QMessageBox::critical(this, "SQL error", q.lastError().databaseText());

        QMessageBox::information(this, tr("Information"), tr("Writed"));
    }

    endDoc();
}

void EStoreDoc::slotDraft()
{
    bool result = startDoc();
    result = writeDraft();

    if (result)
    {
        writeGoodsDraft();

        docSaved = true;
        m_peState->setText("0"); //State draft
        QSqlQuery q(m_db.db);
        q.prepare("UPDATE STORE_DOC SET DOC_STATE=0 WHERE ID=:ID");
        q.bindValue("ID", m_peId->text());
        q.exec();
        QMessageBox::information(this, tr("Information"), tr("Writed"));
    }
    endDoc();
}

void EStoreDoc::slotPrint()
{
    int page_num = 1, row_num = 1, rows_on_page = 22;
    int top = 35;
    QVector<int> table_points;
    QVector<QString> table_values;
    QFont font(qApp->font());
    font.setPointSize(11);

    table_points << 5 << 15 << 35 << 115 << 130 << 150 << 170 << 200;


    QPrintDialog *pd = new QPrintDialog(this);
    if (pd->exec() != QDialog::Accepted)
    {
        delete pd;
        return;
    }
    QString prn_name = pd->printer()->printerName();
    delete pd;
    QPrint printer(prn_name);
    printer.cmdSetFont(font);

    for (int i = 0; i < m_ptGoods->m_model.rowCount(QModelIndex()); i++)
    {
        if (!(i % rows_on_page))
        {
            if (page_num > 1)
                printer.cmdNewPage();

            printMain(printer, page_num);
            table_values.clear();
            table_values << "NN" << tr("Goods code") << tr("Goods name") << tr("Measurment")
                         << tr("Price") << tr("Quantity") << tr("Amount") << "";
            top = 35;
            printRow(printer, table_points, table_values, top, 11);
        }

        table_values.clear();
        table_values << QString::number(row_num++)
                     << m_ptGoods->m_model.String("ID", i)
                     << m_ptGoods->m_model.String("NAME", i)
                     << m_ptGoods->m_model.String("UNIT_NAME", i)
                     << QString::number(m_ptGoods->m_model.Double("PRICE", i))
                     << QString::number(m_ptGoods->m_model.Double("QTY", i))
                     << QString::number(m_ptGoods->m_model.Double("AMOUNT", i))
                     << "";
        printRow(printer, table_points, table_values, top, 7);
    }
    printer.beginPrint();
}

void EStoreDoc::slotClose()
{
    if (!docSaved)
        if (QMessageBox::warning(this, tr("Warning"), tr("Document was not saved, continue?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
    close();
}

void EStoreDoc::slotDocStateChanged(const QString &value)
{
    QSqlQuery *q = m_ptGoods->openDB();
    if (q->prepare("SELECT NAME FROM DOC_STATE WHERE ID=:ID"))
    {
        q->bindValue("ID", value);
        q->exec();
        if (q->next())
            m_plState->setText(q->value(0).toString());
    }
    m_ptGoods->closeDB(q);
}

void EStoreDoc::slotDateChanged(const QDate &value)
{
    Q_UNUSED(value);
    docSaved = false;
}

void EStoreDoc::slotStoreInChanged(const QString &value)
{
    Q_UNUSED(value);
    docSaved = false;
}

void EStoreDoc::slotStoreOutChanged(const QString &value)
{
    Q_UNUSED(value);
    docSaved = false;
}

void EStoreDoc::slotCommentChanged(const QString &value)
{
    Q_UNUSED(value);
    docSaved = false;
}


bool EStoreDoc::startDoc()
{
    m_db.db.open();
    return m_db.db.driver()->beginTransaction();
}

void EStoreDoc::endDoc()
{
    m_db.db.driver()->commitTransaction();
    m_db.db.close();
}

void EStoreDoc::printMain(QPrint &printer, int &page_num)
{
    QFont f(qApp->font()), old_font = printer.m_font;
    int top = 5, left = 5, left2 = 50;
    printer.cmdSetFont(f);
    printer.addCommand(new QPCText(QString("%1 %2 #%3, %4 %5").arg(tr("Store's")).arg(m_plAction->text()).arg(m_peId->text()).arg(tr("Page")).arg(page_num++), QPoint(left, top)));
    printer.addCommand(new QPCText(tr("State") + tr(":") + m_plState->text(), QPoint(150, top)));
    top += printer.incTop();
    printer.addCommand(new QPCText(tr("Date") + tr(":"), QPoint(left, top)));
    printer.addCommand(new QPCText(m_pdDate->date().toString(Data::settings[SETTINGS_DATE_FORMAT].toString()), QPoint(left + 60, top)));
    top += printer.incTop();

    switch(m_peAction->text().toInt())
    {
    case STORE_OUTPUT:
    case STORE_SALE:
        printer.addCommand(new QPCText(tr("Output store") + tr(":"), QPoint(left, top)));
        printer.addCommand(new QPCText(m_plStoreOut->text(), QPoint(left + left2, top)));
        top += printer.incTop();
        break;
    case STORE_INPUT:
    case STORE_INVENTORY:
        printer.addCommand(new QPCText(tr("Input store") + tr(":"), QPoint(left, top)));
        printer.addCommand(new QPCText(m_plStoreIn->text(), QPoint(left + left2, top)));
        top += printer.incTop();
        break;
    case STORE_MOVEMENT:
        printer.addCommand(new QPCText(tr("Ouput store") + tr(":"), QPoint(left, top)));
        printer.addCommand(new QPCText(m_plStoreOut->text(), QPoint(left + left2, top)));
        top += printer.incTop();
        printer.addCommand(new QPCText(tr("Input store") + tr(":"), QPoint(left, top)));
        printer.addCommand(new QPCText(m_plStoreIn->text(), QPoint(left + left2, top)));
        top += printer.incTop();
        break;
    }

    printer.addCommand(new QPCText(tr("Note") + tr(":"), QPoint(left, top)));
    printer.addCommand(new QPCText(m_peComment->text(), QPoint(left + left2, top)));
    top += printer.incTop();

    top = 250;
    f.setPointSize(10);
    printer.addCommand(new QPCSetFont(f));
    printer.addCommand(new QPCText(tr("Operator") + tr(":"), QPoint(left, top)));
    printer.addCommand(new QPCText(tr("Bailee") + tr(":"), QPoint(120, top)));
    top += printer.incTop();
    printer.addCommand(new QPCText(M::user.Name, QPoint(left, top)));
    top += printer.incTop();
    top += printer.incTop();
    printer.addCommand(new QPCLine(QPoint(left, top), QPoint(left + left2, top)));
    printer.addCommand(new QPCLine(QPoint(120, top), QPoint(200, top)));
    top += printer.incTop();
    printer.addCommand(new QPCText(tr("Signature"), QPoint(left + 5, top)));
    printer.addCommand(new QPCText(tr("First name, last name, signature"), QPoint(125, top)));
    top += (printer.incTop() * 2);

    printer.cmdSetFont(old_font);
}

void EStoreDoc::printRow(QPrint &printer, QVector<int> &points, QVector<QString> &values, int &top, int row_height)
{
    printer.addCommand(new QPCLine(QPoint(points.at(0), top), QPoint(points.last(), top)));
    printer.addCommand(new QPCLine(QPoint(points.at(0), top + row_height), QPoint(points.last(), top + row_height)));
    QTextOption to;
    to.setWrapMode(QTextOption::WordWrap);
    for (int i = 0; i < points.count(); i++)
    {
        printer.addCommand(new QPCLine(QPoint(points.at(i), top), QPoint(points.at(i), top + row_height)));
        QRect rect;
        rect.setLeft(points[i] + 1);
        rect.setRight(i == points.count() - 1 ? 0 : points[i + 1] - 1);
        rect.setTop(top + 1);
        rect.setBottom(top + row_height - 1);
        printer.cmdTextRect(values.at(i), rect, to);
    }

    top += row_height;
}
