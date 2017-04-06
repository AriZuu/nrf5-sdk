/*
 * Copyright (c) 2016-2017, Ari Suutari <ari@stonepile.fi>.
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

#if NRF_MODULE_ENABLED(APP_TIMER)

#include <picoos.h>

#if !POSCFG_FEATURE_TIMERCALLBACK
#error POSCFG_FEATURE_TIMERCALLBACK must be set to 1 in poscfg.h
#endif

#include <stdbool.h>

#include "app_timer.h"
#include <stdlib.h>
#include <string.h>
#include "nrf.h"
#include "app_error.h"

typedef struct {

  POSTIMER_t                  timer;
  bool                        reload;
  app_timer_timeout_handler_t handler;
  void*                       arg;
} Timer;

/*
 * Check that timer type fits, as it's storage
 * size is declared elsewhere.
 */
STATIC_ASSERT(sizeof(Timer) <= sizeof(app_timer_t));

static app_timer_evt_schedule_func_t eventScheduleFunc;

static void timerCallback(POSTIMER_t timer, void* arg)
{
  Timer* t = (Timer*)arg;

  if (eventScheduleFunc != NULL) {
 
    uint32_t err = eventScheduleFunc(t->handler, t->arg);
    APP_ERROR_CHECK(err);
  }
  else
    t->handler(t->arg);
}

/*
 * Initialize app timer system.
 */
uint32_t app_timer_init(uint32_t prescaler,
                        uint8_t  queueSize,
                        void*    buffer,
                        app_timer_evt_schedule_func_t scheduleFunc)
{
  eventScheduleFunc = scheduleFunc;
  return NRF_SUCCESS;
}

/*
 * Create new application timer.
 */
uint32_t app_timer_create(app_timer_id_t const       *timerId,
                          app_timer_mode_t            mode,
                          app_timer_timeout_handler_t handler)
{
  if (handler == NULL || timerId == NULL)
    return NRF_ERROR_INVALID_PARAM;

  Timer* t = (Timer*)(*timerId);

  if (t->timer != NULL)
    return NRF_ERROR_INVALID_STATE;

  t->timer  = posTimerCreate();
  if (t->timer == NULL)
    return NRF_ERROR_NO_MEM;
  
  t->reload = (mode != APP_TIMER_MODE_SINGLE_SHOT);
  t->handler = handler;

  return NRF_SUCCESS;
}
                     
/*
 * Make timer tick.
 */

uint32_t app_timer_start(app_timer_id_t timerId,
                         uint32_t timeout,
                         void* context)
{
  Timer* t = (Timer*)timerId;

  if (t->timer == NULL)
    return NRF_ERROR_INVALID_STATE;

  t->arg = context;
  posTimerCallbackSet(t->timer, timerCallback, t, timeout, t->reload ? timeout : 0);
  posTimerStart(t->timer);
  return NRF_SUCCESS;
}

/*
 * Stop timer.
 */
uint32_t app_timer_stop(app_timer_id_t timerId)
{
  Timer* t = (Timer*)timerId;

  if (t->timer == NULL)
    return NRF_ERROR_INVALID_STATE;

  posTimerStop(t->timer);
  t->arg = NULL;
  return NRF_SUCCESS;
}

#endif //NRF_MODULE_ENABLED(APP_TIMER)
