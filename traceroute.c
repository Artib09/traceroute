//Arti Brzozowski

#include "traceroute.h"

char rep_ip[20];
char reply[3][20];

//receive function
int receive_packets(int pid, int TTL, int sockfd){
    int recvd=0;
    struct timeval tvbefore, tvafter;
    int time=0;
    int totaltime=0;
    
    gettimeofday(&tvbefore, NULL);
    
    //waiting for packets to reach their destination or time out
    while(recvd<3 && time<1000){
        fd_set descriptors;
        FD_ZERO (&descriptors);
        FD_SET (sockfd, &descriptors);
        
        struct sockaddr_in 	sender;	
	    socklen_t 			sender_len = sizeof(sender);
	    u_int8_t 			buffer[IP_MAXPACKET];
	    ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
	    
        //receive error handling
	    if (packet_len < 0 && (errno!=EWOULDBLOCK || errno!=EAGAIN)) {
		    fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
		    return EXIT_FAILURE;
	    }
	    
	    //ICMP packet header
	    struct ip *iph = (struct ip *)buffer;
	    struct icmp *icmph = (struct icmp *)((uint8_t *)iph + (*iph).ip_hl *4);
	    
	    //packet reached its destination
	    if(icmph->icmp_type == ICMP_ECHOREPLY && icmph->icmp_seq==TTL && icmph->icmp_id==pid){
	        inet_ntop(AF_INET, &(sender.sin_addr), rep_ip, sizeof(rep_ip));
	        memcpy(reply[recvd], rep_ip, strlen(rep_ip)+1);
	        recvd++;
	        totaltime+=time;
	    }
	    //packet on its way to the destination, TTL exceeded -> read midway adress
	    else if(icmph->icmp_type == ICMP_TIME_EXCEEDED){
	        struct ip *iph1 = (struct ip *)((uint8_t *)icmph + 8);
		    struct icmp *icmph1 = (struct icmp *)((uint8_t *)iph1 + (*iph1).ip_hl * 4);
		    if(icmph1->icmp_seq==TTL && icmph1->icmp_id==pid){
	            inet_ntop(AF_INET, &(sender.sin_addr), rep_ip, sizeof(rep_ip));
	            memcpy(reply[recvd], rep_ip, strlen(rep_ip)+1);
	            recvd++;
	            totaltime+=time;
		    }
	    }
	    gettimeofday (&tvafter, NULL);
	    time = (tvafter.tv_sec - tvbefore.tv_sec)*1000 + (tvafter.tv_usec - tvbefore.tv_usec)/1000;
    }
    
    print_trr(TTL, recvd, reply, totaltime);
    
    return EXIT_SUCCESS;
}

int main (int argc, char* argv[]){
    if(argc>2){
        fprintf(stderr, "too many arguments\n");
        return EXIT_FAILURE;
    }
    
    //create socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		fprintf(stderr, "socket create error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
    
    //add the recipient's address into the address structure
    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    int inet_res = inet_pton(AF_INET, argv[1], &recipient.sin_addr);
    
    if(inet_res<=0){
        fprintf(stderr, "invalid input\n");
        return EXIT_FAILURE;
    }
    
    //traceroute
    int pid = getpid();
    
    for(int TTL=1; TTL<31; TTL++){
        if(send_3packets(pid, TTL, sockfd, recipient)<0){
            return EXIT_FAILURE;
        }
        
        if(receive_packets(pid, TTL, sockfd)<0){
            return EXIT_FAILURE;
        }
        
        if(strcmp(rep_ip, argv[1])==0){
            break;
        }
    }
    return 0;
}
