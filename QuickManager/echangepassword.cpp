#include "echangepassword.h"
#include "global.h"

EChangePassword::EChangePassword(QWidget *parent) :
    EditWindow(parent)
{
    setWindowTitle(tr("Change password"));
    int i = -1;
    QGridLayout *l = new QGridLayout(this);

    l->addWidget(new QLabel(tr("Old password")), ++i, 0);
    l->addWidget(m_pOldPassword = new QLineEdit(), i, 1);
    m_pOldPassword->setEchoMode(QLineEdit::Password);

    l->addWidget(new QLabel(tr("New password")), ++i, 0);
    l->addWidget(m_pNewPassword = new QLineEdit(), i, 1);
    m_pNewPassword->setEchoMode(QLineEdit::Password);

    l->addWidget(new QLabel(tr("New password confirm")), ++i, 0);
    l->addWidget(m_pNewPasswordConfirm = new QLineEdit(), i, 1);
    m_pNewPasswordConfirm->setEchoMode(QLineEdit::Password);

    l->addWidget(m_pwidget, ++i, 0, 1, 2);
    setLayout(l);
}

bool EChangePassword::checkData()
{
    QCryptographicHash h(QCryptographicHash::Md5);
    QString oldPwd = h.hash(m_pOldPassword->text().toLatin1(), QCryptographicHash::Md5).toBase64();
    QString newPwd = h.hash(m_pNewPassword->text().toLatin1(), QCryptographicHash::Md5).toBase64();
    QString newPwdConfirm = h.hash(m_pNewPasswordConfirm->text().toLatin1(), QCryptographicHash::Md5).toBase64();
    int user_id = 0;

    if (m_pNewPassword->text().trimmed().length() == 0 || m_pNewPassword->text().toLower() == "about")
    {
        QMessageBox::critical(this, tr("Error"), tr("Illegal password"));
        return false;
    }

    if (newPwd != newPwdConfirm)
    {
        QMessageBox::critical(this, tr("Error"), tr("Passwords not match"));
        return false;
    }

    QSqlQuery *q = m_db.QuerySelect("SELECT EMPLOYEE_ID FROM LOGINS WHERE PASSWORD=:PASSWORD",
                                    QStringList() << "PASSWORD",
                                    QVector<QVariant>() << oldPwd);
    if (q->next())
        user_id = q->value(0).toInt();
    delete q;
    m_db.db.close();

    if (!user_id)
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid password"));
        return false;
    }

    if (user_id != M::user.Id)
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid password"));
        return false;
    }

    if (q->next())
    {
        QMessageBox::critical(this, tr("Error"), tr("Cannot set password"));
        return false;
    }

    m_db.QueryUpdate("LOGINS",
                     QStringList() << "PASSWORD",
                     QVector<QVariant>() << newPwd,
                     "EMPLOYEE_ID", user_id);

    return true;
}
