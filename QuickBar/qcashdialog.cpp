#include "qcashdialog.h"
#include "qorderdialog.h"

QCashDialog::QCashDialog(QWidget *parent) :
    QBaseDialog(tr("Cash"), parent)
{
    QHBoxLayout *plButtons = new QHBoxLayout();
    QWidget *pwdtButtons = new QWidget(this);
    QPushButton *pbtnDown = qpushbutton("");
    pbtnDown->setIcon(QPixmap(":/icons/down.png"));
    QPushButton *pbtnUp = qpushbutton("");
    pbtnUp->setIcon(QPixmap(":/icons/up.png"));
    QPushButton *pbtnClose = qpushbutton("");
    pbtnClose->setIcon(QPixmap(":/icons/exit.png"));
    plButtons->addWidget(pbtnDown);
    plButtons->addWidget(pbtnUp);
    plButtons->addStretch(1);
    plButtons->addWidget(pbtnClose);
    pwdtButtons->setLayout(plButtons);

    QHBoxLayout *plTotal = new QHBoxLayout();
    QWidget *pwdtTotal = new QWidget(this);
    plTotal->addWidget(new QLabel(tr("Total orders / Total amount"), this));
    plTotal->addWidget(m_pTotal = new QLineEdit(this));
    m_pTotal->setEnabled(false);
    pwdtTotal->setLayout(plTotal);

    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(m_pOrders = new QListWidget(this));
    m_pOrders->setItemDelegate(new QOrdersItemDelegate(m_Data));
    plMain->addWidget(pwdtTotal);
    plMain->addWidget(pwdtButtons);
    setLayout(plMain);

    connect(pbtnDown, SIGNAL(clicked()), SLOT(OnDown()));
    connect(pbtnUp, SIGNAL(clicked()), SLOT(OnUp()));
    connect(pbtnClose, SIGNAL(clicked()), SLOT(reject()));
    connect(m_pOrders, SIGNAL(clicked(QModelIndex)), SLOT (OnListClicked(QModelIndex)));

    getData();
}

void QCashDialog::getData()
{
    QDate date = QDate::currentDate().addDays((QTime::currentTime() < QTime::fromString(Data::settings[S_AUTO_CHANGE_DATE_OF_CASH].toString(), TIME_FORMAT) ? -1 : 0));
    QString sql = "SELECT SH.ID, SM.DATA, HT.NAME AS TABLE_NAME, E.NAME AS EMPLOYEE_NAME, SH.AMOUNT "
            "FROM SALE_HEADER SH "
            "LEFT JOIN SALE_MODS SM ON SH.ID=SM.ORDER_ID AND SM.MOD_ID=:MOD_ID "
            "LEFT JOIN HALL_TABLES HT ON SH.TABLE_ID=HT.ID "
            "LEFT JOIN EMPLOYEES E ON SH.WAITER_ID=E.ID "
            "WHERE SH.DATE_CASH=:DATE_CASH AND SH.STATUS_ID=:STATUS_ID";
    double total_amount = 0;

    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.prepare(q, sql);
    q->bindValue("MOD_ID", ORDER_MOD_TIME_CLOSE);
    q->bindValue("DATE_CASH", date);
    q->bindValue("STATUS_ID", ORDER_CLOSED);
    m_db.exec(q);
    while (q->next())
    {
        OrderData o;
        int i = -1;
        o.id = q->value(++i).toString();
        o.ClosedTime = QDateTime::fromString(q->value(++i).toString(), DATETIME_FORMAT);
        o.TableName = q->value(++i).toString();
        o.WaiterName = q->value(++i).toString();
        o.Amount = q->value(++i).toDouble();
        m_Data.push_back(o);

        QListWidgetItem *wi = new QListWidgetItem(m_pOrders);
        wi->setSizeHint(QSize(100, 60));

        total_amount += o.Amount;
    }
    delete q;
    m_db.close();

    m_pTotal->setText(QString("%1 / %2").arg(m_pOrders->count()).arg(total_amount));
}

void QCashDialog::OnDown()
{
    m_pOrders->verticalScrollBar()->setValue(m_pOrders->verticalScrollBar()->value() + 500);
}

void QCashDialog::OnUp()
{
    m_pOrders->verticalScrollBar()->setValue(m_pOrders->verticalScrollBar()->value() - 500);
}

void QCashDialog::OnListClicked(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (!m_pOrders->selectedItems().count())
        return;

    QList<QListWidgetItem *> l = m_pOrders->selectedItems();

    QOrderDialog *o = new QOrderDialog(this);
    o->m_cashMode = true;
    o->loadOrder(m_Data.at(m_pOrders->row(l[0])).id);
    o->exec();
    delete o;
}

QCashDialog::QOrdersItemDelegate::QOrdersItemDelegate(const QVector<QCashDialog::OrderData> &data) :
    m_data(data)
{
}

void QCashDialog::QOrdersItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor((option.state & QStyle::State_Selected ? Qt::green : Qt::white));
    painter->setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    QTextOption to;
    to.setAlignment(Qt::AlignVCenter);

    QRect r = option.rect;
    painter->fillRect(r, qRgb(255, 255, 255));
    r.adjust(2, 0, 0, 0);
    painter->drawText(r, m_data[index.row()].id, to);

    r.adjust(100, 0, 0, 0);
    painter->fillRect(r, qRgb(255, 240, 255));
    painter->drawText(r, m_data[index.row()].ClosedTime.toString(DATETIME_FORMAT), to);

    r.adjust(180, 0, 0, 0);
    painter->fillRect(r, qRgb(255, 255, 255));
    painter->drawText(r, m_data[index.row()].TableName, to);

    r.adjust(100, 0, 0, 0);
    painter->fillRect(r, qRgb(255, 240, 255));
    painter->drawText(r, m_data[index.row()].WaiterName, to);

    r.adjust(250, 0, 0, 0);
    painter->fillRect(r, qRgb(255, 255, 255));
    painter->drawText(r, QString::number(m_data[index.row()].Amount), to);
}
