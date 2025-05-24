#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>

#include "packet.h"

int parse_packet(Packet *packet);
int parse_ip_header(Packet *packet);
int parse_transport_layer(Packet *packet);
void print_raw_packet(Packet *packet);
void print_packet(Packet *packet);

#endif // PARSER_H