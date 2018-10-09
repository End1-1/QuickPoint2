#include "qstoremap.h"
#include <QAction>


QImap::QImap(QWidget *parent) : QDialog(parent)
{

    setWindowTitle(tr("Store map"));
    setMinimumSize(400, 400);

    m_pNew = new QAction(tr("New"), this);
    m_pEdit = new QAction(tr("Edit"), this);
    m_pDelete = new QAction(tr("Delete"), this);

    connect(m_pNew, SIGNAL(triggered()), SLOT(New()));
    connect(m_pEdit, SIGNAL(triggered()), SLOT(Edit()));
    connect(m_pDelete, SIGNAL(triggered()), SLOT(Delete()));

    m_pMapName = new QLineEdit(this);

    m_t = new TableView(this);
    m_t->m_model.insertColumns(0, 2, QModelIndex());
    m_t->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_t->addAction(m_pNew);
    m_t->addAction(m_pEdit);
    m_t->addAction(m_pDelete);

    QPushButton *pbOk = new QPushButton(tr("OK"), this);
    QPushButton *pbCancel = new QPushButton(tr("Cancel"), this);
    connect(pbOk, SIGNAL(clicked()), SLOT(Ok()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(reject()));

    QGridLayout *l = new QGridLayout(this);
    int i = -1;

    l->addWidget(new QLabel(tr("Map name")), ++i, 0);
    l->addWidget(m_pMapName, i, 1);
    l->addWidget(m_t, ++i, 0, 1, 2);
    l->addWidget(pbOk, ++i, 0, 1, 1, Qt::AlignRight);
    l->addWidget(pbCancel, i, 1, 1, 1, Qt::AlignLeft);

    setLayout(l);
}

void QImap::New()
{
    QKeyValue *kv = new QKeyValue(this);
    if (kv->exec() == QDialog::Accepted)
    {
        int newRow = m_t->addRow();
        setData(kv->m_pKey->text(), kv->m_pValue->text(), newRow);
    }
    delete kv;
}

void QImap::Edit()
{

    QModelIndexList indexes = m_t->selectionModel()->selectedRows();
    QVector<int> rows;

    for (QModelIndexList::const_iterator i = indexes.begin(); i != indexes.end(); i++)
        if (!rows.contains(i->row()))
            rows.push_back(i->row());

    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    if (rows.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    QKeyValue *kv = new QKeyValue(this);
    kv->m_pKey->setText(m_t->m_model.String(0, rows[0]));
    kv->m_pValue->setText(m_t->m_model.String(1, rows[0]));
    if (kv->exec() == QDialog::Accepted)
        setData(kv->m_pKey->text(), kv->m_pValue->text(), rows[0]);
    delete kv;
}

void QImap::Delete()
{
    QModelIndexList indexes = m_t->selectionModel()->selectedRows();
    QVector<int> rows;

    for (QModelIndexList::const_iterator i = indexes.begin(); i != indexes.end(); i++)
        if (!rows.contains(i->row()))
            rows.push_back(i->row());

    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    for (int i = rows.count() - 1; i > -1; i--)
        m_t->m_model.removeRows(rows[i], 1, QModelIndex());
}

void QImap::Ok()
{
    bool result = true;
    QString msg = "";

    if (m_pMapName->text().length() == 0)
    {
        result = false;
        msg += tr("The name cannot be empty.\n");
    }

    if (!result)
    {
        QMessageBox::critical(this, tr("Error"), msg);
        return;
    }

    accept();
}


QKeyValue::QKeyValue(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Key - Value"));
    QGridLayout *l = new QGridLayout(this);
    int i = -1;

    m_pKey = new QLineEdit(this);
    m_pValue = new QLineEdit(this);
    QPushButton *pbOk = new QPushButton(tr("OK"), this);
    QPushButton *pbCancel = new QPushButton(tr("Cancel"), this);

    connect(pbOk, SIGNAL(clicked()), SLOT(Ok()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(reject()));

    l->addWidget(new QLabel(tr("Key")), ++i, 0);
    l->addWidget(m_pKey, i, 1);
    l->addWidget(new QLabel(tr("Value")), ++i, 0);
    l->addWidget(m_pValue, i, 1);
    l->addWidget(pbOk, ++i, 0, 1, 1, Qt::AlignRight);
    l->addWidget(pbCancel, i, 1, 1, 1, Qt::AlignLeft);

    setLayout(l);
}

void QKeyValue::Ok()
{
    QString msg = "";
    bool result = true;

    if (m_pKey->text().length() == 0)
    {
        result = false;
        msg += tr("The key is empty.\n");
    }

    if (m_pKey->text().length() == 0)
    {
        result = false;
        msg += tr("The value is empty.\n");
    }

    if (!result)
    {
        QMessageBox::critical(this, tr("Error"), msg);
        return;
    }

    accept();
}


void QImap::setData(const QString &key, const QString &value, const int &row)
{
    m_t->m_model.setData(row, 0, key);
    m_t->m_model.setData(row, 1, value);
}

