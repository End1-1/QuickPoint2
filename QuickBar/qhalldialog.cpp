#include "qhalldialog.h"
#include "qlistdialog.h"
#include "qcashdialog.h"

QHallDialog::QHallDialog(const QUser &user, QWidget *parent) :
    QBaseDialog(tr("Hall"), parent), m_TableId(0), m_user(user)
{
    m_pquery = 0;
    m_hallFilter = Data::settings[S_DEFAULT_HALL].toInt();

    m_psaTable = new QScrollArea(this);
    m_psaTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    plTable = 0;
    pwdtTable = 0;
    makeTablesList();

    QWidget *pwdtBottom = new QWidget(this);
    QHBoxLayout *plBottom = new QHBoxLayout();
    QPushButton *pbtn_ScrollUp = qpushbutton("");
    pbtn_ScrollUp->setIcon(QPixmap(":/icons/up.png"));
    QPushButton *pbtn_ScrollDown = qpushbutton("");
    pbtn_ScrollDown->setIcon(QPixmap(":/icons/down.png"));
    QPushButton *pbtn_Quit = qpushbutton("");
    pbtn_Quit->setIcon(QPixmap(":/icons/exit.png"));
    QPushButton *pbtn_Hall = qpushbutton(tr("Hall"));
    QPushButton *pbtn_Cash = qpushbutton(tr("Cash"));
    pbtn_Cash->setVisible(user.m_user.Roles.contains(ROLE_VIEW_SALE_DOCS));
    QPushButton *pbtn_UsersPasswords = qpushbutton(tr("Users"));
    pbtn_UsersPasswords->setVisible(false);
    plBottom->addWidget(pbtn_ScrollUp);
    plBottom->addWidget(pbtn_ScrollDown);
    plBottom->addStretch(1);
    plBottom->addWidget(pbtn_Hall);
    plBottom->addWidget(pbtn_Cash);
    //plBottom->addWidget(pbtn_UsersPasswords);
    plBottom->addStretch(1);
    plBottom->addWidget(pbtn_Quit);
    pwdtBottom->setLayout(plBottom);

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(m_psaTable);
    plMain->addWidget(pwdtBottom);
    setLayout(plMain);

    connect(pbtn_Quit, SIGNAL(clicked()), SLOT(reject()));
    connect(pbtn_ScrollUp, SIGNAL(clicked()), SLOT(OnBtnScrollUp()));
    connect(pbtn_ScrollDown, SIGNAL(clicked()), SLOT(OnBtnScrollDown()));
    connect(pbtn_Hall, SIGNAL(clicked()), SLOT(OnBtnHallClicked()));
    connect(pbtn_Cash, SIGNAL(clicked()), SLOT(OnBtnCashClicked()));
    connect(pbtn_UsersPasswords, SIGNAL(clicked()), SLOT(OnBtnUsersPasswordsClicked()));
}

QHallDialog::~QHallDialog()
{
    if (m_pquery)
    {
        delete m_pquery;
        m_db.commitTransaction();
        m_db.close();
    }
}

void QHallDialog::makeTablesList()
{
    for (QMap<int, QEPushButton *>::iterator i = m_vecButtons.begin(); i != m_vecButtons.end(); i++)
        delete i.value();
    m_vecButtons.clear();

    if (plTable)
        delete plTable;
    if (pwdtTable)
        delete pwdtTable;

    plTable = new QGridLayout();
    pwdtTable = new QWidget(this);

    QRect r = QApplication::desktop()->screenGeometry();
    r.setWidth(r.width() - 30);
    int col = r.width() / TABLE_SIZE.width();
    int btn_count = 0;

    QMultiMap<int, Table> temp_tables;
    for (QMap<int, Table>::const_iterator i = Data::tables.begin(); i != Data::tables.end(); i++)
    {
        if (m_hallFilter)
            if (m_hallFilter != i.value().hall.Id)
                continue;

        temp_tables.insert(i.value().Queue, i.value());
    }

    for (QMultiMap<int, Table>::const_iterator i = temp_tables.begin(); i != temp_tables.end(); i++)
    {
        QEPushButton *pb = new QEPushButton(i.value().Id, i.value().Name, this);
        m_vecButtons.insert(i.value().Id, pb);
        pb->setMinimumSize(TABLE_SIZE);
        pb->setMaximumSize(TABLE_SIZE);
        connect(pb, SIGNAL(clicked()), SLOT(OnBtnTableClicked()));
        plTable->addWidget(pb, btn_count / col, btn_count % col);
        btn_count++;
    }



    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.prepare(q, "SELECT TABLE_ID FROM SALE_HEADER WHERE STATUS_ID=:STATUS_ID");
    q->bindValue("STATUS_ID", ORDER_OPEN);
    m_db.exec(q);
    while (q->next())
    {
        if (m_vecButtons.contains(q->value(0).toInt()))
            m_vecButtons[q->value(0).toInt()]->setState(QEPushButton::TableOpened);
    }
    m_db.close();
    delete q;

    pwdtTable->setLayout(plTable);
    m_psaTable->setWidget(pwdtTable);
}

void QHallDialog::OnBtnScrollDown()
{
    m_psaTable->verticalScrollBar()->setValue(m_psaTable->verticalScrollBar()->value() + 500);
}

void QHallDialog::OnBtnScrollUp()
{
    m_psaTable->verticalScrollBar()->setValue(m_psaTable->verticalScrollBar()->value() - 500);
}

void QHallDialog::OnBtnTableClicked()
{
    m_TableId = ((QEPushButton *)sender())->Index;
    m_db.open();
    m_db.beginTransaction();
    m_pquery = m_db.query();
    m_db.prepare(m_pquery, "SELECT ID FROM HALL_TABLES WHERE ID=:ID"); //With lock
    m_pquery->bindValue("ID", m_TableId);
    m_db.exec(m_pquery);
    m_pquery->next();
    accept();
}

void QHallDialog::OnBtnHallClicked()
{
    QListDialog *l = new QListDialog(this);

    QListWidgetItem *wi = new QListWidgetItem(l->m_pList);
    wi->setSizeHint(QSize(100, 60));
    wi->setText(tr("All"));
    wi->setTextAlignment(Qt::AlignCenter);
    wi->setData(Qt::UserRole, 0);

    for (QMap<int, Hall>::const_iterator i = Data::halls.begin(); i != Data::halls.end(); i++)
    {
        QListWidgetItem *wi = new QListWidgetItem(l->m_pList);
        wi->setSizeHint(QSize(100, 60));
        wi->setText(i->Name);
        wi->setTextAlignment(Qt::AlignCenter);
        wi->setData(Qt::UserRole, i.key());
    }

    if (l->exec() == QDialog::Accepted)
    {
        m_hallFilter = l->m_pList->selectedItems()[0]->data(Qt::UserRole).toInt();
        makeTablesList();
    }

    delete l;
}

void QHallDialog::OnBtnCashClicked()
{
    QCashDialog *c = new QCashDialog(this);
    c->exec();
    delete c;
}

void QHallDialog::OnBtnUsersPasswordsClicked()
{
}
