#include "mainwidget.h"
#include "dlgreadcard.h"
#include <QMessageBox>
#include "../All/qnumpadwidget.h"
#include "../All/econnection.h"
#include "../All/qprint.h"
#include "data.h"
#include "change.h"
#include <QSqlError>

MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint), u(DB)
{
    //Left column
    QVBoxLayout *plLeft = new QVBoxLayout();
    plLeft->setMargin(1);
    m_pwdtLeft = new QWidget(this);
    m_pwdtLeft->setMaximumWidth(180);

    QHBoxLayout *plLeftButtons = new QHBoxLayout();
    plLeftButtons->setMargin(0);
    QWidget *pwdtLeftButtons = new QWidget(this);
    pbtnLeftScrollDown = new QPushButton(this);
    pbtnLeftScrollDown->setIcon(QPixmap(":/icons/down.png"));
    pbtnLeftScrollDown->setIconSize(BTN_SQUARE);
    connect(pbtnLeftScrollDown, SIGNAL(clicked()), SLOT(slotScrollGoodsGroupsDown()));
    pbtnLeftScrollUp = new QPushButton(this);
    pbtnLeftScrollUp->setIcon(QPixmap(":/icons/up.png"));
    pbtnLeftScrollUp->setIconSize(BTN_SQUARE);
    connect(pbtnLeftScrollUp, SIGNAL(clicked()), SLOT(slotScrollGoodsGroupsUp()));
    plLeftButtons->addWidget(pbtnLeftScrollDown);
    plLeftButtons->addWidget(pbtnLeftScrollUp);
    plLeftButtons->addStretch(1);
    pwdtLeftButtons->setLayout(plLeftButtons);

    m_pGoodsGroup = new QListWidget(this);
    connect(m_pGoodsGroup, SIGNAL(clicked(QModelIndex)), SLOT(slotFilterGoods(QModelIndex)));
    plLeft->addWidget(m_pGoodsGroup);
    plLeft->addWidget(pwdtLeftButtons);
    m_pwdtLeft->setLayout(plLeft);


    //Middle column
    QVBoxLayout *plMiddle = new QVBoxLayout();
    plMiddle->setMargin(1);
    QWidget *pwdtMiddle = new QWidget(this);

    QHBoxLayout *plMiddleButtons = new QHBoxLayout();
    plMiddleButtons->setMargin(0);
    QWidget *pwdtMiddleButtons = new QWidget(this);
    pbtnMiddleScrollDown = new QPushButton(this);
    pbtnMiddleScrollDown->setIcon(QPixmap(":/icons/down.png"));
    pbtnMiddleScrollDown->setIconSize(BTN_SQUARE);
    connect(pbtnMiddleScrollDown, SIGNAL(clicked()), SLOT(slotScrollGoodsDown()));
    pbtnMiddleScrollUp = new QPushButton(this);
    pbtnMiddleScrollUp->setIcon(QPixmap(":/icons/up.png"));
    pbtnMiddleScrollUp->setIconSize(BTN_SQUARE);
    connect(pbtnMiddleScrollUp, SIGNAL(clicked()), SLOT(slotScrollGoodsUp()));
    plMiddleButtons->addWidget(pbtnMiddleScrollDown);
    plMiddleButtons->addWidget(pbtnMiddleScrollUp);
    plMiddleButtons->addStretch(1);
    pwdtMiddleButtons->setLayout(plMiddleButtons);

    m_pGoods = new QTableWidget(this);
    m_pGoods->horizontalHeader()->hide();
    m_pGoods->verticalHeader()->hide();
    m_pGoods->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pGoods->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_pGoods, SIGNAL(clicked(QModelIndex)), SLOT(slotGoodsClick(QModelIndex)));
    connect(m_pGoods, SIGNAL(clicked(QModelIndex)), SLOT(slotEnableServiceCheckBtn()));
    connect(m_pGoods, SIGNAL(clicked(QModelIndex)), SLOT(slotEnablePaymentBtn()));

    plMiddle->addWidget(m_pGoods);
    plMiddle->addWidget(pwdtMiddleButtons);
    pwdtMiddle->setLayout(plMiddle);

    //Right column
    QVBoxLayout *plRight = new QVBoxLayout();
    plRight->setMargin(1);
    QWidget *pwdtRight = new QWidget(this);
    pwdtRight->setMaximumWidth(300);

    QNumpadWidget *pNumpad = new QNumpadWidget(this);
    pNumpad->m_pInput->setEchoMode(QLineEdit::Normal);
    connect(pNumpad, SIGNAL(Finish(QString)), SLOT(slotChangeGoodsQuantity(QString)));
    QPushButton *pbtnHalf = new QPushButton("0.5", this);
    pbtnHalf->setMinimumHeight(50);
    connect(pbtnHalf, SIGNAL(clicked()), SLOT(slotHalfBtnClick()));

    m_pOrder =  new QListWidget(this);
    m_pOrder->setMinimumHeight(250);
    m_pOrder->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pOrder->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *plRightButtons1 = new QHBoxLayout();
    plRightButtons1->setMargin(0);
    QWidget *pwdtRightButtons1 = new QWidget(this);
    pbtnRightScrollDown = new QPushButton(this);
    pbtnRightScrollDown->setIcon(QPixmap(":/icons/down.png"));
    pbtnRightScrollDown->setIconSize(BTN_SQUARE);
    connect(pbtnRightScrollDown, SIGNAL(clicked()), SLOT(slotScrollOrderDown()));
    pbtnRightScrollUp = new QPushButton(this);
    pbtnRightScrollUp->setIcon(QPixmap(":/icons/up.png"));
    pbtnRightScrollUp->setIconSize(BTN_SQUARE);
    connect(pbtnRightScrollUp, SIGNAL(clicked()), SLOT(slotScrollOrderUp()));
    QPushButton *pbtnMenu = new QPushButton(this);
    pbtnMenu->setIcon(QPixmap(":/icons/menu.png"));
    pbtnMenu->setIconSize(BTN_SQUARE);
    QPushButton *pbtnRightDelete = new QPushButton(this);
    pbtnRightDelete->setIcon(QPixmap(":/icons/delete.png"));
    pbtnRightDelete->setIconSize(BTN_SQUARE);
    connect(pbtnRightDelete, SIGNAL(clicked()), SLOT(slotDeleteGoodsClick()));
    connect(pbtnRightDelete, SIGNAL(clicked()), SLOT(slotEnableServiceCheckBtn()));
    connect(pbtnRightDelete, SIGNAL(clicked()), SLOT(slotEnablePaymentBtn()));
    plRightButtons1->addWidget(pbtnRightScrollDown);
    plRightButtons1->addWidget(pbtnRightScrollUp);
    plRightButtons1->addWidget(pbtnMenu);
    plRightButtons1->addWidget(pbtnRightDelete);
    pwdtRightButtons1->setLayout(plRightButtons1);

    m_pOrderAmount =  new QLineEdit(this);
    m_pOrderAmount->setEnabled(false);
    QFont amountFont(qApp->font());
    amountFont.setPointSize(20);
    m_pOrderAmount->setFont(amountFont);
    m_pOrderAmount->setAlignment(Qt::AlignCenter);
    m_pOrderAmount->setText("0");

    QHBoxLayout *plPrintButtons = new QHBoxLayout();
    plPrintButtons->setMargin(0);
    QWidget *pwdtPrintButtons = new QWidget(this);
    pbtnServiceCheck = new QPushButton(this);
    pbtnServiceCheck->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pbtnServiceCheck->setIcon(QPixmap(":/icons/service_check.png"));
    pbtnServiceCheck->setIconSize(BTN_SQUARE);
    connect(pbtnServiceCheck, SIGNAL(clicked()), SLOT(slotServiceBtnClick()));
    pbtnPayment = new QPushButton(this);
    pbtnPayment->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pbtnPayment->setIcon(QPixmap(":/icons/payment.png"));
    pbtnPayment->setIconSize(BTN_SQUARE);
    connect(pbtnPayment, SIGNAL(clicked()), SLOT(slotPaymentButtonClick()));
    plPrintButtons->addWidget(pbtnServiceCheck);
    plPrintButtons->addWidget(pbtnPayment);
    pwdtPrintButtons->setLayout(plPrintButtons);

    QPushButton *pbtnCard = new QPushButton(this);
    pbtnCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pbtnCard->setIcon(QIcon(":/icons/credit-card.png"));
    pbtnCard->setIconSize(BTN_SQUARE);
    connect(pbtnCard, SIGNAL(clicked()), this, SLOT(btnCardClicked()));
    plPrintButtons->addWidget(pbtnCard);

    QPushButton *pbtnDiscountCard = new QPushButton(this);
    pbtnDiscountCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pbtnDiscountCard->setIcon(QIcon(":/icons/discount.png"));
    pbtnDiscountCard->setIconSize(BTN_SQUARE);
    connect(pbtnDiscountCard, SIGNAL(clicked()), this, SLOT(btnDiscountClicked()));
    plPrintButtons->addWidget(pbtnDiscountCard);


    QHBoxLayout *plRightButtons2 = new QHBoxLayout();
    plRightButtons2->setMargin(0);
    QWidget *pwdtRightButtons2 = new QWidget(this);
    QPushButton *pbtnQuit = new QPushButton(this);
    pbtnQuit->setIcon(QPixmap(":/icons/exit.png"));
    pbtnQuit->setIconSize(BTN_SQUARE);
    connect(pbtnQuit, SIGNAL(clicked()), SLOT(close()));
    plRightButtons2->addWidget(pbtnQuit);

    QPushButton *btnCheckCard = new QPushButton(this);
    btnCheckCard->setIcon(QPixmap(":/icons/credit-card.png"));
    btnCheckCard->setIconSize(BTN_SQUARE);
    connect(btnCheckCard, SIGNAL(clicked()), this, SLOT(btnCheckCard()));
    plRightButtons2->addWidget(btnCheckCard);

    pwdtRightButtons2->setLayout(plRightButtons2);

    plRight->addWidget(pNumpad);
    plRight->addWidget(pbtnHalf);
    plRight->addWidget(m_pOrder);
    plRight->addWidget(m_pOrderAmount);
    plRight->addWidget(pwdtRightButtons1);
    plRight->addWidget(pwdtPrintButtons);
    plRight->addStretch(1);
    plRight->addWidget(pwdtRightButtons2);
    pwdtRight->setLayout(plRight);

    QHBoxLayout *plMain = new QHBoxLayout();
    plMain->setMargin(1);
    plMain->addWidget(m_pwdtLeft);
    QPushButton *pbtnHideGoodsGroups = new QPushButton("<<<", this);
    pbtnHideGoodsGroups->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pbtnHideGoodsGroups, SIGNAL(clicked()), SLOT(slotShowGoodsGroups()));

    pbtnHideGoodsGroups->setMaximumWidth(40);
    plMain->addWidget(pbtnHideGoodsGroups);
    plMain->addWidget(pwdtMiddle);
    plMain->addWidget(pwdtRight);
    setLayout(plMain);

    QFont font(qApp->font());
    font.setPointSize(12);
    setFont(font);
}

