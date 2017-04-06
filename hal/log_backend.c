/*
 * Copyright (c) 2016, Ari Suutari <ari@stonepile.fi>.
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

#include "picoos.h"
#include "sdk_common.h"

#if NRF_MODULE_ENABLED(NRF_LOG)

#include "nrf_log_backend.h"
#include "nrf_error.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

#define HEXDUMP_BYTES_PER_LINE               16

ret_code_t nrf_log_backend_init(bool blocking)
{
  return NRF_SUCCESS;
}

static bool stdHandler(uint8_t                severityLevel,
                       const uint32_t * const timeStamp,
                       const char * const     str,
                       uint32_t               *args,
                       uint32_t               nargs)
{
  if (timeStamp != NULL)
    printf("[%0" NUM_TO_STR(NRF_LOG_TIMESTAMP_DIGITS) PRIu32 "]", *timeStamp);

  switch (nargs) {
    case 0:
      printf("%s", str);
      break;

    case 1:
      printf(str, args[0]);
      break;

    case 2:
      printf(str, args[0], args[1]);
      break;

    case 3:
      printf(str, args[0], args[1], args[2]);
      break;

    case 4:
      printf(str, args[0], args[1], args[2], args[3], args[4]);
      break;

    case 5:
      printf(str, args[0], args[1], args[2], args[3], args[4], args[5]);
      break;

    case 6:
      printf(str, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
      break;
  }

  return true;
}

static uint32_t hexdumpHandler(uint8_t                severityLevel,
                               const uint32_t * const timeStamp,
                               const char * const     str,
                               uint32_t               offset,
                               const uint8_t * const  buf0,
                               uint32_t               buf0Length,
                               const uint8_t * const  buf1,
                               uint32_t               buf1Length)
{
  uint32_t max = buf0Length + buf1Length;
  uint32_t chunkPos = 0;
  uint32_t pos;
  const uint8_t* ptr;
  int i;

  if (timeStamp != NULL)
    printf("[%0" NUM_TO_STR(NRF_LOG_TIMESTAMP_DIGITS) PRIu32 "]", *timeStamp);

  printf("%s", str);

  while (chunkPos < max) {

    if (timeStamp != NULL)
      printf(" %" NUM_TO_STR(NRF_LOG_TIMESTAMP_DIGITS) "s ", "");

// Print chunk in hex.

    pos = chunkPos;
    for (i = 0; i < HEXDUMP_BYTES_PER_LINE; i++) {

      if (pos < buf0Length)
        ptr = buf0 + pos;
      else
        ptr = buf1 + pos - buf0Length;

      if (pos < max)
        printf("%02X ", *ptr);
      else
        printf("   ");

      ++pos;
    }

// Print chunk in ascii.

    pos = chunkPos;
    for (i = 0; i < HEXDUMP_BYTES_PER_LINE; i++) {

      if (pos < buf0Length)
        ptr = buf0 + pos;
      else
        ptr = buf1 + pos - buf0Length;

      if (pos < max)
        printf("%c", isprint(*ptr) ? *ptr : '.');
      else
        break;

      ++pos;
    }

    printf("\n");
    chunkPos += HEXDUMP_BYTES_PER_LINE;
  }

  return max;
}

nrf_log_std_handler_t nrf_log_backend_std_handler_get(void)
{
    return stdHandler;
}


nrf_log_hexdump_handler_t nrf_log_backend_hexdump_handler_get(void)
{
    return hexdumpHandler;
}


uint8_t nrf_log_backend_getchar(void)
{
    return getchar();
}

#endif /* NRF_MODULE_ENABLED(NRF_LOG)*/
