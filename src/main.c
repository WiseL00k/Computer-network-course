#include <stdio.h>
#include <unistd.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "adapter.h"
#include "parser.h"
#include "packet.h"

// 解析IP地址并打印

void clear_screen()
{
    printf("\033[2J\033[H"); // \033[2J 清屏，\033[H 移动光标到左上角
}

int main()
{
    WSADATA wsaData;

    char buffer[65536]; // 缓冲区，用于接收数据包
    Packet packet;

    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup失败: %d\n", WSAGetLastError());
        return 1;
    }

    print_local_ip();

    int num = -1;
    while (1)
    {
        printf("请输入要捕获的网络接口的IP地址对应序号（或按Ctrl+C退出）: ");
        scanf(" %d", &num);
        if (num < 0)
        {
            printf("无效的序号，请重新输入。\n");
            fflush(stdin);
            continue;
        }
        else
        {
            system("cls");
            break;
        }
    }

    char *ip = get_local_ip(num);
    printf("你选择的IP地址是: %s\n", ip);
    if (ip == NULL)
    {
        printf("获取本机IP地址失败\n");
        return 1;
    }

    Adapter adapter;
    if (adapter_init(&adapter, ip))
    {
        printf("适配器初始化失败\n");
        return 1;
    }

    printf("按任意键捕获数据包（按q退出）\n\n");
    puts("=========================");
    // 循环接收数据包
    while (1)
    {
        packet = adapter_receive_packet(&adapter);
        if (getch() != 'q')
        {
            clear_screen();
            puts("=========================");
            if (packet.length > 0)
            {
                parse_packet(&packet);
                print_packet(&packet);
                printf("\n捕获到数据包，长度: %d 字节\n", packet.length);
            }
            else
            {
                printf("接收错误: %d\n", WSAGetLastError());
            }
        }
        else
        {
            break;
        }
        puts("=========================");
        printf("按任意键捕获数据包（按q退出）\n\n");
    }

    // 清理资源
    adapter_close(&adapter);
    WSACleanup();

    system("pause"); // 暂停以查看输出
    return 0;
}