void MainWidget::slotFilterGoods(QModelIndex index)
{
    if (!index.isValid())
        return;
    m_GoodsFilter = m_pGoodsGroup->item(index.row())->text();
    buildGoodsView();
}

void MainWidget::slotShowGoodsGroups()
{
    QPushButton *pbtn = qobject_cast<QPushButton *>(sender());
    if (pbtn->text() == "<<<")
    {
        pbtn->setText(">>>");
        m_pwdtLeft->setVisible(false);
    }
    else
    {
        pbtn->setText("<<<");
        m_pwdtLeft->setVisible(true);
    }
    qApp->processEvents();
    buildGoodsView();
}

void MainWidget::slotScrollGoodsGroupsDown()
{
    m_pGoodsGroup->verticalScrollBar()->setValue(m_pGoodsGroup->verticalScrollBar()->value() + 5);
}

void MainWidget::slotScrollGoodsGroupsUp()
{
    m_pGoodsGroup->verticalScrollBar()->setValue(m_pGoodsGroup->verticalScrollBar()->value() + 5);
}

void MainWidget::slotScrollGoodsDown()
{
    m_pGoods->verticalScrollBar()->setValue(m_pGoods->verticalScrollBar()->value() + 5);
}

void MainWidget::slotScrollGoodsUp()
{
    m_pGoods->verticalScrollBar()->setValue(m_pGoods->verticalScrollBar()->value() - 5);
}

