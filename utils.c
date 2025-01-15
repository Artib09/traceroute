//Arti Brzozowski

#include "traceroute.h"

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void print_trr(int TTL, int recvd, char reply[3][20], int totaltime){
    printf("%d. ", TTL);
    if (recvd == 0) {
        printf("*");
    } else if (recvd==1) {
        printf("%s ???", reply[0]);
    } else if (recvd==2) {
        if(strcmp(reply[0],reply[1])!=0){
            printf("%s %s ???", reply[0], reply[1]);
        }
        else{
            printf("%s ???", reply[0]);
        }
    } else if (recvd==3) {
        if(strcmp(reply[0],reply[1])!=0 && strcmp(reply[0],reply[2])!=0){
            printf("%s %s %s %dms", reply[0], reply[1], reply[2], totaltime/3);
        }
        if(strcmp(reply[0],reply[1])==0 && strcmp(reply[0],reply[2])!=0){
            printf("%s %s %dms", reply[1], reply[2], totaltime/3);
        }
        if(strcmp(reply[0],reply[1])!=0 && strcmp(reply[0],reply[2])==0){
            printf("%s %s %dms", reply[0], reply[1], totaltime/3);
        }
        if(strcmp(reply[0],reply[1])==0 && strcmp(reply[0],reply[1])==0){
            printf("%s %dms", reply[0], totaltime/3);
        }
    }
    printf("\n");
}
