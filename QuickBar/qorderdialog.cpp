#include "qorderdialog.h"
#include "../All/qnumpadwidget.h"
#include "../ALL/qprint.h"
#include "qmenuwindow.h"
#include "qmessagedialog.h"
#include "qhalldialog.h"
#include "qmoveorderdialog.h"
#include "qpassworddialog.h"
#include "qcostumdishdialog.h"
#include "../ALL/global.h"

QOrderDialog::QOrderDialog(QWidget *parent) :
    QBaseDialog(tr("Order"), parent)
{
    //Left side
    m_pMenu = new QListWidget(this);
    m_pMenu->setItemDelegate(new QMenuItemDelegate());
    m_porder = new QOrder(m_db);

    QHBoxLayout *layoutMenuButtons = new QHBoxLayout();
    layoutMenuButtons->setMargin(1);
    QWidget *pwdtMenuUpDown = new QWidget(this);
    QPushButton *pbtnMenuUp = qpushbutton("");
    pbtnMenuUp->setIcon(QPixmap(":/icons/up.png"));
    QPushButton *pbtnMenuDown  = qpushbutton("");
    pbtnMenuDown->setIcon(QPixmap(":/icons/down.png"));
    QPushButton *pbtnSwitchDish = qpushbutton("");
    pbtnSwitchDish->setIcon(QPixmap(":/icons/dishes.png"));
    QPushButton *pbtnSwitchGroup = qpushbutton("");
    pbtnSwitchGroup->setIcon(QPixmap(":/icons/groups.png"));
    QPushButton *pbtnSwitchMenu = qpushbutton("");
    pbtnSwitchMenu->setIcon(QPixmap(":/icons/menu.png"));

    layoutMenuButtons->addWidget(pbtnMenuUp);
    layoutMenuButtons->addWidget(pbtnMenuDown);
    layoutMenuButtons->addWidget(pbtnSwitchMenu);
    layoutMenuButtons->addWidget(pbtnSwitchDish);
    layoutMenuButtons->addWidget(pbtnSwitchGroup);
    layoutMenuButtons->addStretch(1);
    pwdtMenuUpDown->setLayout(layoutMenuButtons);

    QWidget *pwdtLeft = new QWidget(this);
    QWidget *pwdtCorrect1 = new QWidget(this);
    pwdtCorrect1->setMinimumHeight(10);
    QVBoxLayout *layoutMainLeft = new QVBoxLayout();
    layoutMainLeft->setMargin(0);
    layoutMainLeft->addWidget(m_pMenu);
    layoutMainLeft->addWidget(pwdtMenuUpDown);
    layoutMainLeft->addWidget(pwdtCorrect1);
    pwdtLeft->setLayout(layoutMainLeft);

    //Right side
    QWidget *pwdtTop = new QWidget(this);
    QHBoxLayout *plTop = new QHBoxLayout();
    plTop->setMargin(0);
    m_pNumpad = new QNumpadWidget(this);
    m_pNumpad->m_pInput->setEchoMode(QLineEdit::Normal);
    m_pInfo = new QListWidget(this);
    m_pInfo->setItemDelegate(new QInfoItemDelegate());
    m_pInfo->setMaximumHeight(m_pNumpad->height());
    plTop->addWidget(m_pNumpad);
    plTop->addWidget(m_pInfo);
    pwdtTop->setLayout(plTop);
    pwdtTop->setMaximumHeight(m_pNumpad->height() + 2);

    m_pOrderItems = new QListWidget(this);
    m_pOrderItems->setItemDelegate(new QOrderItemsDelegate(*m_porder));

    QWidget *pwdtOrderButtons = new QWidget(this);
    QHBoxLayout *plOrderButtons = new QHBoxLayout();
    plOrderButtons->setMargin(1);
    QPushButton *pbtnScrollOrderUp = qpushbutton("");
    pbtnScrollOrderUp->setIcon(QPixmap(":/icons/up.png"));
    QPushButton *pbtnScrollOrderDown = qpushbutton("");
    pbtnScrollOrderDown->setIcon(QPixmap(":/icons/down.png"));
    m_pbtnServiceCheck = qpushbutton("");
    m_pbtnServiceCheck->setIcon(QPixmap(":/icons/print.png"));
    QPushButton *pbtnCloseWindow = qpushbutton("");
    pbtnCloseWindow->setIcon(QPixmap(":/icons/exit.png"));
    QPushButton *pbtnMenuWindow = qpushbutton("");
    pbtnMenuWindow->setIcon(QPixmap(":/icons/menu.png"));
    QPushButton *pbtnDeleteRow = qpushbutton("");
    pbtnDeleteRow->setIcon(QPixmap(":/icons/delete_row.png"));
    plOrderButtons->addWidget(pbtnScrollOrderDown);
    plOrderButtons->addWidget(pbtnScrollOrderUp);
    plOrderButtons->addWidget(pbtnMenuWindow);
    plOrderButtons->addStretch(1);
    plOrderButtons->addWidget(pbtnDeleteRow);
    plOrderButtons->addStretch(1);
    plOrderButtons->addWidget(m_pbtnServiceCheck);
    plOrderButtons->addWidget(pbtnCloseWindow);
    pwdtOrderButtons->setLayout(plOrderButtons);


    QVBoxLayout *layoutMainRight = new QVBoxLayout();
    layoutMainRight->setMargin(0);
    QWidget *pwdtRight = new QWidget(this);
    QWidget *pwdtCorrect2 = new QWidget(this);
    pwdtCorrect2->setMinimumHeight(10);
    layoutMainRight->addWidget(pwdtTop);
    layoutMainRight->addWidget(m_pOrderItems);
    layoutMainRight->addWidget(pwdtOrderButtons);
    layoutMainRight->addWidget(pwdtCorrect2);
    pwdtRight->setLayout(layoutMainRight);

    //Finally
    QHBoxLayout *layoutMain = new QHBoxLayout();
    layoutMain->setMargin(0);
    layoutMain->addWidget(pwdtLeft);
    layoutMain->addWidget(pwdtRight);
    setLayout(layoutMain);

    m_groupFilter = 0;
    m_menuFilter = Data::settings[S_DEFAULT_MENU].toInt();
    m_cashMode = false;
    OnSwitchGroups();

    connect(pbtnSwitchDish, SIGNAL(clicked()), SLOT(OnSwitchDishes()));
    connect(pbtnSwitchGroup, SIGNAL(clicked()), SLOT(OnSwitchGroups()));
    connect(pbtnSwitchMenu, SIGNAL(clicked()), SLOT(OnSwitchMenus()));
    connect(m_pMenu, SIGNAL(clicked(QModelIndex)), SLOT(OnMenuClicked(QModelIndex)));
    connect(pbtnMenuDown, SIGNAL(clicked()), SLOT(OnScrollMenuDown()));
    connect(pbtnMenuUp, SIGNAL(clicked()), SLOT(OnScrollMenuUp()));
    connect(m_pNumpad, SIGNAL(onEditFinish()), SLOT(OnNumpadOk()));
    connect(pbtnScrollOrderDown, SIGNAL(clicked()), SLOT(OnScrollOrderDown()));
    connect(pbtnScrollOrderUp, SIGNAL(clicked()), SLOT(OnScrollOrderUp()));
    connect(m_pbtnServiceCheck, SIGNAL(clicked()), SLOT(OnServiceCheck()));
    connect(pbtnDeleteRow, SIGNAL(clicked()), SLOT(OnDeleteRow()));
    connect(pbtnCloseWindow, SIGNAL(clicked()), SLOT(OnClose()));
    connect(pbtnMenuWindow, SIGNAL(clicked()), SLOT(OnOpenMainMenu()));
}

