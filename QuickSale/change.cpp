#include "change.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

change::change(double bill, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
{
    QList<double> change;
    change << 10 << 20 << 50 << 100 << 200 << 500 << 1000 << 5000 << 10000 << 15000 << 20000;

    int i = -1;
    QGridLayout *plMain = new QGridLayout();
    m_bill = new QLineEdit(this);
    m_bill->setReadOnly(true);
    m_bill->setText(QString::number(bill, 'f', 0));
    m_change = new QLineEdit(this);
    m_change->setReadOnly(true);
    m_change->setText("0");

    plMain->addWidget(new QLabel(tr("Bill:"), this), ++i, 0);
    plMain->addWidget(new QLabel(tr("Banknote:"), this), i, 1);
    plMain->addWidget(m_bill, ++i, 0);
    plMain->addWidget(m_change, i, 1);

    int ost1 = static_cast<int>(bill) % 100;
    double near1 = (bill - ost1) + 100;
    int ost2 = static_cast<int>(bill) % 1000;
    double near2 = (bill - ost2) + 1000;

    QPushButton *pbtnNear1 = new QPushButton(QString::number(near1, 'f', 0), this);
    QPushButton *pbtnNear2 = new QPushButton(QString::number(near2, 'f', 0), this);

    btnSize(pbtnNear1);
    btnSize(pbtnNear2);

    connect(pbtnNear1, SIGNAL(clicked()), SLOT(slotAmount()));
    connect(pbtnNear2, SIGNAL(clicked()), SLOT(slotAmount()));

    plMain->addWidget(pbtnNear1, ++i, 0);
    plMain->addWidget(pbtnNear2, i, 1);

    int r = 0;
    for(int j = 0; j < change.count(); j++)
        if(change[j] >= bill) {
            QPushButton *pbtnChange = new QPushButton(QString::number(change[j], 'f', 0), this);
            btnSize(pbtnChange);
            plMain->addWidget(pbtnChange, (r % 2 ? i : ++i), r % 2);
            connect(pbtnChange, SIGNAL(clicked()), SLOT(slotAmount()));
            r++;
        }

    QPushButton *pbtnOk = new QPushButton(tr("Ok"), this);
    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    btnSize(pbtnOk);
    plMain->addWidget(pbtnOk, ++i, 0);

    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
    btnSize(pbtnCancel);
    plMain->addWidget(pbtnCancel, i, 1);

    setLayout(plMain);
}

void change::btnSize(QPushButton *btn)
{
    btn->setMinimumSize(150, 70);
}

void change::slotAmount()
{
    double change = dynamic_cast<QPushButton *>(sender())->text().toDouble() - m_bill->text().toDouble();
    m_change->setText(QString::number(change, 'f', 0));

}
