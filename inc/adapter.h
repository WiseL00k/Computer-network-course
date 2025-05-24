#ifndef __ADAPTER_H__
#define __ADAPTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"

typedef struct
{
    WSADATA wsaData;
    SOCKET sock;
    DWORD flag;
    char *buffer;
    int buf_size;
    int bytesReceived;
    SOCKADDR_IN addr;
} Adapter, *AdapterHandle;

int adapter_init(Adapter *adapter, const char *ip);
Packet adapter_receive_packet(Adapter *adapter);
void adapter_close(Adapter *adapter);
int print_local_ip();
char* get_local_ip(int num);

#endif // ADAPTER_H