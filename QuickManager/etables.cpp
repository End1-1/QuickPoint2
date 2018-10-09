#include "etables.h"
#include "checklist2.h"

ETables::ETables(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Table"));
    QGridLayout *layout = new QGridLayout(this);
    int i = -1;

    layout->addWidget(new QLabel(tr("Id"), this), ++i, 0);
    layout->addWidget(m_pId = new QLineEdit(this), i, 1);
    m_pId->setEnabled(false);
    layout->addWidget(new QLabel(tr("Hall"), this), ++i, 0);
    layout->addWidget(m_pHallName = new QLineEdit(this), i, 1);
    m_pHallName->setEnabled(false);
    m_pbtnHall = newBtn(this);
    layout->addWidget(m_pbtnHall, i, 2);
    layout->addWidget(new QLabel(tr("Table name"), this), ++i, 0);
    layout->addWidget(m_pTableName = new QLineEdit(this), i, 1);
    m_pTableName->setFocus();
    layout->addWidget(new QLabel(tr("Queue"), this), ++i, 0);
    layout->addWidget(m_pQueue = new QLineEdit(this), i, 1);
    m_pQueue->setValidator(new QIntValidator(this));
    layout->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(layout);

    m_HallId = 0;

    connect(m_pbtnHall, SIGNAL(clicked()), SLOT(slotHall()));

}

bool ETables::checkData()
{
    bool result = true;
    QString msg = "";

    if (!m_HallId)
    {
        result = false;
        msg += tr("The hall is not selected\n");
    }

    if (!m_pTableName->text().length())
    {
        result = false;
        msg += tr("The name cannot be empty\n");
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void ETables::slotHall()
{
    CheckList2 *c = new CheckList2(false, this);

    c->buildList("SELECT ID, NAME FROM HALL ORDER BY NAME", "ID", "NAME");

    if (c->exec() == QDialog::Accepted)
    {
        QString keys, values;
        c->returnResult(keys, values);
        if (values.length())
        {
            m_pHallName->setText(values);
            m_HallId = keys.toInt();
        }
    }
    delete c;
}
