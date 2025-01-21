#TRACEROUTE
# How it works
Simple traceroute prints route from your computer to the given address. Program uses raw sockets to send 3 ICMP packets with increasing time to live (TTL). When TTL is 0 router will send time limit exceeded packet, then I can use this packet to read last router address.

# How to run
- Compile the program with make
- Run ./traceroute «IPv4 address» as root