void MainWidget::slotScrollOrderDown()
{
    m_pOrder->verticalScrollBar()->setValue(m_pOrder->verticalScrollBar()->value() + 5);
}

void MainWidget::slotScrollOrderUp()
{
    m_pOrder->verticalScrollBar()->setValue(m_pOrder->verticalScrollBar()->value() - 5);
}

void MainWidget::slotGoodsClick(QModelIndex index)
{
    if (!index.isValid())
        return;
    if (!m_pGoods->item(index.row(), index.column()))
        return;
    int goods_index = m_pGoods->item(index.row(), index.column())->data(Qt::UserRole).toInt();
    addGoodsToOrder(m_Goods[goods_index]);
}

void MainWidget::slotDeleteGoodsClick()
{
    removeGoodsFromOrder(m_pOrder->currentRow()); 
}

void MainWidget::slotChangeGoodsQuantity(QString quantity)
{
    int goods_index = m_pOrder->currentRow();
    if ((goods_index < 0) || (goods_index > m_pOrder->count() - 1))
        return;
    if (!quantity.length())
        return;
    if (quantity.toDouble() < 0.001)
        return;
    if (quantity.toDouble() > 100)
        return;

    m_OrderGoods[goods_index].quantity = quantity.toDouble();
    QListWidgetItem *w = m_pOrder->item(goods_index);
    w->setText(QString("%1\n%2  X  %3").arg(m_OrderGoods[goods_index].name).arg(m_OrderGoods[goods_index].quantity).arg(m_OrderGoods[goods_index].price));
    countOrderAmount();
}

