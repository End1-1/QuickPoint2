#ifndef FILTERCARDSTAT_H
#define FILTERCARDSTAT_H

#include <QDialog>

namespace Ui {
class FilterCardStat;
}

class FilterCardStat : public QDialog
{
    Q_OBJECT

public:
    explicit FilterCardStat(QWidget *parent = 0);
    ~FilterCardStat();
    static bool getFilter(QString &filter, QString &d1, QString &d2, bool &g);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::FilterCardStat *ui;
};

#endif // FILTERCARDSTAT_H
