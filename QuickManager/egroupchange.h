#ifndef EGROUPCHANGE_H
#define EGROUPCHANGE_H

#include <QDialog>
#include <QtGui>
#include "eunit.h"

class EGroupChange : public QDialog
{
    Q_OBJECT
public:
    explicit EGroupChange(QWidget *parent = 0);
    QString m_menu;
    QString m_group;
    QString m_store;
    QString m_print;
    QLineEdit *m_pMenuName;
    QLineEdit *m_pGroupName;
    QLineEdit *m_pStoreName;
    QLineEdit *m_pPrintName;
    QLineEdit *m_pColor;
    QLineEdit *m_pQueue;
    QCheckBox *m_pVisible;
    QCheckBox *m_pImNotIdiot;
    QPushButton *m_pbtnOk;
    QPushButton *m_pbtnCancel;
    QPushButton *m_pbtnSelectMenu;
    QPushButton *m_pbtnSelectGroup;
    QPushButton *m_pbtnSelectStore;
    QPushButton *m_pbtnSelectPrint;
    QPushButton *m_pbtnSelectColor;

private slots:
    void slotMenu();
    void slotGroup();
    void slotStore();
    void slotPrint();
    void slotColor();
    void slotImNotIdiot(bool checked = false);
    
};

#endif // EGROUPCHANGE_H
