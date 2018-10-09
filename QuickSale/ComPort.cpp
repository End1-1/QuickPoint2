#include "ComPort.h"
#include <QMessageBox>

bool ComPort::configPort(char *port)
{
    wchar_t *wport = new wchar_t[255];
    memset(reinterpret_cast<void *>(wport), 0, sizeof(wchar_t) * 255);
    mbstowcs(wport, port, strlen(port));

    m_File = CreateFile(wport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (!m_File)
        return false;

    SetupComm(m_File, static_cast<DWORD>(2048), static_cast<DWORD>(2048));

    DCB dcb;
    memset(reinterpret_cast<void *>(&dcb), 0, sizeof(DCB));
    bool fSuccess = GetCommState(m_File, &dcb);
    if(!fSuccess)
        return false;

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    fSuccess = SetCommState(m_File, &dcb);
    if (!fSuccess)
        return false;

    COMMTIMEOUTS ct;
    fSuccess = GetCommTimeouts(m_File, &ct);
    if (!fSuccess)
        return false;
    ct.ReadIntervalTimeout = 100;
    ct.ReadTotalTimeoutConstant = 500;
    ct.ReadTotalTimeoutMultiplier = 0;
    ct.WriteTotalTimeoutConstant = 2000;
    ct.WriteTotalTimeoutMultiplier = 60;

    fSuccess = SetCommTimeouts(m_File, &ct);
    return fSuccess;
}


ComPort::ComPort(char *port)
{
    if (!configPort(port))
        QMessageBox::critical(0, "Error", "Cannot configure comport!");
}

bool ComPort::Write(const char *data, const int &data_len, DWORD *data_writed)
{
    bool fSuccess = WriteFile(m_File, data, data_len, data_writed, 0);
    return fSuccess;
}

bool ComPort::Read(QByteArray &data)
{
    //LPVOID data;

    //ReadFile(m_file, data, 1024, dataSize, 0);
    return true;
}


ComPort::~ComPort()
{
    CloseHandle(m_File);
}
