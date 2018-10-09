#include "qmaindialog.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QLineEdit>
#include "qstoremap.h"
#include <QtSql>
#include "qaboutdialog.h"

QMainDialog::QMainDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Synchronization"));

    QVBoxLayout *l1 = new QVBoxLayout(this);
    m_pDbList = new QListWidget(this);
    m_pDbList->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_pDbList->addAction(m_pNew = new QAction(tr("New"), this));
    m_pDbList->addAction(m_pEdit = new QAction(tr("Edit"), this));
    m_pDbList->addAction(m_pDelete = new QAction(tr("Delete"), this));
    m_pDbList->setMinimumSize(400, 400);
    l1->addWidget(m_pDbList);
    connect(m_pNew, SIGNAL(triggered()), this, SLOT(newDbPair()));
    connect(m_pEdit, SIGNAL(triggered()), this, SLOT(editDbPair()));
    connect(m_pDelete, SIGNAL(triggered()), this, SLOT(deleteDbPair()));
    QWidget *w1 = new QWidget(this);
    w1->setLayout(l1);

    QVBoxLayout *l2 = new QVBoxLayout(this);
    m_pStoreMap = new QListWidget(this);
    m_pStoreMap->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_pStoreMap->addAction(m_pStoreNew = new QAction(tr("New"), this));
    m_pStoreMap->addAction(m_pStoreEdit = new QAction(tr("Edit"), this));
    m_pStoreMap->addAction(m_pStoreDelete = new QAction(tr("Delete"), this));
    l2->addWidget(m_pStoreMap);
    connect(m_pStoreNew, SIGNAL(triggered()), SLOT(newStoreMap()));
    connect(m_pStoreEdit, SIGNAL(triggered()), SLOT(editStoreMap()));
    connect(m_pStoreDelete, SIGNAL(triggered()), SLOT(deleteStoreMap()));
    QWidget *w2 = new QWidget(this);
    w2->setLayout(l2);

    QWidget *w3 = new QWidget(this);
    QVBoxLayout *l3 = new QVBoxLayout(this);
    m_pFoodMap = new QListWidget(this);
    m_pFoodMap->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_pFoodMap->addAction(m_pFoodNewMap = new QAction(tr("New"), this));
    m_pFoodMap->addAction(m_pFoodDeleteMap = new QAction(tr("Delete"), this));
    m_pFoodMap->addAction(m_pFoodLoadFromFile = new QAction(tr("Load from file"), 0));
    m_pFoodMap->addAction(m_pFoodExportToFile = new QAction(tr("Export to file"), 0));
    connect(m_pFoodNewMap, SIGNAL(triggered()), SLOT(newFoodMap()));
    connect(m_pFoodDeleteMap, SIGNAL(triggered()), SLOT(deleteFoodMap()));
    connect(m_pFoodLoadFromFile, SIGNAL(triggered()), SLOT(loadFoodMapFromFile()));
    connect(m_pFoodExportToFile, SIGNAL(triggered()), SLOT(exportFoodMapToFile()));
    l3->addWidget(m_pFoodMap);
    w3->setLayout(l3);

    int i = -1;
    QWidget *wdtAction = new QWidget(this);
    QGridLayout *layoutAction = new QGridLayout(this);
    QPushButton *pbGo = new QPushButton(tr("Go"), this);
    QPushButton *pbAbout = new QPushButton(tr("About"), this);
    m_pDate1 = new QDateEdit(QDate::currentDate().addDays((QDate::currentDate().day() - 1) * -1), this);
    m_pDate2 = new QDateEdit(QDate::currentDate(), this);

    connect(pbGo, SIGNAL(clicked()), SLOT(go()));
    connect(pbAbout, SIGNAL(clicked()), SLOT(about()));

    layoutAction->addWidget(new QLabel(tr("Date range"), this), ++i, 0);
    layoutAction->addWidget(m_pDate1, i, 1);
    layoutAction->addWidget(m_pDate2, i, 2);
    layoutAction->addWidget(pbGo, ++i, 0, 1, 3);
    layoutAction->addWidget(pbAbout, ++i, 0, 1, 3);
    layoutAction->setRowStretch(++i, 1);
    wdtAction->setLayout(layoutAction);

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(wdtAction, tr("Action"));
    tab->addTab(w1, tr("Databases"));
    tab->addTab(w2, tr("Store map"));
    tab->addTab(w3, tr("Food map"));

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(tab);
    setLayout(l);
    connect(this, SIGNAL(finished(int)), SLOT(writeData()));
    readData();
}

