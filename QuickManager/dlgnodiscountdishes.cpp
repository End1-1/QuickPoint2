#include "dlgnodiscountdishes.h"
#include "ui_dlgnodiscountdishes.h"

DlgNoDiscountDishes::DlgNoDiscountDishes(QWidget *parent) :
    EDialog(parent),
    ui(new Ui::DlgNoDiscountDishes)
{
    ui->setupUi(this);
    QSqlQuery *q = M::db.QuerySelect("select val from cards_nodiscount_dishes");
    if (q) {
        q->next();
        ui->textEdit->setPlainText(q->value(0).toString());
        delete q;
    }
}

DlgNoDiscountDishes::~DlgNoDiscountDishes()
{
    delete ui;
}

void DlgNoDiscountDishes::on_btnCancel_clicked()
{
    reject();
}

void DlgNoDiscountDishes::on_btnOK_clicked()
{
    QStringList paramNames;
    paramNames << "val";
    QVector<QVariant> paramValues;
    paramValues << ui->textEdit->toPlainText();
    QSqlQuery *q = M::db.QuerySelect("update cards_nodiscount_dishes set val=:val", paramNames, paramValues);
    if (q) {
        delete q;
        M::db.commit();
    }
    accept();
}
