#ifndef QDBPAIR_H
#define QDBPAIR_H

#include <QDialog>
#include <QMultiMap>

class QLineEdit;
class QComboBox;

struct DBPair {
    char name [255];
    char src_conn_str [255];
    char src_db [64];
    char src_user [255];
    char src_pass [255];
    char dst_conn_str [255];
    char dst_user [255];
    char dst_pass [255];
    int operatorId;
    char storeConverter [255];
    void *pstoreConverter;
    char foodConverter [255];
    void *pfoodConverter;
};

class QDBPair : public QDialog
{
    Q_OBJECT
public:
    explicit QDBPair(QWidget *parent = 0);
    
    QLineEdit *m_pName;
    QLineEdit *m_pSrcDb;
    QLineEdit *m_pSrcConnStr;
    QLineEdit *m_pSrcUser;
    QLineEdit *m_pSrcPass;
    QLineEdit *m_pDstDb;
    QLineEdit *m_pDstUser;
    QLineEdit *m_pDstPass;
    QComboBox *m_pstoreConverter;
    QComboBox *m_pfoodConverter;
    QLineEdit *m_pOperatorId;

    void makeStoreMap(const QMap<QString, QMap<QString, int> > &map);
    void makeFoodMap(const QMap<QString, QMap<QString, int> > &map);

private slots:
    void OK();
};

#endif // QDBPAIR_H