void QOrderDialog::setTable(const Table &table)
{
    m_porder->m_table = table;
    viewCommonInfo();
}

void QOrderDialog::setUser(const QUser &user)
{
    m_porder->m_CurrentWaiter = user;
    viewCommonInfo();
}

void QOrderDialog::loadOrder(const QString &order_id)
{
    m_porder->loadOrder(order_id);
    viewOrderDishes(0);
}

void QOrderDialog::OnSwitchDishes()
{
    m_viewMode = vmDishes;
    m_pMenu->clear();
    for (QMap<int, Dish>::const_iterator i = Data::dishes.begin(); i != Data::dishes.end(); i++)
    {
        if (m_menuFilter)
            if (i.value().Menu.Id != m_menuFilter)
                continue;

        if (m_groupFilter)
            if (i.value().Group.Id != m_groupFilter)
                continue;

        QListWidgetItem *wi = new QListWidgetItem(m_pMenu);
        wi->setSizeHint(MINSIZE);
        wi->setData(Qt::UserRole, i.key());
        wi->setText(QString("%1 - %2")
                    .arg(i.value().Name).arg(i.value().Price));
    }

    m_pMenu->sortItems();
}

void QOrderDialog::OnSwitchGroups()
{
    m_viewMode = vmDishesGroups;
    m_groupFilter = 0;
    m_pMenu->clear();
    for (QMap<int, DishGroup>::const_iterator i = Data::dishesGroups.begin(); i != Data::dishesGroups.end(); i++)
    {
        QListWidgetItem *wi = new QListWidgetItem(m_pMenu);
        wi->setSizeHint(MINSIZE);
        wi->setData(Qt::UserRole, i.key());
        wi->setText(i->Name);
    }

    m_pMenu->sortItems();
}

