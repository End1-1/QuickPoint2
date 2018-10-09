#include "simplepassword.h"

SimplePassword::SimplePassword(const QString &title, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Settings password"));
    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 200);
    int i = -1;

    plMain->addWidget(new QLabel(title, this), ++i, 0, 1, 2);
    plMain->addWidget(new QLabel(tr("Password"), this), ++i, 0);
    plMain->addWidget(m_password = new QLineEdit(this), i, 1);
    m_password->setEchoMode(QLineEdit::Password);

    QWidget *pwdtButtons = new QWidget(this);
    QHBoxLayout *plButtons = new QHBoxLayout();
    QPushButton *pbtnOk= new QPushButton(tr("OK"), this);
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    plButtons->addWidget(pbtnOk);
    plButtons->addWidget(pbtnCancel);
    pwdtButtons->setLayout(plButtons);
    plMain->addWidget(pwdtButtons, ++i, 0, 1, 2);
    setLayout(plMain);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(slotOk()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
}

void SimplePassword::slotOk()
{
    if (!m_password->text().length())
    {
        QMessageBox::critical(this, tr("Error"), tr("The password cannot be empty"));
        return;
    }

    accept();
}
