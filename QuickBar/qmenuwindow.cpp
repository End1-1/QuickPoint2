#include "qmenuwindow.h"

QMenuWindow::QMenuWindow(QWidget *parent) :
    QBaseDialog(tr("Menu"), parent), m_actionCode(0)
{
    QVBoxLayout *plMain = new QVBoxLayout();
    plMain->addWidget(m_pActions = new QListWidget(this));
    QPushButton *pbtnCancel = new QPushButton(tr("Cancel"), this);
    pbtnCancel->setMinimumSize(0, 60);
    plMain->addWidget(pbtnCancel);
    setLayout(plMain);

    QListWidgetItem *wi;
    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Print payment check"));
    wi->setData(Qt::UserRole, ACTION_PRINT_PAYMENT_CHECK);
    wi->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Close order"));
    wi->setData(Qt::UserRole, ACTION_ORDER_CLOSE);
    wi->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Move order to another table"));
    wi->setData(Qt::UserRole, ACTION_ORDER_MOVE);
    wi->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    /*
    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Move part of order to the another table"));
    wi->setData(Qt::UserRole, ACTION_ORDER_MOVE_PARTIAL);
    wi->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    */

    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Invalidate the order"));
    wi->setData(Qt::UserRole, ACTION_ORDER_INVALIDATE);
    wi->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    wi = new QListWidgetItem(m_pActions);
    wi->setSizeHint(QSize(500, 60));
    wi->setText(tr("Recipe on fly"));
    wi->setTextAlignment(Qt::AlignCenter);
    wi->setData(Qt::UserRole, ACTION_RECIPE_ON_FLY);

    connect(m_pActions, SIGNAL(clicked(QModelIndex)), SLOT(OnAction(QModelIndex)));
    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
}

void QMenuWindow::OnAction(QModelIndex index)
{
    if (!index.isValid())
        return;

    m_actionCode = index.data(Qt::UserRole).toInt();
    accept();
}