void MainWidget::slotPaymentButtonClick()
{
    paymentClick();
    slotEnablePaymentBtn();
    slotEnableServiceCheckBtn();
    return;
    //Old code
    change *c = new change(m_OrderData[ORDER_AMOUNT].toDouble(), this);
    if (c->exec() == QDialog::Accepted) {
        paymentClick();
        slotEnablePaymentBtn();
        slotEnableServiceCheckBtn();
    }
    delete c;
}

void MainWidget::slotEnableServiceCheckBtn()
{
    pbtnServiceCheck->setEnabled(m_pOrder->count() > 0);
}

void MainWidget::slotEnablePaymentBtn()
{
    pbtnPayment->setEnabled(m_pOrder->count() > 0);
}

void MainWidget::slotServiceBtnClick()
{
    return;
    servicePrintBegin();
    slotEnablePaymentBtn();
    slotEnableServiceCheckBtn();
}

void MainWidget::slotHalfBtnClick()
{
    slotChangeGoodsQuantity("0.5");
}

void MainWidget::btnCardClicked()
{

    QString code;
    if (DlgReadCard::cardCode(code)) {
        QStringList paramNames;
        paramNames << "cardcode";
        QVector<QVariant> paramValues;
        paramValues << code;
        QSqlQuery *q = DB.QuerySelect("select val/100, id from cards_append where cardcode=:cardcode", paramNames, paramValues);
        if (!q) {
            QMessageBox::critical(this, tr("SQL error"), tr("Unknown card"));
            return;
        }
        if (q->next()) {
            fCard = code;
            fCardValue = q->value(0).toDouble();
            fCardId = q->value(1).toInt();
            countOrderAmount();
        } else {
            QMessageBox::critical(this, tr("Card error"), tr("Unknown card"));
        }
        delete q;
    }
}

void MainWidget::btnDiscountClicked()
{

    QString code;
    if (DlgReadCard::cardCode(code)) {
        QStringList paramNames;
        paramNames << "cardcode";
        QVector<QVariant> paramValues;
        paramValues << code;
        QSqlQuery *q = DB.QuerySelect("select amount, id from cards_append where cardcode=:cardcode", paramNames, paramValues);
        if (!q) {
            QMessageBox::critical(this, tr("SQL error"), tr("Unknown card"));
            return;
        }
        if (q->next()) {
            fCard = code;
            fCardValue = q->value(0).toDouble() * -1;
            fCardId = q->value(1).toInt();
            countOrderAmount();
        } else {
            QMessageBox::critical(this, tr("Card error"), tr("Unknown card"));
        }
        delete q;
    }
}

void MainWidget::btnCheckCard()
{
    QString code;
    if (DlgReadCard::cardCode(code)) {
        QStringList paramNames;
        paramNames << "cardcode";
        QVector<QVariant> paramValues;
        paramValues << code;
        QSqlQuery *q = DB.QuerySelect("select amount, id from cards_append where cardcode=:cardcode", paramNames, paramValues);
        if (!q) {
            QMessageBox::critical(this, tr("SQL error"), tr("Unknown card"));
            return;
        }
        if (q->next()) {
            QMessageBox::information(this, tr("Card amount"), q->value(0).toString());
        } else {
            QMessageBox::critical(this, tr("Card error"), tr("Unknown card"));
        }
        delete q;
    }
}