void QOrderDialog::OnSwitchMenus()
{
    m_viewMode = vmMenus;
    m_menuFilter = 0;
    m_groupFilter = 0;
    m_pMenu->clear();
    for (QMap<int, DishMenu>::const_iterator i = Data::dishesMenus.begin(); i != Data::dishesMenus.end(); i++)
    {
        QListWidgetItem *wi = new QListWidgetItem(m_pMenu);
        wi->setSizeHint(MINSIZE);
        wi->setData(Qt::UserRole, i.key());
        wi->setText(i->Name);
    }

    m_pMenu->sortItems();
}

void QOrderDialog::OnMenuClicked(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (m_cashMode)
    {
        QMessageDialog::ShowMessage(tr("Access denied"));
        return;
    }

    switch(m_viewMode)
    {
    case vmDishes: {
        if (!m_porder->Id.length())
        {
            m_porder->newOrder();
            m_porder->m_status = ORDER_OPEN;
            m_porder->m_OrderOwner = m_porder->m_CurrentWaiter;
            m_porder->saveOrder();
        }

        Dish *dish = new Dish();
        *dish = Data::dishes[m_pMenu->item(index.row())->data(Qt::UserRole).toInt()];
        dish->OrderId = m_porder->Id;
        dish->genRecordId();
        m_porder->dishes.insert(dish->RecordId, dish);
        viewOrderDishes(m_porder->dishes.count() - 1);
        break;
    }
    case vmDishesGroups: {
        m_groupFilter = m_pMenu->item(index.row())->data(Qt::UserRole).toInt();
        OnSwitchDishes();
        break;
    }
    case vmMenus: {
        m_menuFilter = m_pMenu->item(index.row())->data(Qt::UserRole).toInt();
        OnSwitchGroups();
        break;
    }
    default:
        break;
    }
}

void QOrderDialog::OnScrollMenuDown()
{
    m_pMenu->verticalScrollBar()->setValue(m_pMenu->verticalScrollBar()->value() + 500);
}

void QOrderDialog::OnScrollMenuUp()
{
    m_pMenu->verticalScrollBar()->setValue(m_pMenu->verticalScrollBar()->value() - 500);
}

void QOrderDialog::OnNumpadOk()
{
    if (m_cashMode)
    {
        QMessageDialog::ShowMessage(tr("Access denied"));
        return;
    }

    double qty = m_pNumpad->m_pInput->text().toDouble();
    m_pNumpad->setDefault();

    if (!qty)
        return;

    QList<QListWidgetItem *> l = m_pOrderItems->selectedItems();
    if (!l.count())
        return;

    if (m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->isPrinted())
        m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->setQuantity(m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->quantity() + qty);
    else
        m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->setQuantity(qty);

    viewOrderDishes(m_pOrderItems->row(l[0]));
}

void QOrderDialog::OnScrollOrderDown()
{
    m_pOrderItems->verticalScrollBar()->setValue(m_pOrderItems->verticalScrollBar()->value() + 500);
}

void QOrderDialog::OnScrollOrderUp()
{
    m_pOrderItems->verticalScrollBar()->setValue(m_pOrderItems->verticalScrollBar()->value() - 500);
}

void QOrderDialog::OnClose()
{
    if (!m_porder->dishes.count())
    {
        m_porder->m_status = ORDER_EMPTY;
        m_porder->saveOrder();
    }
    accept();
}

