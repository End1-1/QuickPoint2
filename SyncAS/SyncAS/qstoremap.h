#ifndef QSTOREMAP_H
#define QSTOREMAP_H

#include <QDialog>
#include <QtGui>
#include "../QuickManager/tablemodel.h"

class QImap : public QDialog
{
    Q_OBJECT

private:
    QAction *m_pNew;
    QAction *m_pEdit;
    QAction *m_pDelete;
    void setData(const QString &key, const QString &value, const int &row);

public:
    TableView *m_t;
    QLineEdit *m_pMapName;
    explicit QImap(QWidget *parent);

private slots:
    void New();
    void Edit();
    void Delete();
    void Ok();
};


class QKeyValue : public QDialog
{
    Q_OBJECT

public:
    explicit QKeyValue(QWidget *parent = 0);
    QLineEdit *m_pKey;
    QLineEdit *m_pValue;

private slots:
    void Ok();
};

#endif // QSTOREMAP_H
