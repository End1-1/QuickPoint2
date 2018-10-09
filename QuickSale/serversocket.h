#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QThread>

class ServerSocket : public QThread
{
public:
    ServerSocket(QObject *parent = 0);
protected:
    virtual void run();
};

#endif // SERVERSOCKET_H
