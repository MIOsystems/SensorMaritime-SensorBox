/*********************************************************************************************************************
* DAVE APP Name : ETH_LWIP       APP Version: 4.1.12
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SYS                     1
#define LWIP_SOCKET                0
#define LWIP_NETCONN               0
#define SYS_LIGHTWEIGHT_PROT        0
#define SO_REUSE                    0
#define MEM_ALIGNMENT               4

#define LWIP_NETIF_HOSTNAME         0
#define LWIP_NETIF_STATUS_CALLBACK  0
#define LWIP_NETIF_LINK_CALLBACK    1
#define ETH_PAD_SIZE              2
#define TCP_LISTEN_BACKLOG        1
#define LWIP_SO_RCVTIMEO          1
#define MEM_SIZE                1600
#define PBUF_POOL_SIZE          16
#define PBUF_POOL_BUFSIZE       1536

#define MEMP_NUM_PBUF           16

#define MEMP_NUM_RAW_PCB        4


#define LWIP_UDP                1
#define MEMP_NUM_UDP_PCB        4

#define LWIP_TCP                1
#define MEMP_NUM_TCP_PCB        5
#define MEMP_NUM_TCP_PCB_LISTEN  5
#define TCP_MSS                 536
#define TCP_WND                 2144
#define TCP_SND_BUF             1072
#define TCP_SND_QUEUELEN        8

#define LWIP_DHCP               0

#define LWIP_ARP                1
#define ARP_TABLE_SIZE          10
#define MEMP_NUM_ARP_QUEUE      30

#define LWIP_AUTOIP             0

#define LWIP_ICMP               1

#define LWIP_IGMP               0

#define LWIP_DNS                0

#define LWIP_SNMP               0

#define IP_OPTIONS_ALLOWED      0

#define IP_FRAG                 0

#define IP_REASSEMBLY           0

#define LWIP_NOASSERT           1

#define LWIP_STATS              0
#define LWIP_STATS_DISPLAY      0

#define MEMP_NUM_TCP_SEG        8
#define MEMP_NUM_REASSDATA      5
#define IP_DEFAULT_TTL          255


#define CHECKSUM_GEN_IP         0
#define CHECKSUM_GEN_UDP        0
#define CHECKSUM_GEN_TCP        0
#define CHECKSUM_GEN_ICMP       0
#define CHECKSUM_CHECK_IP       0
#define CHECKSUM_CHECK_UDP      0
#define CHECKSUM_CHECK_TCP      0
#define CHECKSUM_CHECK_ICMP     0

#define LWIP_RAW                1
#define MEMP_NUM_SYS_TIMEOUT    (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 8)


#ifdef __cplusplus
}
#endif
#endif /* __LWIPOPTS_H__ */

