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
#ifndef NRF_LOG_H_
#define NRF_LOG_H_

#include <picoos.h>
#include "nrf_log_types.h"
#include "sdk_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NRF_LOG_ENABLED

/**
 * Simple thin logging layer using pico]OS print functions.
 */
#ifndef NRF_LOG_LEVEL
    #define NRF_LOG_LEVEL NRF_LOG_DEFAULT_LEVEL
#endif

#define _NRF_LOG_FMT(level, fmt, ...)   "%s: " fmt "\n", thinlogSeverityNames[level]
#define _NRF_LOG_ARGS(fmt, ...)         , ##__VA_ARGS__
#define _NRF_LOG_PRINT(level, ...)      { if (NRF_LOG_LEVEL >= level) { \
                                              logPrintf(level, __VA_ARGS__); \
                                          } \
                                        }

#define NRF_LOG_ERROR(...)              _NRF_LOG_PRINT(NRF_LOG_SEVERITY_ERROR, __VA_ARGS__)
#define NRF_LOG_WARNING(...)            _NRF_LOG_PRINT(NRF_LOG_SEVERITY_WARNING, __VA_ARGS__)
#define NRF_LOG_INFO(...)               _NRF_LOG_PRINT(NRF_LOG_SEVERITY_INFO, __VA_ARGS__)
#define NRF_LOG_DEBUG(...)              _NRF_LOG_PRINT(NRF_LOG_SEVERITY_DEBUG, __VA_ARGS__)

#define NRF_LOG_PUSH(_str)                     (_str)
#define NRF_LOG_MODULE_REGISTER() 

extern const char* thinlogSeverityNames[];
void logPrintf(int level, const char* fmt, ...);

#else

#define NRF_LOG_ERROR(...)
#define NRF_LOG_WARNING(...)
#define NRF_LOG_INFO(...)
#define NRF_LOG_DEBUG(...)

#define NRF_LOG_PUSH(_str)                     (_str)
#define NRF_LOG_MODULE_REGISTER() 

#endif

#define NRF_LOG_INST_DEBUG(...)
#define NRF_LOG_INST_INFO(...)

#ifdef __cplusplus
}
#endif

#endif // NRF_LOG_H_

