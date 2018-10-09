#include "qsettingsdialog.h"

Connection QSettingsDialog::connection;

QSettingsDialog::QSettingsDialog(QWidget *parent) :
    QBaseDialog(tr("Connection parameters"), parent)
{
    QGridLayout *plMain = new QGridLayout();
    int i =  -1;

    plMain->setRowStretch(++i, 1);
    plMain->setColumnStretch(0, 1);
    plMain->setColumnStretch(3, 1);
    plMain->setColumnMinimumWidth(2, 200);
    plMain->addWidget(new QLabel(tr("Path to database"), this), ++i, 1);
    plMain->addWidget(m_pConnectionString = new QLineEdit(this), i, 2);
    plMain->addWidget(new QLabel(tr("Username"), this), ++i, 1);
    plMain->addWidget(m_pUsername = new QLineEdit(this), i, 2);
    plMain->addWidget(new QLabel(tr("Password"), this), ++i, 1);
    plMain->addWidget(m_pPassword = new QLineEdit(this), i, 2);
    plMain->addWidget(new QLabel(tr("Settings password"), this), ++i, 1);
    plMain->addWidget(m_pSettingsPassword = new QLineEdit(this), i, 2);
    plMain->setRowStretch(++i, 2);
    plMain->addWidget(m_pwdtOkCancel(), ++i, 0, 1, 4);
    setLayout(plMain);

    readConnection();
    m_pConnectionString->setText(connection.PathToDb);
    m_pUsername->setText(connection.Username);
    m_pPassword->setText(connection.Password);
    m_pSettingsPassword->setText(connection.SettingsPassword);
}

void QSettingsDialog::readConnection()
{
    QFile file(Data::fileConnection);
    if (file.open(QIODevice::ReadOnly))
    {
        file.read(reinterpret_cast<char *>(&connection), sizeof(Connection));
        file.close();
    }
}

bool QSettingsDialog::checkPassword(const QString &password)
{
    return (password == QString(connection.SettingsPassword));
}

void QSettingsDialog::onOk()
{
    memset(&connection, 0, sizeof(Connection));
    memcpy(&connection.PathToDb[0], m_pConnectionString->text().toAscii(), m_pConnectionString->text().length());
    memcpy(&connection.Username[0], m_pUsername->text().toAscii(), m_pUsername->text().length());
    memcpy(&connection.Password[0], m_pPassword->text().toAscii(), m_pPassword->text().length());
    memcpy(&connection.SettingsPassword[0], m_pSettingsPassword->text().toAscii(), m_pSettingsPassword->text().length());

    QFile file(Data::fileConnection);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reinterpret_cast<const char *>(&connection), sizeof(Connection));
        file.close();
    }

    accept();
}