void QMainDialog::readDB()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/db");
    if (f.open(QIODevice::ReadOnly))
    {
        m_db.clear();
        while (!f.atEnd())
        {
            DBPair db;
            f.read(reinterpret_cast<char *>(&db), sizeof(DBPair));
            m_db.insert(db.name, db);
            QListWidgetItem *i = new QListWidgetItem(db.name, m_pDbList);
            i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
            i->setCheckState(Qt::Checked);
        }

        f.close();
    }
}

void QMainDialog::newDbPair()
{
    QDBPair *d = new QDBPair(this);
    d->makeStoreMap(m_store);
    d->makeFoodMap(m_food);
    if (d->exec() == QDialog::Accepted)
    {
        DBPair db;
        memset(&db, 0, sizeof(DBPair));
        memcpy(db.name, d->m_pName->text().toAscii(), d->m_pName->text().length());
        memcpy(db.src_db, d->m_pSrcDb->text().toAscii(), d->m_pSrcDb->text().length());
        memcpy(db.src_conn_str, d->m_pSrcConnStr->text().toAscii(), d->m_pSrcConnStr->text().length());
        memcpy(db.src_pass, d->m_pSrcPass->text().toAscii(), d->m_pSrcPass->text().length());
        memcpy(db.src_user, d->m_pSrcUser->text().toAscii(), d->m_pSrcUser->text().length());
        memcpy(db.dst_conn_str, d->m_pDstDb->text().toAscii(), d->m_pDstDb->text().length());
        memcpy(db.dst_user, d->m_pDstUser->text().toAscii(), d->m_pDstUser->text().length());
        memcpy(db.dst_pass, d->m_pDstPass->text().toAscii(), d->m_pDstPass->text().length());
        memcpy(db.storeConverter, d->m_pstoreConverter->currentText().toAscii(), d->m_pstoreConverter->currentText().length());
        memcpy(db.foodConverter, d->m_pfoodConverter->currentText().toAscii(), d->m_pfoodConverter->currentText().length());
        db.operatorId = d->m_pOperatorId->text().toInt();
        m_db.insert(db.name, db);
        QListWidgetItem *i = new QListWidgetItem(db.name, m_pDbList);
        i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
        i->setCheckState(Qt::Checked);
    }
    delete d;
}

