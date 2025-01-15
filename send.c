//Arti Brzozowski

#include "traceroute.h"
#define ICMP_ECHO 8

//send single ICMP packet
int send_packet(int pid, int TTL, int sockfd, struct sockaddr_in rec){
    //create ICMP packet, only header is needed
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = pid;
    header.icmp_hun.ih_idseq.icd_seq = TTL;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));
    
    //get direct access to TTL
    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &TTL, sizeof(int));
    
    //send packet through the socket
    ssize_t bytes_sent = sendto(
    sockfd,
    &header,
    sizeof(header),
    0,
    (struct sockaddr*)&rec,
    sizeof(rec)
    );
    
    if(bytes_sent < 0){
        fprintf(stderr, "packet send error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return 0;
}

//Send 3 ICMP packets (uses function send_packet)
int send_3packets(int pid, int TTL, int sockfd, struct sockaddr_in rec){
    int res;
    for(int i=0; i<3; i++){
        res = send_packet(pid, TTL, sockfd, rec);
        if(res<0){
            return EXIT_FAILURE;
        }
    }
    
    return 0;
}
