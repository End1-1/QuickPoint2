#include "eposition.h"
#include "tablemodel.h"

EPosition::EPosition(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Positions"));
    int i = -1;

    QWidget *w1 = new QWidget(this);
    QGridLayout *l1 = new QGridLayout();
    l1->addWidget(new QLabel(tr("Id")), ++i, 0);
    l1->addWidget(pedt_Id = new QLineEdit(), i, 1);
    pedt_Id->setEnabled(false);
    l1->addWidget(new QLabel(tr("Name")), ++i, 0);
    l1->addWidget(pedt_Name = new QLineEdit(), i, 1);
    l1->setRowStretch(++i, 1);
    w1->setLayout(l1);

    QWidget *w2 = new QWidget(this);
    QHBoxLayout *l2 = new QHBoxLayout(this);
    QPushButton *pbNew = new QPushButton(tr("New"), this);
    QPushButton *pbEdit = new QPushButton(tr("Edit"), this);
    QPushButton *pbDelete = new QPushButton(tr("Delete"), this);
    l2->setMargin(0);
    l2->addWidget(pbNew);
    l2->addWidget(pbEdit);
    l2->addWidget(pbDelete);
    l2->addStretch(1);
    w2->setLayout(l2);

    QWidget *w3 = new QWidget(this);
    QVBoxLayout *l3 = new QVBoxLayout(this);
    m_pSalary = new TableView();
    m_pSalary->setMinimumSize(400, 200);
    m_pSalary->m_model.insertColumns(0, 4, QModelIndex());
    m_pSalary->setColumn(0, "QUEUE", tr("Queue"), 50, DATA_INTEGER);
    m_pSalary->setColumn(1, "OPERATION", tr("Operation"), 150, DATA_STRING);
    m_pSalary->setColumn(2, "PARAMS", tr("Params"), 100, DATA_STRING);
    m_pSalary->setColumn(3, "CELL", tr("Cell"), 50, DATA_STRING);
    l3->addWidget(w2);
    l3->addWidget(m_pSalary);
    l3->addStretch(1);
    w3->setLayout(l3);

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(w1, tr("Main"));
    tab->addTab(w3, tr("Salary"));

    QVBoxLayout *layoutMain = new QVBoxLayout(this);
    layoutMain->addWidget(tab);
    layoutMain->addWidget(m_pwidget);
    setLayout(layoutMain);

    connect(pbNew, SIGNAL(clicked()), SLOT(newFormula()));
    connect(pbEdit, SIGNAL(clicked()), SLOT(editFormula()));
    connect(pbDelete, SIGNAL(clicked()), SLOT(deleteFormula()));
}

bool EPosition::checkData()
{
    bool result = true;
    QString msg;

    if (pedt_Name->text().length() == 0)
    {
        msg += tr("The name cannot be empty\n");
        result = false;
    }

    if (m_pSalary->m_model.rowCount(QModelIndex()))
    {
        bool have_return = false;
        for (int i = 0; i < m_pSalary->m_model.rowCount(QModelIndex()); i++)
        {
            if (m_pSalary->m_model.String("OPERATION", i) == OP_RETURN)
            {
                have_return = true;
                break;
            }
            if (m_pSalary->m_model.String("OPERATION", i) == OP_MANUAL)
            {
                have_return = true;
                break;
            }
        }
        if (!have_return)
        {
            msg += tr("Salary formula hasnt an exit point\n");
            result = false;
        }
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EPosition::setSalaryData(ESalaryFormula *e, const int &row)
{
    m_pSalary->m_model.setData(row, "QUEUE", e->m_pQueue->text());
    m_pSalary->m_model.setData(row, "OPERATION", e->m_pOperation->currentText());
    m_pSalary->m_model.setData(row, "PARAMS", e->m_pParams->text());
    m_pSalary->m_model.setData(row, "CELL", e->m_pCell->currentText());
}

void EPosition::markQueue()
{
    for (int i = 0; i < m_pSalary->m_model.rowCount(QModelIndex()); i++)
        m_pSalary->m_model.setData(i, "QUEUE", i + 1);
}

void EPosition::newFormula()
{
    ESalaryFormula *e = new ESalaryFormula(this);
    e->m_pQueue->setText(QString::number(m_pSalary->m_model.rowCount(QModelIndex()) + 1));
    if (e->exec() == QDialog::Accepted)
    {
        int newRow = m_pSalary->addRow();
        int queue = e->m_pQueue->text().toInt();
        if (newRow + 1 < queue)
            e->m_pQueue->setText(QString::number(newRow + 1));
        setSalaryData(e, newRow);
        if (newRow + 1 != queue)
        {
            m_pSalary->m_model.rowReplace(queue - 1, newRow);
            markQueue();
        }
    }
    delete e;
}

void EPosition::editFormula()
{
    QVector<int> rows;
    if (!m_pSalary->checkRowsCount(rows, false))
        return;
    ESalaryFormula *e = new ESalaryFormula(this);
    e->m_pQueue->setText(m_pSalary->m_model.String("QUEUE", rows[0]));
    int index = e->m_pOperation->findText(m_pSalary->m_model.String("OPERATION", rows[0]));
    if (index > -1)
        e->m_pOperation->setCurrentIndex(index);
    e->m_pParams->setText(m_pSalary->m_model.String("PARAMS", rows[0]));
    index = e->m_pCell->findText(m_pSalary->m_model.String("CELL", rows[0]));
    if (index > -1)
        e->m_pCell->setCurrentIndex(index);
    if (e->exec() == QDialog::Accepted)
    {
        setSalaryData(e, rows[0]);
        int queue = e->m_pQueue->text().toInt();
        if (queue > m_pSalary->m_model.rowCount(QModelIndex()))
            queue = m_pSalary->m_model.rowCount(QModelIndex());
        if (rows[0] + 1 != queue)
        {
            m_pSalary->m_model.rowReplace(rows[0], queue - 1);
            markQueue();
        }
    }
    delete e;
}

void EPosition::deleteFormula()
{
    QVector<int> rows;
    if(!m_pSalary->checkRowsCount(rows))
        return;
    for (int i = rows.count() - 1; i > -1; i--)
        m_pSalary->m_model.removeRows(rows[i], 1, QModelIndex());
    markQueue();
}
