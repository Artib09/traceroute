#ifndef traceroute_h
#define traceroute_h

#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#define error(x) if(x<0) return -1

int receive_packets(int pid, int TTL, int sockfd);
u_int16_t compute_icmp_checksum(const void *buff, int length);
int send_packet(int pid, int TTL, int sockfd, struct sockaddr_in rec);
int send_3packets(int pid, int TTL, int sockfd, struct sockaddr_in rec);
//print for traceroute
void print_trr(int TTL, int recvd, char reply[3][20], int totaltime);
#endif
