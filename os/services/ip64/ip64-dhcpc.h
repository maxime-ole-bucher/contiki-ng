/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
#ifndef IP64_DHCPC_H_
#define IP64_DHCPC_H_

struct ip64_dhcpc_state {
    struct pt pt;
    char state;
    struct uip_udp_conn *conn;
    struct etimer etimer;
    uint32_t ticks;
    const void *mac_addr;
    int mac_len;

    uint8_t serverid[4];

    uint16_t lease_time[2];
    uip_ipaddr_t ipaddr;
    uip_ipaddr_t netmask;
    uip_ipaddr_t dnsaddr;
    uip_ipaddr_t default_router;
};

void ip64_dhcpc_init(const void *mac_addr, int mac_len);

void ip64_dhcpc_request(void);

void ip64_dhcpc_appcall(process_event_t ev, void *data);

/* Mandatory callbacks provided by the user. */
void ip64_dhcpc_configured(const struct ip64_dhcpc_state *s);

void ip64_dhcpc_unconfigured(const struct ip64_dhcpc_state *s);

#endif /* IP64_DHCPC_H_ */
