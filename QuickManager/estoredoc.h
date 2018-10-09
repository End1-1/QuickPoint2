#ifndef ESTOREDOC_H
#define ESTOREDOC_H

#include "tablemodel.h"
#include <QDialog>
#include <QDate>
#include "data.h"
#include "../All/qprint.h"

#define STORE_INPUT 1
#define STORE_OUTPUT 2
#define STORE_MOVEMENT 3
#define STORE_INVENTORY 4
#define STORE_SALE 5

#define STATE_DRAFT 0
#define STATE_WRITED 1

class QPrintCommand;

class QLineEdit;
class QLabel;
class QDateEdit;
class TableView;
class EGoodsForDoc;
class QSqlQuery;

class EStoreDoc : public QDialog
{
    Q_OBJECT

public:
    explicit EStoreDoc(QWidget *parent = 0);
    ~EStoreDoc();
    bool docSaved;
    void getActionName();
    void getStoreOutName();
    void countAmount();
    void enableStoreInputs();
    void loadDoc();
    QLineEdit *m_peId;
    TableView *m_ptGoods;
    QDateEdit *m_pdDate;
    QLineEdit *m_peAction;
    QLabel *m_plAction;
    QLineEdit *m_peStoreOut;
    QLabel *m_plStoreOut;
    QLineEdit *m_peOperator;

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    struct OutputGoods {
        int goods;
        double qty;
        double price;
        int guid;
        QDate date;
        OutputGoods () {goods = 0; qty = 0; price = 0; guid = 0; }
    };

    Database m_db;
    QLineEdit *m_peState;
    QLabel *m_plState;
    QLineEdit *m_peStoreIn;
    QLabel *m_plStoreIn;
    QLineEdit *m_peComment;
    QLineEdit *m_peAmount;

    void setData(EGoodsForDoc *e, const int &row);
    bool checkData();
    bool writeDraft();
    void writeGoodsDraft();
    bool writeGoodsOutput();
    void writeDocIn();
    bool startDoc();
    void endDoc();

    void printMain(QPrint &printer, int &page_num);
    void printRow(QPrint &printer, QVector<int> &points, QVector<QString> &values, int &top, int row_height = 10);

private slots:
    void slotAction();
    void slotStoreIn();
    void slotStoreOut();
    void slotStoreInGetName();
    void slotStoreOutGetName();
    void slotActionGetName();
    void slotNew();
    void slotEdit();
    void slotDelete();
    void slotWrite();
    void slotDraft();
    void slotPrint();
    void slotClose();
    void slotDocStateChanged(const QString &value);
    void slotDateChanged(const QDate &value);
    void slotStoreInChanged(const QString &value);
    void slotStoreOutChanged(const QString &value);
    void slotCommentChanged(const QString &value);
};

#endif // ESTOREDOC_H
