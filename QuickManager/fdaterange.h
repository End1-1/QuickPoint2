#ifndef FDATERANGE_H
#define FDATERANGE_H

#include <QDialog>

namespace Ui {
class FDateRange;
}

class FDateRange : public QDialog
{
    Q_OBJECT

public:
    explicit FDateRange(QWidget *parent = 0);
    ~FDateRange();
    static bool filter(QDate &d1, QDate &d2);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::FDateRange *ui;
};

#endif // FDATERANGE_H
