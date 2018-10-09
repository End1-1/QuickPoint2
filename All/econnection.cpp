#include "econnection.h"

QVector<EConnection::ConnectionData> EConnection::m_connections;

EConnection::EConnection(QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Connection parameters"));

    int i = -1;
    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 200);
    plMain->addWidget(new QLabel(tr("Alias"), this), ++i, 0);
    plMain->addWidget(m_alias = new QLineEdit(this), i, 1);
    plMain->addWidget(new QLabel(tr("Database"), this), ++i, 0);
    plMain->addWidget(m_path = new QLineEdit(this), i, 1);
    plMain->addWidget(new QLabel(tr("Username"), this), ++i, 0);
    plMain->addWidget(m_username = new QLineEdit(this), i, 1);
    plMain->addWidget(new QLabel(tr("Password"), this), ++i, 0);
    plMain->addWidget(m_password = new QLineEdit(this), i, 1);
    plMain->addWidget(new QLabel(tr("Settings password"), this), ++i, 0);
    plMain->addWidget(m_settingsPassword = new QLineEdit(this), i, 1);

    QWidget *pwdtButtons = new QWidget(this);
    QHBoxLayout *plButtons = new QHBoxLayout();
    QPushButton *pbtnOk = new QPushButton(tr("OK"));
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"));
    plButtons->addStretch(1);
    plButtons->addWidget(pbtnOk);
    plButtons->addWidget(pbtnCancel);
    plButtons->addStretch(1);
    pwdtButtons->setLayout(plButtons);

    plMain->addWidget(pwdtButtons, ++i, 0, 1, 2);
    setLayout(plMain);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(slotOk()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
}

void EConnection::getConnection(EConnection::ConnectionData &data)
{
    memset(&data, 0, sizeof(EConnection::ConnectionData));
    memcpy(&data.Alias[0], m_alias->text().toLatin1(), m_alias->text().length());
    memcpy(&data.Database[0], m_path->text().toLatin1(), m_path->text().length());
    memcpy(&data.Username[0], m_username->text().toLatin1(), m_username->text().length());
    memcpy(&data.Password[0], m_password->text().toLatin1(), m_password->text().length());
    memcpy(&data.SettingsPassword[0], m_settingsPassword->text().toLatin1(), m_settingsPassword->text().length());
}

void EConnection::setConnection(EConnection::ConnectionData &data)
{
    m_alias->setText(data.Alias);
    m_path->setText(data.Database);
    m_username->setText(data.Username);
    m_password->setText(data.Password);
    m_settingsPassword->setText(data.SettingsPassword);
}

void EConnection::WriteConnection(const QString &fileName, EConnection::ConnectionData connection)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        file.write(reinterpret_cast<const char *>(&connection), sizeof(ConnectionData));
        file.close();
    }
}

void EConnection::DeleteConnection(EConnection::ConnectionData connection)
{
    QFile::remove(QString(Data::connectionsDir + "/" + connection.Alias));
}

void EConnection::ReadConnections()
{
    QDir dir;
    dir.cd(Data::connectionsDir);
    QStringList files = dir.entryList();

    m_connections.clear();
    for (QStringList::const_iterator i = files.begin(); i != files.end(); i++)
    {
        if ((*i == ".") || (*i == ".."))
            continue;
        ConnectionData c;
        ReadConnection(Data::connectionsDir + "/" + *i, c);
        m_connections.push_back(c);
    }
}

void EConnection::ReadConnection(const QString &fileName, EConnection::ConnectionData &data)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        file.read(reinterpret_cast<char *>(&data), sizeof(ConnectionData));
        file.close();
    }
}

bool EConnection::CheckConnectionPassword(const QString &connectionFile, const QString &password)
{
    QFile file(connectionFile);
    if (file.open(QIODevice::ReadOnly))
    {
        ConnectionData c;
        file.read(reinterpret_cast<char *>(&c), sizeof(ConnectionData));
        file.close();

        if (!QString(c.SettingsPassword).length())
            return false;

        if (QString(c.SettingsPassword) == password)
            return true;
    }
    else
        return true;

    return false;
}

void EConnection::setConnectionParams(QSqlDatabase &db, const int &index)
{
    db.setDatabaseName(m_connections.at(index).Database);
    db.setUserName(m_connections.at(index).Username);
    db.setPassword(m_connections.at(index).Password);
}

void EConnection::setConnectionParams(QSqlDatabase &db, const ConnectionData &data)
{
    db.setDatabaseName(data.Database);
    db.setUserName(data.Username);
    db.setPassword(data.Password);
}

bool EConnection::checkData()
{
    return true;
}


void EConnection::slotOk()
{
    if (!m_alias->text().length())
    {
        QMessageBox::critical(this, tr("Error"), tr("Alias cannot be empty"));
        return;
    }

    accept();
}
