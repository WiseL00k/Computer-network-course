#ifndef __PACKET_H__
#define __PACKET_H__

#include <winsock2.h>
#include <ws2tcpip.h>

typedef struct IP
{
    char src_ip[16]; // 源IP地址
    char dst_ip[16]; // 目的IP地址
    int protocol;
    int ttl;
    // unsigned short id;
} NetworkLayer;

typedef struct TransportLayer
{
    u_short src_port; // 源端口
    u_short dst_port; // 目的端口
} TransportLayer;

typedef struct
{
    char *buffer;
    NetworkLayer ip;          // 网络层抽象
    TransportLayer transport; // 传输层抽象
    int length;
} Packet;

// IP头部结构体（RFC 791）
#pragma pack(push, 1)
typedef struct IP_HEADER
{
    BYTE ver_ihl;     // 版本(4位) + 头部长度(4位)
    BYTE tos;         // 服务类型
    USHORT total_len; // 总长度
    USHORT id;        // 标识符
    USHORT flags_fo;  // 标志位和分段偏移
    BYTE ttl;         // 生存时间
    BYTE protocol;    // 协议类型（TCP=6，UDP=17）
    USHORT checksum;  // 校验和
    ULONG src_ip;     // 源IP地址（网络字节序）
    ULONG dst_ip;     // 目的IP地址（网络字节序）
} IP_HEADER;

// 定义TCP头部结构体（按1字节对齐）
typedef struct _TCP_HEADER
{
    USHORT src_port;      // 源端口（网络字节序）
    USHORT dst_port;      // 目的端口（网络字节序）
    ULONG seq_num;        // 序列号
    ULONG ack_num;        // 确认号
    USHORT hdr_len_flags; // 数据偏移(4位) + 保留(6位) + 标志(6位)
    USHORT window;        // 窗口大小
    USHORT checksum;      // 校验和
    USHORT urgent_ptr;    // 紧急指针
} TCP_HEADER;

// 定义UDP头部结构体（按1字节对齐）
typedef struct _UDP_HEADER
{
    USHORT src_port; // 源端口（网络字节序）
    USHORT dst_port; // 目的端口（网络字节序）
    USHORT length;   // 数据报长度
    USHORT checksum; // 校验和
} UDP_HEADER;
#pragma pack(pop)

#endif // __PACKET_H__