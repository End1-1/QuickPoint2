#include "eemployee.h"
#include "checklist.h"

EEmployee::EEmployee(QWidget *parent) : EditWindow(parent)
{
    setWindowTitle(tr("Employee"));
    int i = -1;
    QGridLayout *l = new QGridLayout();
    l->setColumnMinimumWidth(1, 200);

    l->addWidget(new QLabel(tr("Id")), ++i, 0);
    l->addWidget(pedt_Id = new QLineEdit(), i, 1);
    pedt_Id->setEnabled(false);

    l->addWidget(new QLabel(tr("Position")), ++i, 0);
    l->addWidget(pedt_Position = new QLineEdit(), i, 1);
    pedt_Position->setEnabled(false);
    QPushButton *pbtn_Position = new QPushButton("...");
    pbtn_Position->setMaximumSize(25, 25);
    connect(pbtn_Position, SIGNAL(clicked()), SLOT(slotPosition()));
    l->addWidget(pbtn_Position, i, 2);

    l->addWidget(new QLabel(tr("Name")), ++i, 0);
    l->addWidget(pedt_Name = new QLineEdit(), i, 1);

    l->addWidget(new QLabel(tr("Database access")), ++i, 0);
    l->addWidget(pchk_DBAccess = new QCheckBox(), i, 1);
    connect(pchk_DBAccess, SIGNAL(stateChanged(int)), SLOT(slotDBAccess(int)));

    l->addWidget(new QLabel(tr("Password")), ++i, 0);
    l->addWidget(pedt_Password = new QLineEdit(), i, 1);
    pedt_Password->setEnabled(false);
    pedt_Password->setEchoMode(QLineEdit::Password);
    pedt_Password->setText("*********");
    connect(pedt_Password, SIGNAL(textChanged(QString)), SLOT(slotPassword(QString)));

    l->addWidget(new QLabel(tr("Role")), ++i, 0);
    l->addWidget(pedt_Role = new QLineEdit(), i, 1);
    pedt_Role->setEnabled(false);
    QPushButton *pbtn_Role = new QPushButton("...");
    pbtn_Role->setMaximumSize(25, 25);
    connect(pbtn_Role, SIGNAL(clicked()), SLOT(slotRole()));
    l->addWidget(pbtn_Role, i, 2);

    l->addWidget(new QLabel(tr("Active")), ++i, 0);
    l->addWidget(pchk_Active = new QCheckBox(), i, 1);

    l->addWidget(m_pwidget, ++i, 0, 1, 3);
    setLayout(l);

    m_position = "";
    m_role = "";
    m_password = "";
}

bool EEmployee::checkData()
{
    QString msg = "";
    bool result = true;

    if (m_position.length() == 0)
    {
        msg += tr("The position cannot be empty\n");
        result = false;
    }

    if (pedt_Name->text().length() == 0)
    {
        msg += tr("The name cannot be empty\n");
        result = false;
    }

    if (pchk_DBAccess->isChecked())
    {
        if (m_password.length() == 0)
        {
            msg += tr("The password cannot be empty\n");
            result = false;
        }

        if (m_role.length() == 0)
        {
            msg += tr("The role cannot be empth\n");
            result = false;
        }
    }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}

void EEmployee::slotPosition()
{
    CheckList *c = new CheckList(false, this);

    if (m_position.length() > 0)
        c->result.push_back(m_position);
    c->sql("SELECT ID, NAME FROM POSITIONS ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Position->clear();
        m_position = "";
        if (c->result.count() > 0)
        {
            pedt_Position->setText(c->values[c->checked[0]].toString());
            m_position = c->key[c->checked[0]].toString();
        }
    }
    delete c;
}

void EEmployee::slotRole()
{
    CheckList *c = new CheckList(false, this);

    if (m_role.length() > 0)
        c->result.push_back(m_role);
    c->sql("SELECT ID, NAME FROM ROLES ORDER BY NAME", 1);

    if (c->exec() == QDialog::Accepted)
    {
        pedt_Role->clear();
        m_role = "";
        if (c->result.count() > 0)
        {
            pedt_Role->setText(c->values[c->checked[0]].toString());
            m_role = c->key[c->checked[0]].toString();
        }
    }
    delete c;
}

void EEmployee::slotPassword(const QString &value)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    m_password = h.hash(value.toLatin1(), QCryptographicHash::Md5).toBase64();
}

void EEmployee::slotDBAccess(const int &value)
{
    pedt_Password->setEnabled(value != 0);
    pedt_Role->setEnabled(value != 0);
}
