#ifndef EGOODSFORDOC_H
#define EGOODSFORDOC_H

#include <QDialog>
#include <QtGui>
#include "eunit.h"

class EGoodsForDoc : public QDialog
{
    Q_OBJECT

public:
    explicit EGoodsForDoc(QWidget *parent = 0);
    ~EGoodsForDoc();
    QLineEdit *m_peFoodId;
    QLineEdit *m_peFoodName;
    QLineEdit *m_peQty;
    QLineEdit *m_peUnit;
    QString m_unit;
    QLineEdit *m_pePrice;
    QLineEdit *m_peAmount;
    bool next;
    QPushButton *m_pbNext;

private:
    bool checkData();

private slots:
    void slotFood();
    void slotFoodByCode();
    void slotPrice();
    void slotQty();
    void slotAmount();
    void slotNext();
    void slotOk();
    void slotCancel();
    void slotReturnQty();
    void slotReturnPrice();
    void slotReturnAmount();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // EGOODSFORDOC_H
