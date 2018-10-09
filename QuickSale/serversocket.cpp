#include "serversocket.h"
#include "nettaxthread.h"
#include <winsock2.h>

ServerSocket::ServerSocket(QObject *parent) :
    QThread(parent)
{

}

void ServerSocket::run()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    sockaddr_in addr, clientAddr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(888);

    int mSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(mSocket, (sockaddr *)&addr, sizeof(sockaddr));
    listen(mSocket, 3);
    int clientAddrSize = sizeof(sockaddr);
    uint clientSocket;
    do {
        clientSocket = accept(mSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        NetTaxThread *m = new NetTaxThread(clientSocket);
        m->start();
    } while (clientSocket != INVALID_SOCKET);
    WSACleanup();
}
