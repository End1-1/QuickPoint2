#ifndef CHECKLIST2_H
#define CHECKLIST2_H

#include <QDialog>
#include <QtGui>
#include "tablemodel.h"
#include "eunit.h"

class CheckList2 : public QDialog
{
    Q_OBJECT

private:
    QString m_colKey;
    QString m_colValue;
    bool m_multiCheck;

public:
    explicit CheckList2(bool multiCheck, QWidget *parent = 0);
    void buildList(const QString &sql, const QString &colKey, const QString &colValue);
    TableView *m_pt;
    QListWidget *m_pl;
    QMap<int, QVariant> m_result;
    void returnResult(QString &keys, QString &values);

    static void Data(const QString &sql, QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void Store(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void GroupGoods(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void Goods(const QString &group, QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void Menu(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void DishesGroups(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void StoreActions(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);
    static void InOut(QLineEdit *names, QString &ids, bool multiCheck, QWidget *parent);

private slots:
    void slotItemChanged(QModelIndex index);
    void slotOk();
};

#endif // CHECKLIST2_H
