#include "qmessagedialog.h"
#include <QHBoxLayout>
#include <QPushButton>

QMessageDialog::QMessageDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
{
    m_pMessage = new QLabel(this);
    m_pMessage->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);

    QWidget *pwdtBtn = new QWidget(this);
    QHBoxLayout *plBtn = new QHBoxLayout();
    QPushButton *pbtnOk = new QPushButton(tr("OK"), this);
    pbtnOk->setMinimumSize(QSize(120, 60));
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    pbtnCancel->setMinimumSize(QSize(120, 60));
    plBtn->addStretch(1);
    plBtn->addWidget(pbtnOk);
    plBtn->addWidget(pbtnCancel);
    plBtn->addStretch(1);
    pwdtBtn->setLayout(plBtn);

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(m_pMessage);
    plMain->addWidget(pwdtBtn);
    setLayout(plMain);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
}

int QMessageDialog::ShowMessage(const QString &message, QWidget *parent)
{
    QMessageDialog *m = new QMessageDialog(parent);
    m->m_pMessage->setText(QString("<br><br><font size=\"20\">%1</font>").arg(message));
    m->setWindowState(Qt::WindowMaximized);
    int result = m->exec();
    delete m;
    return result;
}
