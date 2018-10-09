#include "login.h"
#include "simplepassword.h"
#include "global.h"
#include "../All/econnection.h"
#include <QSqlQuery>
#include <QSqlError>

Login::Login(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Login"));

    QGridLayout *plMain = new QGridLayout();
    plMain->setColumnMinimumWidth(1, 200);
    int i = -1;
    plMain->addWidget(new QLabel(tr("Database"), this), ++i, 0);
    plMain->addWidget(m_databases = new QComboBox(this), i, 1);
    QPushButton *pbtnAddDb = new QPushButton("+", this);
    pbtnAddDb->setMaximumSize(25, 25);
    plMain->addWidget(pbtnAddDb, i, 2);
    QPushButton *pbtnRemoveDB = new QPushButton("-", this);
    pbtnRemoveDB->setMaximumSize(25, 26);
    plMain->addWidget(pbtnRemoveDB, i, 3);
    plMain->addWidget(new QLabel(tr("Password"), this), ++i, 0);
    plMain->addWidget(m_password = new QLineEdit(this), i, 1);
    m_password->setEchoMode(QLineEdit::Password);

    QWidget *pwdtButtons = new QWidget(this);
    QHBoxLayout *plButtons = new QHBoxLayout();
    QPushButton *pbtnOk = new QPushButton(tr("OK"), this);
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    plButtons->addStretch(1);
    plButtons->addWidget(pbtnOk);
    plButtons->addWidget(pbtnCancel);
    plButtons->addStretch(1);
    pwdtButtons->setLayout(plButtons);

    plMain->addWidget(pwdtButtons, ++i, 0, 1, 4);
    setLayout(plMain);

    readConnections();

    connect(pbtnAddDb, SIGNAL(clicked()), SLOT(slotPlusDB()));
    connect(pbtnRemoveDB, SIGNAL(clicked()), SLOT(slotRemoveDB()));
    connect(pbtnOk, SIGNAL(clicked()), SLOT(slotOk()));
    connect(m_password, SIGNAL(returnPressed()), SLOT(slotOk()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));

    setTabOrder(m_password, m_databases);
    setTabOrder(m_databases, pbtnOk);
    setTabOrder(pbtnOk, pbtnCancel);
    setTabOrder(pbtnCancel, pbtnAddDb);
    setTabOrder(pbtnAddDb, pbtnRemoveDB);
    m_password->setFocus();
}

void Login::readConnections()
{
    m_databases->clear();
    EConnection::ReadConnections();
    for (QVector<EConnection::ConnectionData>::const_iterator i = EConnection::m_connections.begin(); i != EConnection::m_connections.end(); i++)
        m_databases->addItem(i->Alias);
    m_databases->setCurrentIndex(0);
}

void Login::slotOk()
{
    if (!m_databases->count())
    {
        QMessageBox::critical(this, tr("Error"), tr("No databases defined."));
        return;
    }

    if (m_databases->currentIndex() < 0)
        return;

    bool result = false;

    {
        EConnection::setConnectionParams(M::db.db, m_databases->currentIndex());
        if (!M::db.db.open())
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not connect to database\n") + M::db.db.lastError().databaseText());
            QSqlDatabase::removeDatabase("main");
            return;
        }

        if (!(result = M::user.checkPassword(m_password->text())))
            QMessageBox::critical(this, tr("Error"), tr("Access deined"));
    }

    if (result)
        accept();
}

void Login::slotPlusDB()
{
    EConnection *e = new EConnection(this);
    if (e->exec() == QDialog::Accepted)
    {
        EConnection::ConnectionData c;
        e->getConnection(c);
        EConnection::WriteConnection(Data::connectionsDir + "/" + c.Alias, c);
        readConnections();
    }
    delete e;
}

void Login::slotRemoveDB()
{
    if (m_databases->currentIndex() < 0)
        return;

    SimplePassword *s = new SimplePassword(tr("Enter the settings password"), this);
    if (s->exec() == QDialog::Accepted)
    {
        QString connectionFile = Data::connectionsDir + "/" + EConnection::m_connections.at(m_databases->currentIndex()).Alias;
        if (EConnection::CheckConnectionPassword(connectionFile, s->m_password->text()))
            EConnection::DeleteConnection(EConnection::m_connections.at(m_databases->currentIndex()));
        readConnections();
    }
    delete s;
}
