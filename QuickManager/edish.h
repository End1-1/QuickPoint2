#ifndef EDISH_H
#define EDISH_H

#include "editwindow.h"
#include <QtGui>
#include "tablemodel.h"

class EComboItem;
class AModifiers;

class EDish : public EditWindow
{
    Q_OBJECT

public:
    EDish(QWidget *parent = 0);
    void setColor();

    QString m_menu;
    QString m_group;
    QString m_store;
    QString m_printer;
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Menu;
    QLineEdit *pedt_Group;
    QLineEdit *pedt_Name;
    QLineEdit *pedt_Price;
    QLineEdit *pedt_Store;
    QLineEdit *pedt_Printer;
    QCheckBox *pchk_Visible;
    QPushButton *pbtn_Color;
    QColor *m_color;
    QLineEdit *pedt_Queue;
    QLineEdit *m_pPrintQueue;
    QCheckBox *m_pchkCombo;
    TableView *m_ptCombo;
    QPushButton *m_pbNew;
    QPushButton *m_pbEdit;
    QPushButton *m_pbDel;
    QPushButton *m_pbNewMod;
    QPushButton *m_pbEditMod;
    QPushButton *m_pbDelMod;
    TableView *m_ptModifiers;


private:
    bool checkData();
    void setComboData(const EComboItem *e, const int &row);
    void setModData(const AModifiers *a, const int &row);

private slots:
    void slotMenu();
    void slotGroup();
    void slotStore();
    void slotPrinter();
    void slotChangeColor();
    void slotNewCombo();
    void slotEditCombo();
    void slotDelCombo();
    void slotNewMod();
    void slotEditMod();
    void slotDelMod();

public slots:
    void slotCheckCombo(const bool &value);
};

#endif // EDISH_H
