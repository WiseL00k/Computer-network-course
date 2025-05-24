#include "parser.h"

#include <stdio.h>

int parse_packet(Packet *packet)
{
    if (parse_ip_header(packet) + parse_transport_layer(packet) != 0)
    {
        return -1;
    }
}

int parse_ip_header(Packet *packet)
{
    IP_HEADER *ip_hdr = (IP_HEADER *)(packet->buffer); // 直接解析IP头部

    // IPV4

    // 将网络字节序的IP地址转换为字符串
    struct in_addr src_addr, dst_addr;
    src_addr.s_addr = ip_hdr->src_ip;
    dst_addr.s_addr = ip_hdr->dst_ip;

    packet->ip.protocol = ip_hdr->protocol;
    strcpy(packet->ip.src_ip, inet_ntoa(src_addr));
    strcpy(packet->ip.dst_ip, inet_ntoa(dst_addr));
    packet->ip.ttl = ip_hdr->ttl;
    // packet->ip.id = ntohs(ip_hdr->id);


    return 0;
}

// 解析传输层协议和端口号
int parse_transport_layer(Packet *packet)
{
    IP_HEADER *ip_hdr = (IP_HEADER *)packet->buffer;

    // 计算传输层头部偏移（IP头部长度 = ver_ihl低4位 * 4字节）
    int ip_header_len = (ip_hdr->ver_ihl & 0x0F) * 4;
    char *transport_hdr = (char *)ip_hdr + ip_header_len;

    // 根据协议类型解析TCP或UDP
    if (ip_hdr->protocol == IPPROTO_TCP)
    {
        TCP_HEADER *tcp_hdr = (TCP_HEADER *)transport_hdr;
        printf("=== TCP数据报 ===\n");
        printf("源端口: %d\n", ntohs(tcp_hdr->src_port));
        printf("目的端口: %d\n", ntohs(tcp_hdr->dst_port));
        packet->transport.src_port = ntohs(tcp_hdr->src_port);
        packet->transport.dst_port = ntohs(tcp_hdr->dst_port);
    }
    else if (ip_hdr->protocol == IPPROTO_UDP)
    {
        UDP_HEADER *udp_hdr = (UDP_HEADER *)transport_hdr;
        printf("=== UDP数据报 ===\n");
        printf("源端口: %d\n", ntohs(udp_hdr->src_port));
        printf("目的端口: %d\n", ntohs(udp_hdr->dst_port));
        packet->transport.src_port = ntohs(udp_hdr->src_port);
        packet->transport.dst_port = ntohs(udp_hdr->dst_port);
    }
    else
    {
        printf("未知协议类型: %d\n", ip_hdr->protocol);
    }
    return 0;
}

void print_raw_packet(Packet *packet)
{
    printf("\n=== 数据包内容 ===\n");
    for (int i = 0; i < packet->length; i++)
    {
        printf("%02X ", (unsigned char)packet->buffer[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void print_packet(Packet *packet)
{
    if (packet->length == 0)
    {
        printf("数据包长度为0,无法解析\n");
        return;
    }
    printf("\n=== IP层解析结果 ===");
    printf("\n源IP: %s", packet->ip.src_ip);
    printf("\n目的IP: %s", packet->ip.dst_ip);
    printf("\nTTL: %d", packet->ip.ttl);
    printf("\n协议类型: %s",
           (packet->ip.protocol == IPPROTO_TCP) ? "TCP" : (packet->ip.protocol == IPPROTO_UDP) ? "UDP"
                                                                                               : "其他");
    // printf("\nIP标识符: %d", packet->ip.id);
    printf("\n数据包总长度: %d字节\n", packet->length);

    printf("\n=== 传输层解析结果 ===");
    printf("\n源端口: %d", packet->transport.src_port);
    printf("\n目的端口: %d\n", packet->transport.dst_port);
    printf("\n");
}