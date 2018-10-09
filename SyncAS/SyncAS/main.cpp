#include <QApplication>
#include "qmaindialog.h"
#include <QDir>

int main (int argc, char *argv[])
{
    QDir d;
    if (!d.exists(QDir::homePath() + "/QuickPoint/SyncAS"))
        d.mkpath(QDir::homePath() + "/QuickPoint/SyncAS");

    QApplication a(argc, argv);
    QMainDialog *m = new QMainDialog();
    m->exec();
    delete m;
    return 0;
}
