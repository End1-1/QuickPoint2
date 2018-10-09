#include "qpassworddialog.h"
#include "qnumpadwidget.h"

QPasswordDialog::QPasswordDialog(QWidget *parent) :
    QBaseDialog(tr("Password"), parent)
{
    QNumpadWidget *pNumpad = new QNumpadWidget(this);
    QGridLayout *plMain = new QGridLayout();
    plMain->setRowStretch(0, 1);
    plMain->setColumnStretch(0, 1);
    plMain->setColumnStretch(2, 1);
    plMain->setRowStretch(2, 1);
    plMain->addWidget(pNumpad, 1, 1);
    setLayout(plMain);

    connect(pNumpad, SIGNAL(Finish(QString)), SLOT(OnFinish(QString)));
}

void QPasswordDialog::OnFinish(QString value)
{
    if (!value.length())
    {
        reject();
        return;
    }

    emit Finish(value);
    m_password = value;
    accept();
}
