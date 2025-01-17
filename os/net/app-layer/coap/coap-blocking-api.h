/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 */

/**
 * \addtogroup coap
 * @{
 */

#ifndef COAP_BLOCKING_API_H_
#define COAP_BLOCKING_API_H_

#include "sys/pt.h"
#include "coap-transactions.h"
#include "coap-request-state.h"

/*---------------------------------------------------------------------------*/
/*- Client Part -------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
typedef struct coap_blocking_request_state {
    coap_request_state_t state;
    struct pt pt;
    struct process *process;
} coap_blocking_request_state_t;

typedef void (*coap_blocking_response_handler_t)(coap_message_t *response);

PT_THREAD(coap_blocking_request
                  (coap_blocking_request_state_t * blocking_state, process_event_t ev,
        coap_endpoint_t * remote,
        coap_message_t * request,
        coap_blocking_response_handler_t request_callback));

#define COAP_BLOCKING_REQUEST(server_endpoint, request, chunk_handler)  \
  {                                                                     \
    static coap_blocking_request_state_t blocking_state;                          \
    PT_SPAWN(process_pt, &blocking_state.pt,                             \
             coap_blocking_request(&blocking_state, ev,                  \
                                   server_endpoint,                     \
                                   request, chunk_handler)              \
             );                                                         \
  }

#endif /* COAP_BLOCKING_API_H_ */
/** @} */