void MainWidget::buildMenu()
{
    QString sql = "SELECT DISTINCT(DG.NAME) FROM DISHES D, DISHES_GROUPS DG "
            "WHERE D.GROUP_ID=DG.ID AND D.MENU_ID=:MENU_ID AND D.VISIBLE=1 ORDER BY DG.NAME ";
    QStringList paramNames;
    paramNames << "MENU_ID";
    QVector<QVariant> paramValues;
    paramValues << Data::settings[SETTINGS_DEFAULT_MENU];
    m_GoodsGroups.clear();
    m_GoodsGroups.push_back("*");
    QSqlQuery *q = DB.QuerySelect(sql, paramNames, paramValues);
    if (q)
    {
        while (q->next())
            m_GoodsGroups.push_back(q->value(0).toString());
        delete q;
    }

    sql = "SELECT D.ID, D.NAME AS DISH_NAME, DG.NAME AS GROUP_NAME, D.PRICE, D.COLOR, "
            "D.PRINTER_ID, D.PRINT_QUEUE, D.STORE_ID "
            "FROM DISHES D, DISHES_GROUPS DG "
            "WHERE D.GROUP_ID=DG.ID AND D.MENU_ID=:MENU_ID AND D.VISIBLE=1 "
            "ORDER BY D.QUEUE ";
    m_Goods.clear();
    q = DB.QuerySelect(sql, paramNames, paramValues);
    if (q)
    {
        while (q->next())
        {
            Goods g;
            int i = -1;
            g.id = q->value(++i).toInt();
            g.name = q->value(++i).toString();
            g.group = q->value(++i).toString();
            g.quantity = 1;
            g.price = q->value(++i).toDouble();
            g.color = q->value(++i).toInt();
            g.printSchema = q->value(++i).toInt();
            g.printQueue = q->value(++i).toInt();
            g.store = q->value(++i).toInt();
            m_Goods.push_back(g);
        }
        delete q;
    }

    sql = "SELECT PSD.SCHEMA_ID, PAD.PRINTER_NAME "
            "FROM PRINTER_SCHEMA_DETAILS PSD, PRINTER_ALIASES_DETAILS PAD "
            "WHERE PAD.ALIAS_PRINTER_ID=PSD.PRINTER_ID AND PAD.ALIAS_SCHEMA_ID=:ALIAS_SCHEMA_ID";
    paramNames.clear();
    paramNames << "ALIAS_SCHEMA_ID";
    paramValues.clear();
    paramValues << Data::settings[SETTINGS_PRINTER_ALIAS].toInt();
    m_PrinterSchema.clear();
    q = DB.QuerySelect(sql, paramNames, paramValues);
    if (q)
    {
        m_PrinterSchema.clear();
        while (q->next())
        {
            if (!m_PrinterSchema.contains(q->value(0).toInt()))
                m_PrinterSchema.insert(q->value(0).toInt(), QVector<QString>());
            m_PrinterSchema[q->value(0).toInt()].push_back(q->value(1).toString());
        }
        delete q;
    }

    q = DB.QuerySelect("select val from cards_nodiscount_dishes");
    if (q) {
        if (q->next()) {
            m_GoodsNoDiscount = q->value(0).toString().split(",");
        }
        delete q;
    }

    DB.db.close();

    buildGoodsGroupView();
    m_GoodsFilter = "*";
    buildGoodsView();
    setOrderScrollEnabled();
    slotEnableServiceCheckBtn();
    slotEnablePaymentBtn();

}

void MainWidget::buildGoodsGroupView()
{
    for (QStringList::const_iterator i = m_GoodsGroups.begin(); i != m_GoodsGroups.end(); i++)
    {
        QListWidgetItem *w = new QListWidgetItem(m_pGoodsGroup);
        w->setText(*i);
        w->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        w->setSizeHint(QSize(100, 50));
    }

    bool scroll_btn = 50 * m_pGoodsGroup->count() > m_pGoodsGroup->height();
    pbtnLeftScrollDown->setEnabled(scroll_btn);
    pbtnLeftScrollUp->setEnabled(scroll_btn);
}

