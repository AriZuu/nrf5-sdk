/*
 * Copyright (c) 2021, Ari Suutari <ari@stonepile.fi>.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdk_common.h"

#if NRF_MODULE_ENABLED(NRF_LOG)
#include "nrf_log_backend_serial.h"
#include "nrf_log_internal.h"
#include "app_error.h"
#include "log_console.h"
#include <picoos.h>

/**
 * A very simple log backend for NRF SDK which uses nano layer print functions.
 */

#define BUFSIZE 80
static uint8_t m_string_buff[BUFSIZE];

void nrf_log_backend_console_init(void)
{
}

static void console_tx(void const * p_context, char const * p_buffer, size_t len)
{
  while (len--) {

   if (*p_buffer == '\n')
     nosPrintChar('\r');

   nosPrintChar(*p_buffer);
   ++p_buffer;
  }
}

static void nrf_log_backend_console_put(nrf_log_backend_t const * p_backend,
                                     nrf_log_entry_t * p_msg)
{
  nrf_log_backend_serial_put(p_backend, 
                             p_msg, m_string_buff,
                             BUFSIZE, console_tx);
}

static void nrf_log_backend_console_flush(nrf_log_backend_t const * p_backend)
{
}

static void nrf_log_backend_console_panic_set(nrf_log_backend_t const * p_backend)
{
}

const nrf_log_backend_api_t nrf_log_backend_console_api = {
        .put       = nrf_log_backend_console_put,
        .flush     = nrf_log_backend_console_flush,
        .panic_set = nrf_log_backend_console_panic_set,
};

#endif //NRF_MODULE_ENABLED(NRF_LOG)
