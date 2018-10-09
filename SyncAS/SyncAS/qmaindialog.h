#ifndef QMAINDIALOG_H
#define QMAINDIALOG_H

#include <QDialog>
#include <QMap>
#include <QtGui>
#include "qdbpair.h"

class QListWidget;
class QAction;
class QSqlQuery;

struct DocHeaderItem {
    int id;
    QDate date;
    int storeQP;
    double amount;
};

struct DocBodyItem  {
    int foodCode;
    double qty;
    double amount;
};

class QMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QMainDialog(QWidget *parent = 0);

private:
    QListWidget *m_pDbList;
    QListWidget *m_pStoreMap;
    QListWidget *m_pFoodMap;
    QAction *m_pNew;
    QAction *m_pEdit;
    QAction *m_pDelete;

    QAction *m_pStoreNew;
    QAction *m_pStoreEdit;
    QAction *m_pStoreDelete;

    QAction *m_pFoodNewMap;
    QAction *m_pFoodLoadFromFile;
    QAction *m_pFoodDeleteMap;
    QAction *m_pFoodExportToFile;

    QMap< QString, DBPair> m_db;
    QMap< QString, QMap<QString, int> > m_store;
    QMap< QString, QMap<QString, int> > m_food;

    QDateEdit *m_pDate1;
    QDateEdit *m_pDate2;

    void readDB();
    void writeDB();
    void readStore();
    void writeStore();
    void readFood();
    void writeFood();
    void readData();

    QMap<QString, DocHeaderItem> m_doc_headers;
    QMap<QString, QVector<DocBodyItem> > m_doc_body;
    bool readDoc(const DBPair &db);
    bool writeDoc(const DBPair &db);
    int genId(QSqlQuery *q, const QString &gen_name);
    void log(const QString &msg);
    void flushLog();

private slots:
    void newDbPair();
    void editDbPair();
    void deleteDbPair();
    void newStoreMap();
    void editStoreMap();
    void deleteStoreMap();
    void newFoodMap();
    void deleteFoodMap();
    void loadFoodMapFromFile();
    void exportFoodMapToFile();
    void writeData();
    void go();
    void about();
};

#endif // QMAINDIALOG_H