void MainWidget::buildGoodsView()
{
    qApp->processEvents();
    m_pGoods->clear();

    QSize cell_size = TABLE_GOODS_CELL;
    int col_count = (m_pGoods->width() - 10) / cell_size.width();
    if (!col_count)
        return;
    if (m_pGoods->width() - 10 > col_count * cell_size.width())
    {
        int mod = (m_pGoods->width() - 10) - (col_count * cell_size.width());
        cell_size.setWidth(cell_size.width() + (mod / col_count));
    }
    int off_col = 0, row = 0;
    m_pGoods->setRowCount(1);
    m_pGoods->setRowHeight(0, cell_size.height());
    m_pGoods->setColumnCount(col_count);
    for (int i = 0; i < col_count; i++)
        m_pGoods->setColumnWidth(i, cell_size.width());

    for (int i = 0; i < m_Goods.count(); i++)
    {
        if (m_GoodsFilter != "*")
            if (m_GoodsFilter != m_Goods[i].group)
                continue;

        QTableWidgetItem *w = new QTableWidgetItem();
        w->setText(m_Goods[i].name + " [" + QString::number(m_Goods[i].price) + "]");
        w->setData(Qt::UserRole, i);
        w->setBackgroundColor(m_Goods[i].color);
        w->setFlags(w->flags() ^ Qt::ItemIsEditable);

        if (off_col >= col_count)
        {
            off_col = 0;
            m_pGoods->setRowCount((++row) + 1);
            m_pGoods->setRowHeight(row, cell_size.height());
        }

        m_pGoods->setItem(row, off_col, w);
        off_col++;
    }

    bool scroll_btn = cell_size.height() * m_pGoods->rowCount() > m_pGoods->height();
    pbtnMiddleScrollDown->setEnabled(scroll_btn);
    pbtnMiddleScrollUp->setEnabled(scroll_btn);
}

void MainWidget::addGoodsToOrder(const MainWidget::Goods &goods)
{
    Goods g = goods;
    m_OrderGoods.push_back(g);

    QListWidgetItem *w = new QListWidgetItem(m_pOrder);
    w->setText(QString("%1\n%2  X  %3").arg(g.name).arg(g.quantity).arg(g.price));
    w->setSizeHint(QSize(100, 45));
    w->setData(Qt::UserRole, m_pOrder->count() - 1);

    m_pOrder->setCurrentRow(m_pOrder->count() - 1);
    countOrderAmount();
    setOrderScrollEnabled();
}

void MainWidget::removeGoodsFromOrder(const int &index)
{
    if (index < 0)
        return;
    if (index > m_pOrder->count() - 1)
        return;

    QListWidgetItem *w = m_pOrder->item(index);
    delete w;
    m_OrderGoods.remove(index);

    countOrderAmount();
    setOrderScrollEnabled();
    m_pOrder->setCurrentRow(0);
}

void MainWidget::countOrderAmount()
{
    double amount = 0;
    fCardAmount = 0;
    for (QVector<Goods>::const_iterator i = m_OrderGoods.begin(); i != m_OrderGoods.end(); i++) {
        amount += i->quantity * i->price;
        if (!fCard.isEmpty()) {
            if (fCardValue > 0.01) {
                if (!m_GoodsNoDiscount.contains(QString::number(i->id))) {
                    fCardAmount += ((i->quantity * i->price) * fCardValue);
                }
            }
        }
    }
    if (fCardValue < 0.01) {
        if (fCardValue > amount) {
            fCardAmount = amount;
            amount = 0;
        }
        if (fCardValue <= amount) {
            amount += fCardValue;
            fCardAmount = fCardValue;
        }
    }
    m_pOrderAmount->setText(tr("Bill: ") + QString::number(amount));
    if (fCardAmount > 0.1) {
        m_pOrderAmount->setText(m_pOrderAmount->text() + " Card: " + QString::number(fCardAmount, 'f', 0));
    }

    if (Data::settings[SETTINGS_COMPORT].toString().length())
    {
        QString com_str = "Total: " + m_pOrderAmount->text();
        if (amount <= 0)
            com_str = "Welcome!";

        ComPort comport(Data::settings[SETTINGS_COMPORT].toString().toLatin1().data());
        DWORD *dataWrited = new DWORD;
        int port_str_len = Data::settings[SETTINGS_COMPORT_STRING_LENGTH].toInt();
        char *port_str = new char [port_str_len];
        memset(port_str, 32, port_str_len);
        for (int i = 0; i < com_str.length(); i++)
            port_str[(i + port_str_len) % port_str_len] = com_str.at(i).toLatin1();
        comport.Write(port_str, port_str_len, dataWrited);
    }

    m_OrderData[ORDER_AMOUNT] = amount;
}

void MainWidget::setOrderScrollEnabled()
{
    bool scroll_btn = 45 * m_pOrder->count() > m_pOrder->height();
    pbtnRightScrollDown->setEnabled(scroll_btn);
    pbtnRightScrollUp->setEnabled(scroll_btn);
}

void MainWidget::newOrder()
{

}

