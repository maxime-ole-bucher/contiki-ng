/*
 * Copyright (c) 2012, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "contiki.h"
#include "contiki-net.h"
#include "ip64/ip64-dhcpc.h"
#include "ip64/ip64.h"
#include "ip64/ip64-eth.h"
#include "ipv6/ip64-addr.h"

/*---------------------------------------------------------------------------*/

#include "sys/log.h"

#define LOG_MODULE  "IP64"
#define LOG_LEVEL   LOG_LEVEL_IP64

/*---------------------------------------------------------------------------*/

PROCESS(ip64_ipv4_dhcp_process,
"IPv4 DHCP");

uip_ipaddr_t uip_hostaddr; /* Needed because it is referenced by dhcpc.c */


/*---------------------------------------------------------------------------*/
void
ip64_ipv4_dhcp_init(void) {
    LOG_INFO("Starting DHCPv4\n");
    process_start(&ip64_ipv4_dhcp_process, NULL);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ip64_ipv4_dhcp_process, ev, data
)
{
PROCESS_BEGIN();

ip64_dhcpc_init(&ip64_eth_addr, sizeof(ip64_eth_addr));

LOG_INFO("IPv4 DHCP Inited\n");

ip64_dhcpc_request();
LOG_DBG("IPv4 DHCP Requested\n");
while(1) {
PROCESS_WAIT_EVENT();

if(ev == tcpip_event ||
ev == PROCESS_EVENT_TIMER) {
ip64_dhcpc_appcall(ev, data
);
}
}

PROCESS_END();

}

/*---------------------------------------------------------------------------*/
void
ip64_dhcpc_configured(const struct ip64_dhcpc_state *s) {
    uip_ip6addr_t ip6dnsaddr;
    LOG_INFO("DHCP Configured with %d.%d.%d.%d\n",
             s->ipaddr.u8[0], s->ipaddr.u8[1],
             s->ipaddr.u8[2], s->ipaddr.u8[3]);

    ip64_set_hostaddr((uip_ip4addr_t *) &s->ipaddr);
    ip64_set_netmask((uip_ip4addr_t *) &s->netmask);
    ip64_set_draddr((uip_ip4addr_t *) &s->default_router);
    if (!uip_ip4addr_cmp((uip_ip4addr_t *) &s->dnsaddr, &uip_all_zeroes_addr)) {
        /* Note: Currently we assume only one DNS server */
        uip_ipaddr_t *dns = uip_nameserver_get(0);
        /* Only update DNS entry if it is empty or already IPv4 */
        if (uip_is_addr_unspecified(dns) || ip64_addr_is_ip64(dns)) {
            ip64_addr_4to6((uip_ip4addr_t *) &s->dnsaddr, &ip6dnsaddr);
            uip_nameserver_update(&ip6dnsaddr, uip_ntohs(s->lease_time[0]) * 65536ul + uip_ntohs(s->lease_time[1]));
        }
    }
}

/*---------------------------------------------------------------------------*/
void
ip64_dhcpc_unconfigured(const struct ip64_dhcpc_state *s) {
}
/*---------------------------------------------------------------------------*/