void QOrderDialog::OnDeleteRow()
{
    QList<QListWidgetItem *> l = m_pOrderItems->selectedItems();
    if (!l.count())
        return;

    if (m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->printed() > 0)
    {
        if (m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->printed() < m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->quantity())
        {
            m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->setQuantity(m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->printed());
            viewOrderDishes(m_pOrderItems->row(l[0]));
        }
        else
        {
            QPasswordDialog *p = new QPasswordDialog(this);
            if (p->exec() == QDialog::Accepted)
            {
                QUser *u = new QUser(m_db);
                if (u->IdentifyByPassword(p->m_password))
                {
                    if (u->m_user.Roles.contains(ROLE_DEL_SALE_DOCS))
                    {
                        m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->deleteSelf();
                        m_porder->dishes.remove(l[0]->data(Qt::UserRole).toString());
                        viewOrderDishes(0);
                    }
                    else
                        QMessageDialog::ShowMessage(tr("Access denied"), this);
                }
                else
                    QMessageDialog::ShowMessage(tr("Access denied"), this);
                delete u;
            }
            delete p;
        }
        return;
    }

    m_porder->dishes[l[0]->data(Qt::UserRole).toString()]->deleteSelf();
    m_porder->dishes.remove(l[0]->data(Qt::UserRole).toString());
    viewOrderDishes(0);
}

