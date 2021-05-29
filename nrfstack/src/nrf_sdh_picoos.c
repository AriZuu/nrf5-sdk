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

#include <picoos.h>
#include "nrf_sdh_picoos.h"
#include "nrf_sdh.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "nrfx_config.h"

#define NRF_LOG_MODULE_NAME nrf_sdh_picoos
#include "nrf_log.h"

NRF_LOG_MODULE_REGISTER();

#define SDH_TASK_STACK 2048

static POSFLAG_t  softdeviceEventFlag;
static POSMUTEX_t stackMutex;

/**
 * Handle interrupt from softdevice.
 */
void SD_EVT_IRQHandler()
{
  c_pos_intEnter();
  posFlagSet(softdeviceEventFlag, 0);
  c_pos_intExitQuick();
}

/**
 * Task to pump softdevice events to application.
 */
static void softdeviceTask(void* arg)
{
  NRF_LOG_DEBUG("Softdevice task start.");

  while (true) {

    posMutexLock(stackMutex);
    nrf_sdh_evts_poll();
    posMutexUnlock(stackMutex);
    posFlagGet(softdeviceEventFlag, POSFLAG_MODE_GETSINGLE);
  }
}

/**
 * Lock softdevice task mutex. After locking, other
 * threads are allowed to call BLE stack functions.
 */
void nrf_sdh_lock()
{
  posMutexLock(stackMutex);
}

/**
 * Unlock softdevice task mutex.
 */
void nrf_sdh_unlock()
{
  posMutexUnlock(stackMutex);
}

/**
 * Start task for softdevice event pumping.
 */
void nrf_sdh_picoos_start()
{
  POSTASK_t task;

  NRF_LOG_DEBUG("Creating a SoftDevice task.");

  softdeviceEventFlag = posFlagCreate();
  stackMutex = posMutexCreate();
  task = posTaskCreate(softdeviceTask, NULL, POSCFG_MAX_PRIO_LEVEL - 1, SDH_TASK_STACK);
  if (task == NULL) {

    NRF_LOG_ERROR("SoftDevice task not created.");
    APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
  }

  POS_SETTASKNAME(task, "sdh");
}
