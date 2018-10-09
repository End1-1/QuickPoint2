#ifndef EGOODS_H
#define EGOODS_H

#include "editwindow.h"
#include <QtGui>

class EGoods : public EditWindow
{
    Q_OBJECT

public:
    EGoods(QWidget *parent = 0);
    QLineEdit *pedt_Id;
    QLineEdit *pedt_Name;
    QLineEdit *pedt_Group;
    QLineEdit *pedt_Unit;
    QString m_group;
    QString m_unit;

private:
    bool checkData();

private slots:
    void slotGroup();
    void slotUnit();
};

#endif // EGOODS_H