void MainWidget::writeOrder()
{
    if (!m_OrderData.contains(ORDER_ID))
        m_OrderData.insert(ORDER_ID, QString("%1%2").arg(Data::settings[SETTINGS_ID_PREFIX].toString()).arg(DB.GetID("GEN_SALE_HEADER_ID")));
    m_OrderData.insert(ORDER_DATE_CASH, Data::cashDate());
    m_OrderData.insert(ORDER_STATUS_ID, ORDER_STATUS_CLOSED);
    m_OrderData.insert(ORDER_TABLE_ID, Data::settings[SETTINGS_TABLE_ID]);
    m_OrderData.insert(ORDER_WAITER_ID, u.Id);

    QStringList fields;
    QVector<QVariant> values;
    fields << "ID" << "STATUS_ID" << "DATE_CASH" << "TABLE_ID" << "WAITER_ID" << "AMOUNT";
    values << m_OrderData[ORDER_ID] << m_OrderData[ORDER_STATUS_ID] << m_OrderData[ORDER_DATE_CASH]
              << m_OrderData[ORDER_TABLE_ID] << m_OrderData[ORDER_WAITER_ID] << m_OrderData[ORDER_AMOUNT];
    DB.QueryInsert("SALE_HEADER", fields, values);
    fields.clear();
    values.clear();

    if (!fCard.isEmpty()) {
        QStringList paramNames;
        paramNames << "amount" << "cardcode";
        QVector<QVariant> paramValues;
        paramValues << fCardAmount << fCard;
        QSqlQuery *q = DB.QuerySelect("update cards_append set amount=amount+:amount where cardcode=:cardcode", paramNames, paramValues);
        if (!q) {
            QMessageBox::critical(this, tr("SQL Error"), q->lastError().databaseText());
        }
        delete q;
        paramNames.clear();
        paramNames << "cardid" << "orderid" << "val";
        paramValues.clear();
        paramValues << fCardId << m_OrderData[ORDER_ID] << fCardAmount;
        DB.QueryInsert("cards_history", paramNames, paramValues);
    }

    fields << "ORDER_ID" << "DISH_ID" << "QTY" << "PRICE" << "STORE_ID" << "PRINTED";
    for (QVector<Goods>::iterator i = m_OrderGoods.begin(); i != m_OrderGoods.end(); i++)
    {
        if (!i->record_id.length())
        {
            QStringList n;
            n << "ID";
            QVector<QVariant> v;
            i->record_id = Data::settings[SETTINGS_ID_PREFIX].toString() + QString::number(DB.GetID("GEN_SALE_BODY_ID"));
            v << i->record_id;
            DB.QueryInsert("SALE_BODY", n, v);
        }

        values.clear();
        values << m_OrderData[ORDER_ID] << i->id << i->quantity << i->price
                  << i->store << i->printedQuantity;
        DB.QueryUpdate("SALE_BODY", fields, values, "ID", i->record_id);
    }

    fields.clear();
    values.clear();
    fields << "ORDER_ID" << "MOD_ID" << "DATA";
    values << m_OrderData[ORDER_ID] << MOD_PAYMENT_DATETIME << QDateTime::currentDateTime().toString(Data::settings[SETTINGS_DATETIME_FORMAT].toString());
    DB.QueryInsert("SALE_MODS", fields, values);
}

void MainWidget::clearOrder()
{
    fCardValue = 0.0;
    fCardAmount = 0.0;
    fCard.clear();
    m_OrderData.clear();
    m_OrderGoods.clear();
    m_pOrder->clear();
    setOrderScrollEnabled();
    countOrderAmount();
}

void MainWidget::paymentClick()
{
    writeOrder();
    //Option 1. Print payment check and continue with new order
    paymentPrint();
    clearOrder();
}

void MainWidget::serviceCheckClick()
{
}

void MainWidget::servicePrintBegin()
{
    // 1. Get queue
    // 2. Get printers
    // 3. Print by queue
    QVector<int> print_queue;
    QVector<QString> printers;
    for (QVector<Goods>::const_iterator i = m_OrderGoods.begin(); i != m_OrderGoods.end(); i++)
    {
        if (!print_queue.contains(i->printQueue))
            print_queue.push_back(i->printQueue);

        if (m_PrinterSchema.contains(i->printSchema))
        {
            for (QVector<QString>::const_iterator j = m_PrinterSchema[i->printSchema].begin(); j != m_PrinterSchema[i->printSchema].end(); j++)
                if (!printers.contains(*j))
                    printers.push_back(*j);
        }
    }

    std::sort(print_queue.begin(), print_queue.end());
    for (QVector<int>::const_iterator i = print_queue.begin(); i != print_queue.end(); i++)
        for (QVector<QString>::const_iterator j = printers.begin(); j != printers.end(); j++)
        {
            QVector<Goods> goods_to_print;
            for (QVector<Goods>::const_iterator k = m_OrderGoods.begin(); k != m_OrderGoods.end(); k++)
                if ((k->printQueue == *i) && (m_PrinterSchema[k->printSchema].contains(*j)))
                    goods_to_print.push_back(*k);
            servicePrint(*j, goods_to_print);
        }
}