void QOrderDialog::OnServiceCheck()
{
    //Get printers
    QVector<QString> printers;
    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
    {
        if ((*i)->printed() == (*i)->quantity())
            continue;

        for (QVector<QString>::const_iterator prn = Data::printSchema[(*i)->printSchema].begin(); prn != Data::printSchema[(*i)->printSchema].end(); prn++)
            if (!printers.contains(*prn))
                printers.push_back(*prn);
    }

    //Print on the each printer
    for (QVector<QString>::const_iterator i = printers.begin(); i != printers.end(); i++)
    {
        int left = 5, top = 5;
        QPrint p(*i);
        if (Data::settings[S_LOGO_ON_SERVICE_CHECK].toBool())
        {
            left = 30;
            p.addCommand(new QPCBitmap(QRect(left, top, 20, 20), Data::fileLogo));
            top += 30;
            left = 5;
        }

        QFont font(qApp->font());
        font.setPointSize(font.pointSize() - 2);
        p.addCommand(new QPCSetFont(font));

        QPen pen;
        pen.setStyle(Qt::SolidLine);
        p.addCommand(new QPCSetPen(pen));

        //Header
        p.addCommand(new QPCText(tr("Order:"), QPoint(left, top)));
        p.addCommand(new QPCText(m_porder->Id, QPoint(left + 30, top)));
        top += p.rscale(QFontMetrics(font).height());

        p.addCommand(new QPCText(tr("Hall/Table:"), QPoint(left, top)));
        p.addCommand(new QPCText(m_porder->m_table.hall.Name + "/" + m_porder->m_table.Name, QPoint(left + 30, top)));
        top += p.rscale(QFontMetrics(font).height());

        p.addCommand(new QPCText(tr("Waiter:"), QPoint(left, top)));
        p.addCommand(new QPCText(m_porder->m_CurrentWaiter.m_user.Name, QPoint(left + 30, top)));
        top += p.rscale(QFontMetrics(font).height());

        p.addCommand(new QPCText(tr("Printed:"), QPoint(left, top)));
        p.addCommand(new QPCText(QDateTime::currentDateTime().toString(DATE_FORMAT), QPoint(left + 30, top)));
        top += 1;

        p.addCommand(new QPCLine(QPoint(5, top), QPoint(75, top)));
        top += 2;

        //Dishes
        font.setPointSize(font.pointSize() + 2);
        p.addCommand(new QPCSetFont(font));
        for (QMap<QString, Dish *>::const_iterator j = m_porder->dishes.begin(); j != m_porder->dishes.end(); j++)
        {
            if ((*j)->printed() == (*j)->quantity())
                continue;

            top += p.rscale(QFontMetrics(font).height());
            p.addCommand(new QPCText((*j)->Name, QPoint(left, top)));
            top += p.rscale(QFontMetrics(font).height());
            p.addCommand(new QPCText(QString::number((*j)->quantity() - (*j)->printed()), QPoint(35, top)));
            top += 1;
            p.addCommand(new QPCLine(QPoint(5, top), QPoint(75, top)));
            if (top  > 270)
            {
                top = 5;
                p.addCommand(new QPCNewPage());
            }
        }
        p.beginPrint();
    }

    for (QMap<QString, Dish *>::iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
        (*i)->setPrintedQuantity((*i)->quantity());

    viewOrderDishes(0);

}

void QOrderDialog::OnOpenMainMenu()
{
    QMenuWindow *m = new QMenuWindow(this);
    m->exec();
    switch(m->m_actionCode)
    {
    case ACTION_NONE:
        break;
    case ACTION_PRINT_PAYMENT_CHECK:
        printPaymentCheck();
        break;
    case ACTION_ORDER_CLOSE:
        closeOrder();
        break;
    case ACTION_ORDER_MOVE:
        moveOrder();
        break;
    case ACTION_RECIPE_ON_FLY:
        recipeOnFly();
        break;
    default:
        QMessageBox::critical(this, QString::number(m->m_actionCode), "");
    }
    delete m;
}

void QOrderDialog::viewOrderDishes(int set_index)
{
    m_pOrderItems->clear();
    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
    {
        QListWidgetItem *wi = new QListWidgetItem(m_pOrderItems);
        wi->setSizeHint(MINSIZE);
        wi->setData(Qt::UserRole, i.key());
        wi->setText(i.value()->Name);
    }
    m_pOrderItems->setCurrentRow(set_index);

    bool service_check_btn = false;
    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
        if ((*i)->quantity() > (*i)->printed())
        {
            service_check_btn = true;
            break;
        }
    m_pbtnServiceCheck->setEnabled(service_check_btn);

    viewCommonInfo();
}

void QOrderDialog::viewCommonInfo()
{
    m_pInfo->clear();
    m_porder->dishesAmount();

    QListWidgetItem *wi;

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1=%2/%3").arg(tr("Hall/Table")).arg(m_porder->m_table.hall.Name).arg(m_porder->m_table.Name));

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1=%2").arg(tr("Waiter")).arg(m_porder->m_CurrentWaiter.m_user.Name));

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1=%2").arg(tr("Counted")).arg(m_porder->prop_d(ORDER_MOD_COUNTED)));

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1 %2%=%3").arg(tr("Service margin")).arg(m_porder->prop_d(ORDER_MOD_SERVICE_VALUE)).arg(m_porder->prop_d(ORDER_MOD_SERVICE_AMOUNT)));

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1=%2").arg(tr("Discount")).arg(0));

    wi = new QListWidgetItem(m_pInfo);
    wi->setText(QString("%1=%2").arg(tr("Total amount")).arg(m_porder->m_Amount));
}

void QOrderDialog::printPaymentCheck()
{
    bool ready = true;
    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
        if ((*i)->quantity() != (*i)->printed())
        {
            ready = false;
            break;
        }

    ready = ready && m_porder->dishes.count();

    if (!ready)
    {
        QMessageDialog::ShowMessage(tr("Order incomplete"), this);
        return;
    }

    QPrint p(Data::settings[S_CASH_PRINTER].toString());
    QFont font(qApp->font());
    font.setPointSize(15);
    p.addCommand(new QPCSetFont(font));

    int left = 0, top = 5, width = 60;

    QStringList l = Data::settings[S_PAYMENT_CHECK_LOGO_SIZE].toString().split(" ");
    p.addCommand(new QPCBitmap(QRect(l[0].toInt(), l[1].toInt(), l[2].toInt(), l[3].toInt()), Data::fileLogo));
    top += l[3].toInt();

    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top += 3;

    font.setPointSize(10);
    p.addCommand(new QPCSetFont(font));

    p.addCommand(new QPCText(tr("Order"), QPoint(left, top)));
    QString ord = m_porder->Id;
    p.addCommand(new QPCText(ord, QPoint(width - p.rscale(QFontMetrics(font).width(ord)) - 2, top)));
    top += p.rscale(QFontMetrics(font).height());

    p.addCommand(new QPCText(tr("Hall/Table"), QPoint(left, top)));
    QString table = m_porder->m_table.hall.Name + "/" + m_porder->m_table.Name;
    p.addCommand(new QPCText(table, QPoint(width - p.rscale(QFontMetrics(font).width(table)) - 2, top)));
    top += p.rscale(QFontMetrics(font).height());

    p.addCommand(new QPCText(tr("Waiter"), QPoint(left, top)));
    p.addCommand(new QPCText(m_porder->m_CurrentWaiter.m_user.Name, QPoint(width - p.rscale(QFontMetrics(font).width(m_porder->m_CurrentWaiter.m_user.Name)) - 2, top)));
    top += p.rscale(QFontMetrics(font).height());

    p.addCommand(new QPCText(tr("Printed"), QPoint(left, top)));
    p.addCommand(new QPCText(QDateTime::currentDateTime().toString(DATE_FORMAT), QPoint(width - p.rscale(QFontMetrics(font).width(QDateTime::currentDateTime().toString(DATE_FORMAT))) - 2, top)));
    top ++;

    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top ++;

    QPen pen;
    pen.setStyle(Qt::DashLine);
    p.addCommand(new QPCSetPen(pen));
    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top ++;


    font.setPointSize(12);
    p.addCommand(new QPCSetFont(font));

    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
    {
        top += p.rscale(QFontMetrics(font).height());
        p.addCommand(new QPCText((*i)->Name, QPoint(left, top)));
        top += p.rscale(QFontMetrics(font).height());
        p.addCommand(new QPCText(QString::number((*i)->quantity()), QPoint(15, top)));
        p.addCommand(new QPCText(QString::number((*i)->Price), QPoint(30, top)));
        p.addCommand(new QPCText(QString::number((*i)->quantity() * (*i)->Price), QPoint(50, top)));
        top ++;
        p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    }

    top ++;
    pen.setStyle(Qt::SolidLine);
    p.addCommand(new QPCSetPen(pen));
    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top += p.rscale(QFontMetrics(font).height());

    p.addCommand(new QPCText(QString("%1").arg(tr("Counted")).arg(m_porder->prop_d(ORDER_MOD_COUNTED)), QPoint(left, top)));
    QString counted = QString::number(m_porder->prop_d(ORDER_MOD_COUNTED));
    p.addCommand(new QPCText(counted, QPoint(width - 2 - p.rscale(QFontMetrics(font).width(counted)), top)));
    top += p.rscale(QFontMetrics(font).height());

    if (m_porder->prop_d(ORDER_MOD_SERVICE_AMOUNT) > 0)
    {
        p.addCommand(new QPCText(QString("%1 +%2%").arg(tr("Service")).arg(m_porder->prop_d(ORDER_MOD_SERVICE_VALUE)), QPoint(left, top)));
        QString service_value = QString::number(m_porder->prop_d(ORDER_MOD_SERVICE_AMOUNT));
        p.addCommand(new QPCText(service_value, QPoint(width - 2 - p.rscale(QFontMetrics(font).width(service_value)), top)));
        top += p.rscale(QFontMetrics(font).height());
    }

    font.setPointSize(14);
    font.setBold(true);
    p.addCommand(new QPCSetFont(font));
    p.addCommand(new QPCText(tr("Total"), QPoint(left, top)));
    QString total = QString::number(m_porder->m_Amount);
    p.addCommand(new QPCText(total, QPoint(width - 2 - p.rscale(QFontMetrics(font).width(total)), top)));
    top ++;

    top++;
    p.addCommand(new QPCLine(QPoint(left, top), QPoint(width, top)));
    top += p.rscale(QFontMetrics(font).height());

    font.setPointSize(10);
    p.addCommand(new QPCSetFont(font));
    p.addCommand(new QPCText(tr("Thank You!"), QPoint(left, top)));
    top += p.rscale(QFontMetrics(font).height());

    font.setPointSize(8);
    p.addCommand(new QPCSetFont(font));
    QString sample_number = QString("%1: %2").arg(tr("Sample")).arg(m_porder->prop_i(ORDER_MOD_PRINTED_PAYMENT_CHECKS) + 1);
    p.addCommand(new QPCText(sample_number, QPoint(width - 2 - p.rscale(QFontMetrics(font).width(sample_number)), top)));

    p.beginPrint();

    m_porder->setProp_I(ORDER_MOD_PRINTED_PAYMENT_CHECKS, m_porder->prop_i(ORDER_MOD_PRINTED_PAYMENT_CHECKS) + 1);
    m_porder->saveOrder();
}

void QOrderDialog::closeOrder()
{

    if (m_cashMode)
    {
        QMessageDialog::ShowMessage(tr("Access denied"));
        return;
    }

    if (!m_porder->dishes.count())
    {
        QMessageDialog::ShowMessage(tr("Order empty\n"));
        return;
    }

    for (QMap<QString, Dish *>::const_iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
        if ((*i)->quantity() > (*i)->printed())
        {
            QMessageDialog::ShowMessage(tr("Order must processed by service\n"), this);
            return;
        }

    if (!m_porder->prop_i(ORDER_MOD_PRINTED_PAYMENT_CHECKS))
    {
        QMessageDialog::ShowMessage(tr("The payment check is not printed\n"), this);
        return;
    }

    m_porder->m_status = ORDER_CLOSED;
    m_porder->mods.insert(ORDER_MOD_TIME_CLOSE, QDateTime::currentDateTime().toString(DATETIME_FORMAT));
    m_porder->setDateCash(QTime::fromString(Data::settings[S_AUTO_CHANGE_DATE_OF_CASH].toString(), TIME_FORMAT));
    m_porder->saveOrder();
    accept();
}

void QOrderDialog::moveOrder()
{
    if (m_cashMode)
    {
        QMessageDialog::ShowMessage(tr("Access denied"));
        return;
    }

    if (!m_porder->dishes.count())
    {
        QMessageDialog::ShowMessage(tr("You cannot move the empty table\n"), this);
        return;
    }

    int table_id = 0;
    QHallDialog *h = new QHallDialog(m_porder->m_CurrentWaiter, this);
    if (h->exec() == QDialog::Accepted)
    {
        table_id = h->m_TableId;

        if (table_id == m_porder->m_table.Id)
        {
            QMessageDialog::ShowMessage(tr("You cannot move order on same table"), this);
        }
        else
        {
            QString order_id = "";
            m_db.open();
            QSqlQuery *q = m_db.query();
            m_db.prepare(q, "SELECT ID FROM SALE_HEADER WHERE TABLE_ID=:TABLE_ID AND STATUS_ID=:STATUS_ID");
            q->bindValue("TABLE_ID", table_id);
            q->bindValue("STATUS_ID", ORDER_OPEN);
            m_db.exec(q);
            q->next();
            order_id = q->value(0).toString();
            m_db.close();

            QMoveOrderDialog *m = new QMoveOrderDialog(this);
            for (QMap<QString, Dish *>::iterator i = m_porder->dishes.begin(); i != m_porder->dishes.end(); i++)
            {
                QListWidgetItem *wi = new QListWidgetItem(m->m_pSrcTable);
                wi->setText((*i)->Name);
                wi->setSizeHint(QSize(100, 60));
                wi->setData(Qt::UserRole, i.key());
            }
            m->m_pSrcTableName->setText(m_porder->m_table.Name);


            QOrder order(m_db);
            if (order_id.length())
            {
                order.loadOrder(order_id);
                for (QMap<QString, Dish *>::const_iterator i = order.dishes.begin(); i != order.dishes.end(); i++)
                {
                    QListWidgetItem *wi = new QListWidgetItem(m->m_pDstTable);
                    wi->setText((*i)->Name);
                    wi->setSizeHint(QSize(100, 60));
                    wi->setData(Qt::UserRole, i.key());
                }
            }
            m->m_pDstTableName->setText(Data::tables[table_id].Name);

            if (m->exec() == QDialog::Accepted)
            {
                for (int i = 0; i < m->m_pSrcTable->count(); i++)
                {
                    QString RecordId = m->m_pSrcTable->item(i)->data(Qt::UserRole).toString();
                    if (!m_porder->dishes.contains(RecordId))
                    {
                        order.dishes[RecordId]->OrderId = m_porder->Id;
                        order.dishes[RecordId]->save();
                        m_porder->dishes.insert(RecordId, order.dishes[RecordId]);
                        order.dishes.remove(RecordId);
                    }
                }
                m_porder->dishesAmount();
                m_porder->saveOrder();

                for (int i = 0; i < m->m_pDstTable->count(); i++)
                {
                    if (!order.Id.length())
                    {
                        order.newOrder();
                        order.m_status = ORDER_OPEN;
                        order.m_OrderOwner = m_porder->m_CurrentWaiter;
                        order.m_CurrentWaiter = m_porder->m_CurrentWaiter;
                        order.m_table = Data::tables[table_id];
                        order.saveOrder();
                    }

                    QString RecordId = m->m_pDstTable->item(i)->data(Qt::UserRole).toString();
                    if (!order.dishes.contains(RecordId))
                    {
                        m_porder->dishes[RecordId]->OrderId = order.Id;
                        m_porder->dishes[RecordId]->save();
                        order.dishes.insert(RecordId, m_porder->dishes[RecordId]);
                        m_porder->dishes.remove(RecordId);
                    }
                }
                order.dishesAmount();
                order.saveOrder();

                if (order.Id.length())
                    if (!order.dishes.count())
                    {
                        order.m_status = ORDER_EMPTY;
                        order.setDateCash(QTime::fromString(Data::settings[S_AUTO_CHANGE_DATE_OF_CASH].toString(), TIME_FORMAT));
                        order.saveOrder();
                    }
                if (!m_porder->dishes.count())
                {
                    m_porder->m_status = ORDER_EMPTY;
                    m_porder->setDateCash(QTime::fromString(Data::settings[S_AUTO_CHANGE_DATE_OF_CASH].toString(), TIME_FORMAT));
                    m_porder->saveOrder();
                }
            }
            delete q;
        }
    }
    delete h;
    close();
}

void QOrderDialog::recipeOnFly()
{
    if (m_cashMode)
    {
        QMessageDialog::ShowMessage(tr("Access denied"));
        return;
    }

    QList<QListWidgetItem *> l = m_pOrderItems->selectedItems();
    if (!l.count())
        return;

    QCostumDishDialog *c = new QCostumDishDialog(this);
    c->exec();
    delete c;
}

QOrderItemsDelegate::QOrderItemsDelegate(QOrder &order) :
    m_order(order)
{
}

void QOrderItemsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(index.row() % 2 ? COLOR_EVEN : COLOR_OGG);
    if (option.state & QStyle::State_Selected)
        brush.setColor(qRgb(170, 255, 170));
    painter->setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter->setPen(pen);

    QTextOption to;
    to.setAlignment(Qt::AlignVCenter);
    to.setWrapMode(QTextOption::WrapAnywhere);

    Dish *dish = m_order.dishes[index.data(Qt::UserRole).toString()];
    QRect rectName = option.rect;
    int r_width = option.rect.width() / 2;
    rectName.setRight(option.rect.left() + r_width);
    QRect rectQuan = option.rect;
    rectQuan.setRect(rectName.right() + 2, option.rect.top(), r_width / 3, option.rect.height());
    QRect rectPrintQuan = option.rect;
    rectPrintQuan.setRect(rectQuan.right() + 2, option.rect.top(), r_width / 3, option.rect.height());
    QRect rectPrice = option.rect;
    rectPrice.setRect(rectPrintQuan.right() + 2, option.rect.top(), r_width / 3, option.rect.height());

    painter->drawRect(option.rect);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawText(rectName, dish->Name, to);
    painter->drawText(rectQuan, QString::number(dish->quantity()), to);
    painter->drawText(rectPrintQuan, QString::number(dish->printed()), to);
    painter->drawText(rectPrice, QString::number(dish->Price), to);
}


void QMenuItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(index.row() % 2 ? COLOR_EVEN : COLOR_OGG);
    if (option.state & QStyle::State_Selected)
        brush.setColor(qRgb(170, 255, 170));
    painter->setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter->setPen(pen);

    QTextOption to;
    to.setAlignment(Qt::AlignVCenter);
    to.setWrapMode(QTextOption::WrapAnywhere);

    painter->drawRect(option.rect);
    QRect rect = option.rect;
    rect.adjust(2, 0, 0, 0);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawText(rect, index.data(Qt::DisplayRole).toString(), to);
}


void QInfoItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(index.row() % 2 ? COLOR_EVEN : COLOR_OGG);
    if (option.state & QStyle::State_Selected)
        brush.setColor(qRgb(170, 255, 170));
    painter->setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter->setPen(pen);

    QTextOption to;
    to.setAlignment(Qt::AlignVCenter);
    to.setWrapMode(QTextOption::WrapAnywhere);

    painter->drawRect(option.rect);
    QRect rect = option.rect;
    rect.adjust(2, 0, 0, 0);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    QStringList l = index.data(Qt::DisplayRole).toString().split("=");
    painter->drawText(rect, l[0], to);
    to.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    painter->drawText(rect, l[1], to);
}
