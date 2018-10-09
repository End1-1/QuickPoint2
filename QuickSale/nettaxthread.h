#ifndef NETTAXTHREAD_H
#define NETTAXTHREAD_H

#include <QThread>

class NetTaxThread : public QThread
{
public:
    NetTaxThread(int socket);
    int fSocket;
    virtual void run();
};

#endif // NETTAXTHREAD_H
