#ifndef FSTOREDOC_H
#define FSTOREDOC_H

#include "editwindow.h"
#include "tablemodel.h"
#include <QtGui>

class FStoreDoc : public EditWindow
{
    Q_OBJECT

public:
    FStoreDoc(QWidget *parent = 0);

    QCheckBox *m_pchkId;

    QDateEdit *m_pDate1;
    QDateEdit *m_pDate2;
    QCheckBox *m_pchkDate;

    QLineEdit *m_peDocType;
    QPushButton *m_pbDocType;
    QCheckBox *m_pchkDocType;
    QLabel *m_plDocType;

    QLineEdit *m_peDocState;
    QPushButton *m_pbDocState;
    QCheckBox *m_pchkDocState;
    QLabel *m_plDocState;

    QLineEdit *m_peStoreIn;
    QCheckBox *m_pchkStoreIn;
    QPushButton *m_pbStoreIn;
    QLabel *m_plStoreIn;

    QLineEdit *m_peStoreOut;
    QCheckBox *m_pchkStoreOut;
    QPushButton *m_pbStoreOut;
    QLabel *m_plStoreOut;

    QLineEdit *m_peFood;
    QPushButton *m_pbFood;
    QCheckBox *m_pchkFood;
    QLabel *m_plFood;

private:
    bool checkData();
    Database *m_db;

private slots:
    void slotGetDocTypeByCode();
    void slotGetDocType();
    void slotGetDocStateByCode();
    void slotGetDocState();
    void slotGetStoreInByCode();
    void slotGetStoreIn();
    void slotGetStoreOutByCode();
    void slotGetStoreOut();
    void slotGetFoodByCode();
    void slotGetFood();
};

#endif // FSTOREDOC_H
