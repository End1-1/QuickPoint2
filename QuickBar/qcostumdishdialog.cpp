#include "qcostumdishdialog.h"
#include "qnumpadwidget.h"

QCostumDishDialog::QCostumDishDialog(QWidget *parent) :
    QBaseDialog(tr("Cosum dish"), parent)
{
    m_pGoods = new QListWidget(this);
    QHBoxLayout *plGoodsButtons = new QHBoxLayout();
    QWidget *pwdtGoodsButtons = new QWidget(this);
    QPushButton *pbtnGoodsDown = qpushbutton(tr("Down"));
    QPushButton *pbtnGoodsUp = qpushbutton(tr("Up"));
    QPushButton *pbtnGoodsGroups = qpushbutton(tr("Groups"));
    QPushButton *pbtnGoods = qpushbutton(tr("Goods"));
    plGoodsButtons->addWidget(pbtnGoodsDown);
    plGoodsButtons->addWidget(pbtnGoodsUp);
    plGoodsButtons->addWidget(pbtnGoodsGroups);
    plGoodsButtons->addWidget(pbtnGoods);
    plGoodsButtons->addStretch(1);
    pwdtGoodsButtons->setLayout(plGoodsButtons);

    QVBoxLayout *plLeft = new QVBoxLayout();
    QWidget *pwdtLeft = new QWidget(this);
    plLeft->addWidget(m_pGoods);
    plLeft->addWidget(pwdtGoodsButtons);
    pwdtLeft->setLayout(plLeft);

    QNumpadWidget *pNumpad = new QNumpadWidget(this);
    m_pTotal = new QListWidget(this);
    QHBoxLayout *plTotal = new QHBoxLayout();
    QWidget *pwdtTotal = new QWidget(this);
    plTotal->addWidget(pNumpad);
    plTotal->addWidget(m_pTotal);
    pwdtTotal->setLayout(plTotal);

    m_pRecipe = new QListWidget(this);
    QHBoxLayout *plRecipeButtons = new QHBoxLayout();
    QWidget *pwdtRecipeButtons = new QWidget(this);
    QPushButton *pbtnRecipeDown = qpushbutton(tr("Down"));
    QPushButton *pbtnRecipeUp = qpushbutton(tr("Up"));
    QPushButton *pbtnOk = qpushbutton(tr("Ok"));
    QPushButton *pbtnCancel = qpushbutton(tr("Cancel"));
    plRecipeButtons->addWidget(pbtnRecipeDown);
    plRecipeButtons->addWidget(pbtnRecipeUp);
    plRecipeButtons->addStretch(1);
    plRecipeButtons->addWidget(pbtnOk);
    plRecipeButtons->addWidget(pbtnCancel);
    pwdtRecipeButtons->setLayout(plRecipeButtons);

    QVBoxLayout *plRight = new QVBoxLayout();
    QWidget *pwdtRight = new QWidget(this);
    plRight->addWidget(pwdtTotal);
    plRight->addWidget(m_pRecipe);
    plRight->addWidget(pwdtRecipeButtons);
    pwdtRight->setLayout(plRight);

    QHBoxLayout *plMain = new QHBoxLayout();
    plMain->addWidget(pwdtLeft);
    plMain->addWidget(pwdtRight);
    setLayout(plMain);

    connect(pbtnCancel, SIGNAL(clicked()), SLOT(reject()));
}