void MainWidget::servicePrint(const QString &printer, QVector<Goods> goods)
{
    QPrint p(printer);
    for (QVector<Goods>::const_iterator i = goods.begin(); i != goods.end(); i++)
        QMessageBox::information(this, "", i->name);
}

void MainWidget::paymentPrint() {

    QFont f(qApp->font());
    QPen pen;
    QPrint p(Data::settings[SETTINGS_PRINTER_PAYMENT].toString());
    int left = 5, width = Data::settings[SETTINGS_PRINTER_PAPER_WIDTH].toInt(), top = 5;

    if (QFile::exists(Data::homeDir + "/payment_logo.png"))
    {
        QStringList log_size = Data::settings[SETTINGS_PRINT_LOGO_SIZE].toString().split(" ");
        p.addCommand(new QPCBitmap(QRect(log_size[0].toInt(), log_size[1].toInt(), log_size[2].toInt(), log_size[3].toInt()), Data::homeDir + "/payment_logo.png"));
        top += log_size[3].toInt() + 2;
    }

    p.addCommand(new QPCSetFont(f));
    p.addCommand(new QPCText(Data::settings[SETTINGS_PRINT_PAYMENT_HEADER].toString(), QPoint(left, top)));
    top += (p.rscale(QFontMetrics(f).height()) / 2);
    pen.setWidth(2);
    p.addCommand(new QPCSetPen(pen));
    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top += 2;
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    p.addCommand(new QPCSetPen(pen));
    for (QVector<Goods>::const_iterator i = m_OrderGoods.begin(); i < m_OrderGoods.end(); i++)
    {
        top += p.rscale(QFontMetrics(f).height());
        p.addCommand(new QPCText(i->name, QPoint(left, top)));
        p.addCommand(new QPCText(QString::number(i->quantity), QPoint((left + 15) - p.rscale(QFontMetrics(f).width(QString::number(i->quantity))), (top += p.rscale(QFontMetrics(f).height()) + 1))));
        p.addCommand(new QPCText(" X ", QPoint(left + 16, top)));
        p.addCommand(new QPCText(QString::number(i->price), QPoint(left + 17 + p.rscale(QFontMetrics(f).width(" X ")), top)));
        p.addCommand(new QPCText(QString::number(i->quantity * i->price) , QPoint(width - p.rscale(QFontMetrics(f).width(QString::number(i->price))) - 5, top)));
        top ++;
        p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    }
    top += 2;
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    p.addCommand(new QPCSetPen(pen));
    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    f.setPointSize(16);
    f.setBold(true);
    p.addCommand(new QPCSetFont(f));
    top += p.rscale(QFontMetrics(f).height());
    p.addCommand(new QPCText(tr("Total"), QPoint(left, top)));
    QString total_amount = QString::number(m_OrderData[ORDER_AMOUNT].toDouble());
    p.addCommand(new QPCText(total_amount, QPoint(width - p.rscale(QFontMetrics(f).width(total_amount)) - 5, top)));
    top += p.rscale(QFontMetrics(f).height());

    f.setBold(false);
    f.setPointSize(qApp->font().pointSize());
    p.addCommand(new QPCSetFont(f));
    p.addCommand(new QPCText(tr("Cashier") + ": " + u.Name, QPoint(left, top)));
    top += (p.rscale(QFontMetrics(f).height()) * 2);
    p.addCommand(new QPCText(Data::settings[SETTINGS_PRINT_PAYMENT_BOTTOM].toString(), QPoint(left, top)));
    top += p.rscale(QFontMetrics(f).height());
    f.setPointSize(8);
    p.addCommand(new QPCSetFont(f));
    p.addCommand(new QPCText(tr("Printed") + ": " + QDateTime::currentDateTime().toString(Data::settings[SETTINGS_DATETIME_FORMAT].toString()), QPoint(left, top)));
    p.beginPrint();
}

