#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include "network_app.h"
#include "timer_ctrl.h"
#define PACKETSIZE  64
struct packet
{
    struct icmphdr hdr;
    char msg[PACKETSIZE-sizeof(struct icmphdr)];
};

unsigned short 
NetworkApp::check_sum(void *b, int len)
{
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

/*--------------------------------------------------------------------*/
/*--- ping - Create message and send it.                           ---*/
/*    return 0 is ping Ok, return 1 is ping not OK.                ---*/
/*--------------------------------------------------------------------*/
bool 
NetworkApp::ping(std::string adress)
{
	int pid;
    const int val=255;
    int i, sd;
    struct packet pckt;
    struct sockaddr_in r_addr;
    int loop;
    struct hostent *hname;
    struct sockaddr_in addr_ping,*addr;
    struct protoent *proto;

    pid = getpid();
    proto = getprotobyname("ICMP");
    hname = gethostbyname(adress.c_str());
    bzero(&addr_ping, sizeof(addr_ping));
    addr_ping.sin_family = hname->h_addrtype;
    addr_ping.sin_port = 0;
    addr_ping.sin_addr.s_addr = *(long*)hname->h_addr;

    addr = &addr_ping;

    sd = socket(PF_INET, SOCK_RAW, proto->p_proto);
    if ( sd < 0 )
    {
        perror("socket");
        return false;
    }
    if ( setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
    {
        perror("Set TTL option");
        close(sd);
        return false;
    }
    if ( fcntl(sd, F_SETFL, O_NONBLOCK) != 0 )
    {
        perror("Request nonblocking I/O");
        close(sd);
        return false;
    }

    for (loop=0;loop < 5; loop++)
    {
		static int cnt=1;
        int len=sizeof(r_addr);

        if ( recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, (socklen_t *)&len) > 0 )
        {
			close(sd);
            return true;
        }

        bzero(&pckt, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = pid;
        for ( i = 0; i < (int)(sizeof(pckt.msg)-1); i++ )
            pckt.msg[i] = i+'0';
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = cnt++;
        pckt.hdr.checksum = check_sum(&pckt, sizeof(pckt));
        
        //if ( sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr)) <= 0 )
        //    perror("sendto");
		sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr));

        //usleep(300000);
        TimerCtrl::delay_ms(300);
    }
    
	close(sd);
    return false;
}

NetworkApp::NetworkApp(void)
{
}

NetworkApp::~NetworkApp(void)
{
}
