#include "adapter.h"

int adapter_init(Adapter *adapter, const char *ip)
{
    DWORD flag = 1;                                                     // 启用混杂模式的标志
    adapter->buf_size = 65536;                                          // 与IP包最大长度一致
    adapter->buffer = (char *)malloc(adapter->buf_size * sizeof(char)); // 64KB 缓冲区
    adapter->bytesReceived = 0;
    // 创建原始套接字（需管理员权限）
    adapter->sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (adapter->sock == INVALID_SOCKET)
    {
        printf("套接字创建失败: %d\n", WSAGetLastError());
        return -1;
    }

    // 自动获取本机IP
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(adapter->sock, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("绑定失败: %d\n", WSAGetLastError());
        closesocket(adapter->sock);
        WSACleanup();
        return -1;
    }

    if (setsockopt(adapter->sock, SOL_SOCKET, SO_RCVBUF, (char *)&adapter->buf_size, sizeof(adapter->buf_size)) == SOCKET_ERROR)
    {
        printf("设置接收缓冲区失败: %d\n", WSAGetLastError());
    }

    // 启用混杂模式（接收所有数据包）
    if (ioctlsocket(adapter->sock, SIO_RCVALL, &flag) == SOCKET_ERROR)
    {
        printf("混杂模式设置失败: %d\n", WSAGetLastError());
        closesocket(adapter->sock);
        WSACleanup();
        return -1;
    }

    return 0;
}

Packet adapter_receive_packet(AdapterHandle adapter)
{
    Packet packet;
    if (adapter != NULL)
    {
        // recv参数要填大小，对于一个指针，sizeof只能获得指针本身的大小，而不是指针指向的内存块的大小，从而导致缓冲区大小不够大，导致数据包截断，报错10040
        int bytesReceived = recv(adapter->sock, adapter->buffer, adapter->buf_size, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            printf("接收失败: %d\n", WSAGetLastError());
            packet.buffer = NULL;
            packet.length = 0;
        }
        else
        {
            adapter->bytesReceived = bytesReceived;
            packet.length = bytesReceived;
            packet.buffer = adapter->buffer;
        }
    }
    else
    {
        packet.length = 0;
    }
    return packet;
}

void adapter_close(Adapter *adapter)
{
    if (adapter != NULL)
    {
        if (adapter->sock != INVALID_SOCKET)
        {
            closesocket(adapter->sock);
            adapter->sock = INVALID_SOCKET;
        }
        if (adapter->buffer != NULL)
        {
            free(adapter->buffer);
            adapter->buffer = NULL;
        }
    }
}

// 获取本机IPv4地址
int print_local_ip()
{
    char hostname[256];
    struct hostent *host;

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        printf("获取主机名失败: %d\n", WSAGetLastError());
        return -1;
    }

    if ((host = gethostbyname(hostname)) == NULL)
    {
        printf("获取主机信息失败: %d\n", WSAGetLastError());
        return -1;
    }
    puts("=========================");
    printf("本机所拥有的网卡IP地址: \n");

    for (int i = 0; host->h_addr_list[i] != NULL; i++)
    {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
        printf("%d.%s\n", i + 1, inet_ntoa(addr));
    }
    puts("=========================");
    return -1;
}

char *get_local_ip(int num)
{
    char hostname[256];
    struct hostent *host;

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        printf("获取主机名失败: %d\n", WSAGetLastError());
        return NULL;
    }

    if ((host = gethostbyname(hostname)) == NULL)
    {
        printf("获取主机信息失败: %d\n", WSAGetLastError());
        return NULL;
    }

    struct in_addr addr;
    memcpy(&addr, host->h_addr_list[num - 1], sizeof(struct in_addr));
    return inet_ntoa(addr);
}