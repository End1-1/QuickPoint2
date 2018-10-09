#include "fdaterange.h"
#include "ui_fdaterange.h"

FDateRange::FDateRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FDateRange)
{
    ui->setupUi(this);
}

FDateRange::~FDateRange()
{
    delete ui;
}

bool FDateRange::filter(QDate &d1, QDate &d2)
{
    FDateRange *d = new FDateRange();
    d->ui->d1->setDate(d1);
    d->ui->d2->setDate(d2);
    bool result = d->exec() == QDialog::Accepted;
    if (result) {
        d1 = d->ui->d1->date();
        d2 = d->ui->d2->date();
    }
    delete d;
    return result;
}

void FDateRange::on_btnCancel_clicked()
{
    reject();
}

void FDateRange::on_btnOK_clicked()
{
    accept();
}
