#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include "../All/db.h"
#include "ComPort.h"
#include "../All/classes.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtPrintSupport/QPrinter>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>


#define BTN_SQUARE QSize(60, 60)
#define TABLE_GOODS_CELL QSize(130, 60)


class MainWidget : public QDialog
{
    Q_OBJECT

    struct Goods {
        QString record_id;
        int id;
        QString name;
        QString group;
        double price;
        double quantity;
        double printedQuantity;
        int color;
        int printSchema;
        int printQueue;
        int store;
        Goods ()
        {
            record_id = "";
            id = 0;
            name = "";
            group = "";
            price = 0;
            quantity = 1;
            printedQuantity = 0;
            color = 0;
            printSchema = -1;
            printQueue = 0;
            store = 0;
        }
    };

private:
   QString fCard;
   double fCardValue;
   double fCardAmount;
   int fCardId;
   QPushButton *pbtnLeftScrollDown;
   QPushButton *pbtnLeftScrollUp;
   QPushButton *pbtnMiddleScrollDown;
   QPushButton *pbtnMiddleScrollUp;
   QPushButton *pbtnRightScrollDown;
   QPushButton *pbtnRightScrollUp;
   QPushButton *pbtnServiceCheck;
   QPushButton *pbtnPayment;
   QWidget *m_pwdtLeft;
   QListWidget *m_pGoodsGroup;
   QTableWidget *m_pGoods;
   QListWidget *m_pOrder;
   QLineEdit *m_pOrderAmount;
   QStringList m_GoodsGroups;
   QVector<Goods> m_Goods;
   QMap<int, QVariant> m_OrderData;
   QVector<Goods> m_OrderGoods;
   QMap<int, QVector<QString> > m_PrinterSchema;
   QString m_GoodsFilter;
   QStringList m_GoodsNoDiscount;

   void buildGoodsGroupView();
   void buildGoodsView();
   void addGoodsToOrder(const Goods &goods);
   void removeGoodsFromOrder(const int &index);
   void countOrderAmount();
   void setOrderScrollEnabled();
   void newOrder();
   void writeOrder();
   void clearOrder();
   void paymentClick();
   void serviceCheckClick();

   void servicePrintBegin();
   void servicePrint(const QString &printer, QVector<Goods> goods);
   void paymentPrint();

public:
   MainWidget(QWidget *parent = 0);
   void buildMenu();
   CUser u;

private slots:
   void slotFilterGoods(QModelIndex index);
   void slotShowGoodsGroups();
   void slotScrollGoodsGroupsDown();
   void slotScrollGoodsGroupsUp();
   void slotScrollGoodsDown();
   void slotScrollGoodsUp();
   void slotScrollOrderDown();
   void slotScrollOrderUp();
   void slotGoodsClick(QModelIndex index);
   void slotDeleteGoodsClick();
   void slotChangeGoodsQuantity(QString quantity);
   void slotPaymentButtonClick();
   void slotEnableServiceCheckBtn();
   void slotEnablePaymentBtn();
   void slotServiceBtnClick();
   void slotHalfBtnClick();
   void btnCardClicked();
   void btnDiscountClicked();
   void btnCheckCard();
};

#endif // MAINWIDGET_H
