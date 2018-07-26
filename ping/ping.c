#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#define BUF_SZ 1024
#define PACK_LEN 56
int sendnum = 0;
int recv_packet = 0;
char sendpack[BUF_SZ];
char recvpack[BUF_SZ];
long diftime(const struct timeval *end, const struct timeval *bg)
{
   return (end->tv_sec - bg->tv_sec) * 1000 + (end->tv_usec - bg->tv_usec) / 1000;
}

unsigned short chksum(unsigned short *addr, int len)
{
    unsigned int ret = 0;
    while(len > 1)
    {
        ret += *addr++;
        len -= 2;
    }
    if(len == 1)
    {
        ret += *(unsigned char*)addr;
    }
    ret = (ret >> 16) + (ret & 0xffff);
    ret += (ret >> 16);
    return (unsigned short) ~ret;
}

int pack(int num, pid_t pid)
{
    memset(sendpack, 0x00, sizeof sendpack);
    struct icmp *p = (struct icmp*)sendpack;
    p->icmp_type = ICMP_ECHOREPLY;
    p->icmp_code = 0;
    p->icmp_cksum = 0;
    p->icmp_seq = num;
    p->icmp_id = pid;
    struct timeval tval;
    gettimeofday(&tval, NULL);
    memcpy((void*)p->icmp_data,(void*)&tval, sizeof tval);


    p->icmp_cksum = cksum((unsigned short*)sendpack, PACK_LEN + 8);
    return PACK_LEN + 8;
}

void send_packet(int sfd, pid_t pid, struct sockaddr_in ad)
{
    sendnum++;
    int r = pack(sendnum, pid);
    sendto(sfd, sendpack, r, 0, (struct sockaddr*)&ad, sizeof ad);
}
void unpack(unsigned short *buf, int len, pid_t pid)
{
    struct ip *pip = (struct ip*)buf;
    struct icmp *picmp = (struct icmp*)(buf + (pip->ip_hl << 2));
    if(picmp->icmp_id == pid)


}

void recv_packet(int sfd, pid_t pid)
{
    memset(recvpack, 0x00, sizeof recvpack);
    struct sockaddr_in from;
    socklen_t len = sizeof from;
    int r;
    r = recvfrom(sfd, recvpack, BUF_SZ, 0, (struct sockaddr*)&from, &len);
    unpack(recvpack, r, pid);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        return 1;
    struct in_addr addr;
    struct sockaddr_in ad;
    if((addr.s_addr = inet_addr(argv[1])) == INADDR_NONE)
    {
        struct hostent *pent = gethostbyname(argv[1]);
        if(pent == NULL)
            perror("gethostbyname"),exit(1);
        memcpy((char*)&addr, (char*)pent->h_addr, pent->h_length);
    }
    int sfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sfd == -1)
    {
        perror("socket");
        exit(1);
    }
    ad.sin_family = AF_INET;
    ad.sin_addr = addr;
    pid_t pid = getpid();

    while(1)
    {
        send_packet(sfd, pid,ad);
        recv_packet(sfd, pid);
        sleep(1);
    }
}
