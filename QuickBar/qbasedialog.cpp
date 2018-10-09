#include "qbasedialog.h"
#include "qsettingsdialog.h"
#include <QSqlError>

int QEDatabase::Count = 0;

QBaseDialog::QBaseDialog(QString title, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(title);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowState(windowState() | Qt::WindowMaximized);
    m_db.setDBParams(QSettingsDialog::connection);
}

QPushButton *QBaseDialog::qpushbutton(const QString &text)
{
    QPushButton *pbtn = new QPushButton(text, this);
    pbtn->setMinimumSize(MINSIZE);
    return pbtn;
}

QWidget *QBaseDialog::m_pwdtOkCancel()
{

    QWidget *pwdtOkCancel = new QWidget(this);
    QHBoxLayout *plButtons = new QHBoxLayout();
    QPushButton *pbtnOk = qpushbutton(tr("OK"));
    pbtnOk->setMinimumWidth(120);
    QPushButton *pbtnCancel = qpushbutton(tr("Cancel"));
    pbtnCancel->setMinimumWidth(120);
    plButtons->addStretch(1);
    plButtons->addWidget(pbtnOk);
    plButtons->addWidget(pbtnCancel);
    plButtons->addStretch(1);
    pwdtOkCancel->setLayout(plButtons);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(onOk()));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(onCancel()));

    return pwdtOkCancel;
}

bool QBaseDialog::isOk()
{
    return true;
}

void QBaseDialog::onOk()
{
    if (isOk())
        accept();
}

void QBaseDialog::onCancel()
{
    reject();
}

bool QEDatabase::exec(QSqlQuery *q, const QString &sql)
{
    if (q->exec(sql))
        return true;
    else
    {
        QMessageBox::critical(0, "SQL error", q->lastError().databaseText());
        return false;
    }
}

bool QEDatabase::exec(QSqlQuery *q)
{
    if (q->exec())
        return true;
    else
    {
        QMessageBox::critical(0, "SQL error", q->lastError().databaseText());
        return false;
    }
}

bool QEDatabase::prepare(QSqlQuery *q, const QString &sql)
{
    if (q->prepare(sql))
        return true;
    else
    {
        QMessageBox::critical(0, "SQL error", q->lastError().databaseText());
        return false;
    }
}

void QEDatabase::close()
{
    m_db.close();
}

bool QEDatabase::beginTransaction()
{
    return m_db.transaction();
}

bool QEDatabase::commitTransaction()
{
    return m_db.commit();
}

QSqlQuery *QEDatabase::query()
{
    QSqlQuery *q = new QSqlQuery(m_db);
    return q;
}


QEPushButton::QEPushButton(const int index, const QString &text, QWidget *parent) :
    QPushButton(text, parent), Index(index)
{
}

void QEPushButton::setState(const QEPushButton::ButtonState state)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), Qt::green);
    setPalette(p);
}