void QMainDialog::editDbPair()
{
    QModelIndexList list = m_pDbList->selectionModel()->selectedRows();
    if (!list.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (list.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    DBPair db = m_db[m_pDbList->currentItem()->text()];
    QDBPair *d = new QDBPair(this);
    d->makeStoreMap(m_store);
    d->makeFoodMap(m_food);
    d->m_pName->setText(db.name);
    d->m_pSrcDb->setText(db.src_db);
    d->m_pSrcConnStr->setText(db.src_conn_str);
    d->m_pSrcUser->setText(db.src_user);
    d->m_pSrcPass->setText(db.src_pass);
    d->m_pDstDb->setText(db.dst_conn_str);
    d->m_pDstUser->setText(db.dst_user);
    d->m_pDstPass->setText(db.dst_pass);
    d->m_pOperatorId->setText(QString::number(db.operatorId));
    for (int i = 0; i < d->m_pstoreConverter->count(); i++)
        if (d->m_pstoreConverter->itemText(i) == db.storeConverter)
        {
            d->m_pstoreConverter->setCurrentIndex(i);
            break;
        }
    for (int i = 0; i < d->m_pfoodConverter->count(); i++)
        if (d->m_pfoodConverter->itemText(i) == db.foodConverter)
        {
            d->m_pfoodConverter->setCurrentIndex(i);
            break;
        }
    if (d->exec() == QDialog::Accepted)
    {
        memset(&db, 0, sizeof(DBPair));
        memcpy(db.name, d->m_pName->text().toAscii(), d->m_pName->text().length());
        memcpy(db.src_conn_str, d->m_pSrcConnStr->text().toAscii(), d->m_pSrcConnStr->text().length());
        memcpy(db.src_pass, d->m_pSrcPass->text().toAscii(), d->m_pSrcPass->text().length());
        memcpy(db.src_user, d->m_pSrcUser->text().toAscii(), d->m_pSrcUser->text().length());
        memcpy(db.src_db, d->m_pSrcDb->text().toAscii(), d->m_pSrcDb->text().length());
        memcpy(db.dst_conn_str, d->m_pDstDb->text().toAscii(), d->m_pDstDb->text().length());
        memcpy(db.dst_user, d->m_pDstUser->text().toAscii(), d->m_pDstUser->text().length());
        memcpy(db.dst_pass, d->m_pDstPass->text().toAscii(), d->m_pDstPass->text().length());
        memcpy(db.storeConverter, d->m_pstoreConverter->currentText().toAscii(), d->m_pstoreConverter->currentText().length());
        memcpy(db.foodConverter, d->m_pfoodConverter->currentText().toAscii(), d->m_pfoodConverter->currentText().length());
        db.operatorId = d->m_pOperatorId->text().toInt();
        m_db[m_pDbList->currentItem()->text()] = db;
        m_pDbList->currentItem()->setText(db.name);
    }
    delete d;
}

void QMainDialog::deleteDbPair()
{
    QModelIndexList list = m_pDbList->selectionModel()->selectedRows();
    if (!list.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    for (int i = list.count() - 1; i > -1; i--)
    {
        m_db.remove(m_pDbList->item(i)->text());
        QListWidgetItem *item = m_pDbList->item(i);
        delete(item);
    }
}

void QMainDialog::newStoreMap()
{
    QImap *m = new QImap(this);
    if (m->exec() == QDialog::Accepted)
    {
        if (m_store.contains(m->m_pMapName->text()))
            m_store.erase(m_store.find(m->m_pMapName->text()));

        QMap<QString, int> storeMap;
        for(int i = 0; i < m->m_t->m_model.rowCount(QModelIndex()); i++)
            storeMap[m->m_t->m_model.data(0, i).toString()] = m->m_t->m_model.Integer(1, i);

        m_store.insert(m->m_pMapName->text(), storeMap);
        QListWidgetItem *i = new QListWidgetItem(m->m_pMapName->text(), m_pStoreMap);
        Q_UNUSED(i);
    }
    delete m;
}

void QMainDialog::editStoreMap()
{
    QList<QListWidgetItem*> items = m_pStoreMap->selectedItems();
    if (items.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (items.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }
    QString currItem = items.at(0)->text();
    QImap *m = new QImap(this);
    m->m_pMapName->setText(currItem);
    for (QMap<QString, int>::const_iterator i = m_store[currItem].begin(); i != m_store[currItem].end(); i++)
    {
        int newRow = m->m_t->addRow();
        m->m_t->m_model.setData(newRow, 0, i.key());
        m->m_t->m_model.setData(newRow, 1, i.value());
    }
    if (m->exec() == QDialog::Accepted)
    {
        m_pStoreMap->item(0)->setText(m->m_pMapName->text());
        m_store.remove(currItem);

        QMap<QString, int> storeMap;
        for(int i = 0; i < m->m_t->m_model.rowCount(QModelIndex()); i++)
            storeMap[m->m_t->m_model.data(0, i).toString()] = m->m_t->m_model.Integer(1, i);

        m_store.insert(m->m_pMapName->text(), storeMap);
    }
    delete m;

}

void QMainDialog::deleteStoreMap()
{
    QList<QListWidgetItem*> items = m_pStoreMap->selectedItems();
    if (items.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    for (int i = m_pStoreMap->count() - 1; i > -1; i--)
    {
        QListWidgetItem *item = m_pStoreMap->item(i);
        m_store.remove(item->text());
        delete item;
    }

}

void QMainDialog::newFoodMap()
{
    bool ok = true;
    QString mapName = QInputDialog::getText(this, tr("Food map name"), tr("Name"), QLineEdit::Normal, "", &ok);
    if (!ok)
        return;
    if (!mapName.length())
    {
        QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty"));
        return;
    }
    if (m_food.contains(mapName))
    {
        QMessageBox::critical(this, tr("Error"), tr("The name already exists"));
        return;
    }

    QMultiMap <QString, int> map;
    m_food.insert(mapName, map);
    QListWidgetItem *i = new QListWidgetItem(mapName, m_pFoodMap);
    Q_UNUSED(i);
}

void QMainDialog::deleteFoodMap()
{
}

void QMainDialog::loadFoodMapFromFile()
{
    QList<QListWidgetItem *> ml = m_pFoodMap->selectedItems();
    if (!ml.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (ml.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    QString mapName = ml.at(0)->text();
    if (m_food.contains(mapName))
        m_food[mapName].clear();
    else
        m_food.insert(mapName, QMultiMap<QString, int>());

    QString fileName = QFileDialog::getOpenFileName(this, tr("Map file"), "", "Text files (*.txt)");
    if (fileName.length())
    {
        char line [255];
        QFile f(fileName);
        if (f.open(QIODevice::ReadOnly))
        {
            while (!f.atEnd())
            {
                f.readLine(line, 255);
                QStringList list = QString(line).split("=");
                m_food[mapName].insert(list[0], list[1].toInt());
            }
            f.close();
        }
    }
}

void QMainDialog::exportFoodMapToFile()
{
}

void QMainDialog::writeData()
{
    writeDB();
    writeStore();
    writeFood();
}

void QMainDialog::go()
{
    flushLog();
    bool all_success = true;

    for (int i = 0; i < m_pDbList->count(); i++)
        if (m_pDbList->item(i)->checkState() == Qt::Checked)
        {
            bool success = readDoc(m_db[m_pDbList->item(i)->text()]);
            all_success = all_success && success;

            if (success)
                all_success = all_success && writeDoc(m_db[m_pDbList->item(0)->text()]);
        }

    if (!all_success)
    {
        QStringList args;
        args << QDir::homePath() + "/QuickPoint/SyncAS/log.txt";
        QProcess p;
        p.execute("c:\\Windows\\notepad.exe", args);
    }

    if (all_success)
        QMessageBox::information(this, tr("Information"), tr("Success"));
}

void QMainDialog::about()
{
    QAboutDialog *about = new QAboutDialog(this);
    about->exec();
    delete about;
}


void QMainDialog::writeDB()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/db");
    if (f.open(QIODevice::ReadWrite))
    {
        for (QMap<QString, DBPair>::const_iterator i = m_db.begin(); i != m_db.end(); i++)
            f.write(reinterpret_cast<const char *>(&(i.value())), sizeof(DBPair));
        f.close();
    }
}

void QMainDialog::readStore()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/store");
    if (f.open(QIODevice::ReadOnly))
    {
        m_store.clear();
        char mapName [255];
        char storeAS [255];
        int storeQP;
        while (!f.atEnd())
        {
            f.read(mapName, 255);
            f.read(storeAS, 255);
            f.read(reinterpret_cast<char *>(&storeQP), sizeof(int));
            if (!m_store.contains(mapName))
            {
                m_store.insert(mapName, QMultiMap<QString, int>());
                QListWidgetItem *i = new QListWidgetItem(mapName, m_pStoreMap);
                Q_UNUSED(i);
            }
            m_store[mapName].insert(storeAS, storeQP);
        }
        f.close();
    }
}

void QMainDialog::writeStore()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/store");
    if (f.open(QIODevice::ReadWrite))
    {
        char mapName[255];
        char storeAS[255];
        int storeQP;
        for (QMap<QString, QMap<QString, int> >::const_iterator i = m_store.begin(); i != m_store.end(); i++)
        {
            memset(mapName, 0, 255);
            memcpy(mapName, i.key().toAscii(), i.key().length());
            for (QMap<QString, int>::const_iterator j = i.value().begin(); j != i.value().end(); j++)
            {
                memset(storeAS, 0, 255);
                memcpy(storeAS, j.key().toAscii(), j.key().length());
                storeQP = j.value();
                f.write(mapName, 255);
                f.write(storeAS, 255);
                f.write(reinterpret_cast<const char *>(&storeQP), sizeof(int));
            }
        }
        f.close();
    }
}

void QMainDialog::readData()
{
    readDB();
    readStore();
    readFood();
}

bool QMainDialog::readDoc(const DBPair &db)
{
    QString sql_header = QString("SELECT DISTINCT(D.fDOCNUM) AS ID, D.fDATE, M.fSTORAGE AS FSTORE, D.fSUMM AS FSUM "
                    "FROM %1.dbo.DOCUMENTS D, %1.dbo.MTHI M "
                    "WHERE D.fDOCNUM=M.fDOCNUM AND D.fDOCTYPE=6 AND D.fDATE BETWEEN :DATE1 AND :DATE2 "
                    "AND M.fOP='I' AND M.fDBCR='True'").arg(db.src_db);

    QString sql_body = QString("SELECT M.fDOCNUM AS fDOC, M.fMT AS FFOOD, M.fQTY AS FQNT, M.fSUMM AS FSUM "
                               "FROM %1.dbo.MTHI M, %1.dbo.DOCUMENTS D "
                               "WHERE M.fDOCNUM=D.fDOCNUM AND M.fDBCR='True' AND M.fOP='I' "
                               "AND D.fDOCTYPE=6 AND D.fDATE BETWEEN :DATE1 AND :DATE2").arg(db.src_db);

    QMap<QString, int> &storeMap = m_store[db.storeConverter];
    QMap<QString, int> &foodMap = m_food[db.foodConverter];
    bool success = true;
    flushLog();

    QSqlDatabase d = QSqlDatabase::addDatabase("QODBC3", "src");
    d.setDatabaseName(db.src_conn_str);
    d.setUserName(db.src_user);
    d.setPassword(db.src_pass);
    if (!d.open())
    {
        success = true;
        log(d.lastError().databaseText());
    }

    m_doc_headers.clear();
    QSqlQuery q(d);
    if (success)
    {
        if (!q.prepare(sql_header))
        {
            success = false;
            log(q.lastError().databaseText());
        }
        q.bindValue("DATE1", m_pDate1->date().toString("MM-dd-yyyy"));
        q.bindValue("DATE2", m_pDate2->date().toString("MM-dd-yyyy"));
    }

    if (success)
        if (!q.exec())
        {
            success = false;
            log(q.lastError().databaseText());
        }

    if (success)
        while(q.next())
        {
            if (!storeMap.contains(q.value(2).toString()))
            {
                success = false;
                log(QString("Cannot find key %1 in the store map\n").arg(q.value(2).toString()));
                continue;
            }
            DocHeaderItem dh;
            dh.date = q.value(1).toDate();
            dh.storeQP = storeMap[q.value(2).toString()];
            dh.amount = q.value(3).toDouble();
            m_doc_headers.insert(q.value(0).toString(), dh);
        }

    m_doc_body.clear();
    if (success)
    {
        if (!q.prepare(sql_body))
        {
            success = false;
            log(q.lastError().databaseText());
        }
        q.bindValue("DATE1", m_pDate1->date().toString("MM-dd-yyyy"));
        q.bindValue("DATE2", m_pDate2->date().toString("MM-dd-yyyy"));
    }

    if (success)
        if (!q.exec())
        {
            success = false;
            log(q.lastError().databaseText());
        }

    if (success)
        while (q.next())
        {
            if (!foodMap.contains(q.value(1).toString().trimmed()))
            {
                log(QString("Cannot find the key %1 in the food map\n").arg(q.value(1).toString()));
                success = false;
            }
            DocBodyItem i;
            i.foodCode = foodMap[q.value(1).toString().trimmed()];
            i.qty = q.value(2).toDouble();
            i.amount = q.value(3).toDouble();

            if (!m_doc_body.contains(q.value(0).toString()))
                m_doc_body.insert(q.value(0).toString(), QVector<DocBodyItem>());

            m_doc_body[q.value(0).toString()].push_back(i);
        }

    return success;
}

bool QMainDialog::writeDoc(const DBPair &db)
{
    bool success = true;
    QSqlDatabase d = QSqlDatabase::addDatabase("QIBASE", "dst");
    d.setDatabaseName(db.dst_conn_str);
    d.setUserName(db.dst_user);
    d.setPassword(db.dst_pass);

    if (!d.open())
    {
        success = false;
        log(d.lastError().databaseText());
    }
    QSqlQuery q(d);

    if (success)
        if (!q.prepare("DELETE FROM STORE_DOC WHERE DOC_DATE BETWEEN :DATE1 AND :DATE2 AND OPERATOR=:OPERATOR"))
        {
            success = false;
            log(q.lastError().databaseText());
        }

    if (success)
    {
        q.bindValue("DATE1", m_pDate1->date());
        q.bindValue("DATE2", m_pDate2->date());
        q.bindValue("OPERATOR", db.operatorId);
        if (!q.exec())
        {
            success = false;
            log(q.lastError().databaseText());
        }
    }

    if (success)
    {
        for (QMap<QString, DocHeaderItem>::const_iterator i = m_doc_headers.begin(); i != m_doc_headers.end(); i++)
        {
            int doc_id = genId(&q, "GEN_STORE_DOC_ID");
            if (!q.prepare("INSERT INTO STORE_DOC (ID, DOC_TYPE, DOC_STATE, DOC_DATE, STORE_IN_ID, STORE_OUT_ID, OPERATOR, COMMENT) "
                           "VALUES (:ID, :DOC_TYPE, :DOC_STATE, :DOC_DATE, :STORE_IN_ID, :STORE_OUT_ID, :OPERATOR, :COMMENT)"))
            {
                success = false;
                log(q.lastError().databaseText());
                continue;
            }

            if (success)
            {
                q.bindValue("ID", doc_id);
                q.bindValue("DOC_TYPE", 1);
                q.bindValue("DOC_STATE", 1);
                q.bindValue("DOC_DATE", i.value().date); qDebug() <<  i.value().date.toString("dd.MM.yyyy");
                q.bindValue("STORE_IN_ID", i.value().storeQP);
                q.bindValue("STORE_OUT_ID", 0);
                q.bindValue("OPERATOR", db.operatorId);
                q.bindValue("COMMENT", "Synchronization");
                if (!q.exec())
                {
                    success = false;
                    log(q.lastError().databaseText());
                }
            }

            if (success)
            {
                QVector<DocBodyItem> &body = m_doc_body[i.key()];
                for (QVector<DocBodyItem>::const_iterator j = body.begin(); j != body.end(); j++)
                {
                    if (!q.prepare("INSERT INTO STORE_DRAFT (DOC_ID, GOODS_ID, QTY, PRICE) VALUES (:DOC_ID, :GOODS_ID, :QTY, :PRICE)"))
                    {
                        success = false;
                        log(d.lastError().databaseText());
                        continue;
                    }

                    if (success)
                    {
                        q.bindValue("DOC_ID", doc_id);
                        q.bindValue("GOODS_ID", j->foodCode);
                        q.bindValue("QTY", j->qty);
                        q.bindValue("PRICE", j->amount / j->qty);
                        if (!q.exec())
                        {
                            success = false;
                            log(q.lastError().databaseText());
                            continue;
                        }
                    }

                    if (!q.prepare("INSERT INTO STORE_GOODS (DOC_ID, STORE_ID, GOODS_ID, QTY, PRICE) VALUES (:DOC_ID, :STORE_ID, :GOODS_ID, :QTY, :PRICE)"))
                    {
                        success = false;
                        log(q.lastError().databaseText());
                        continue;
                    }

                    if (success)
                    {
                        q.bindValue("DOC_ID", doc_id);
                        q.bindValue("STORE_ID", i.value().storeQP);
                        q.bindValue("GOODS_ID", j->foodCode);
                        q.bindValue("QTY", j->qty);
                        q.bindValue("PRICE", j->amount / j->qty);
                        if (!q.exec())
                        {
                            success = false;
                            log(q.lastError().databaseText());
                            continue;
                        }
                    }
                }
            }
        }
    }

    return success;
}

int QMainDialog::genId(QSqlQuery *q, const QString &gen_name)
{
    q->exec(QString("SELECT GEN_ID(%1, 1) FROM RDB$DATABASE").arg(gen_name));
    q->next();
    return q->value(0).toInt();
}

void QMainDialog::log(const QString &msg)
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/log.txt");
    if (f.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        f.write(QString("%1: %2\r\n").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss")).arg(msg).toAscii());
        f.close();
    }
}

void QMainDialog::flushLog()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/log.txt");
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    f.close();
}


void QMainDialog::readFood()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/food");
    if (f.open(QIODevice::ReadOnly))
    {
        char mapName[255];
        char foodAS [255];
        int foodQP;
        while (!f.atEnd())
        {
            memset(mapName, 0, 255);
            memset(foodAS, 0, 255);
            foodQP = 0;
            f.read(mapName, 255);
            f.read(foodAS, 255);
            f.read(reinterpret_cast<char *>(&foodQP), sizeof(int));
            if (!m_food.contains(mapName))
            {
                m_food.insert(mapName, QMultiMap<QString, int>());
                QListWidgetItem *i = new QListWidgetItem(mapName, m_pFoodMap);
                Q_UNUSED(i);
            }
            m_food[mapName].insert(foodAS, foodQP);
        }
    }
}

void QMainDialog::writeFood()
{
    QFile f(QDir::homePath() + "/QuickPoint/SyncAS/food");
    if (f.open(QIODevice::ReadWrite))
    {
        char mapName[255];
        char foodAS[255];
        int foodQP;
        for (QMap<QString, QMap<QString, int> >::const_iterator i = m_food.begin(); i != m_food.end(); i++)
        {
            memset(mapName, 0, 255);
            memcpy(mapName, i.key().toAscii(), i.key().length());
            for (QMultiMap<QString, int>::const_iterator j = i.value().begin(); j != i.value().end(); j++)
            {
                memset(foodAS, 0, 255);
                memcpy(foodAS, j.key().toAscii(), j.key().length());
                foodQP = j.value();
                f.write(mapName, 255);
                f.write(foodAS, 255);
                f.write(reinterpret_cast<const char *>(&foodQP), sizeof(int));
            }
        }
        f.close();
    }
}
