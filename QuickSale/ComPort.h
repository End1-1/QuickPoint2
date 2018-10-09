#ifndef COMPORT_H
#define COMPORT_H

#include <windows.h>
#include <QByteArray>

class ComPort {
public:
    ComPort(char *port);
    ~ComPort();
    bool Write(const char *data, const int &data_len, DWORD *data_writed);
    bool Read(QByteArray &data);
private:
    bool configPort(char *port);
    HANDLE m_File;
};

#endif // COMPORT_H
