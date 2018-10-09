#include "filtercardstat.h"
#include "ui_filtercardstat.h"
#include "defines.h"

FilterCardStat::FilterCardStat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterCardStat)
{
    ui->setupUi(this);
}

FilterCardStat::~FilterCardStat()
{
    delete ui;
}

bool FilterCardStat::getFilter(QString &filter, QString &d1, QString &d2, bool &g)
{
    FilterCardStat *f = new FilterCardStat(fMainWindow);
    f->ui->leCard->setText(filter);
    f->ui->deStart->setDate(QDate::fromString(d1, "dd.MM.yyyy"));
    f->ui->deEnd->setDate(QDate::fromString(d2, "dd.MM.yyyy"));
    f->ui->chGroup->setChecked(g);
    bool result = f->exec() == QDialog::Accepted;
    if (result) {
        filter = f->ui->leCard->text();
        d1 = f->ui->deStart->date().toString("dd.MM.yyyy");
        d2 = f->ui->deEnd->date().toString("dd.MM.yyyy");
        g = f->ui->chGroup->isChecked();
    }
    delete f;
    return result;
}

void FilterCardStat::on_btnCancel_clicked()
{
    reject();
}

void FilterCardStat::on_btnOK_clicked()
{
    accept();
}
