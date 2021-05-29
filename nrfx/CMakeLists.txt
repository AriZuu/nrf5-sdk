#
# Copyright (c) 2021, Ari Suutari <ari@stonepile.fi>.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. The name of the author may not be used to endorse or promote
#     products derived from this software without specific prior written
#     permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

include_guard(GLOBAL)

set(DIST ${CMAKE_CURRENT_LIST_DIR}/../../distfiles/nRF5SDK1702d674dde.zip)

set(WORK ${CMAKE_CURRENT_SOURCE_DIR}/../work/nRF5_SDK_17.0.2_d674dde)
set(NRFX ${WORK}/modules/nrfx)
set(NRFX_DRIVERS ${NRFX}/drivers/src)
set(COMPONENTS ${WORK}/components)


if(NRF5_CHIP STREQUAL "NRF51")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf51.c)
elseif(NRF5_CHIP STREQUAL "NRF52810_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52810.c)
elseif(NRF5_CHIP STREQUAL "NRF52811_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52811.c)
elseif(NRF5_CHIP STREQUAL "NRF52820_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52820.c)
elseif(NRF5_CHIP STREQUAL "NRF52832_XXAA" OR NRF5_CHIP STREQUAL "NRF52832_XXAB")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52.c)
elseif(NRF5_CHIP STREQUAL "NRF52833_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52833.c)
elseif(NRF5_CHIP STREQUAL "NRF52840_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf52840.c)
elseif(NRF5_CHIP STREQUAL "NRF9160_XXAA")
    set(CHIP_SRC ${NRFX}/mdk/system_nrf9160.c)
else()
    message( FATAL_ERROR "Unknown chip ${NRF5_CHIP}.")
endif()

set(SOURCE
    ${CHIP_SRC}
    ${NRFX_DRIVERS}/nrfx_adc.c
    ${NRFX_DRIVERS}/nrfx_clock.c
    ${NRFX_DRIVERS}/nrfx_comp.c
    ${NRFX_DRIVERS}/nrfx_dppi.c
    ${NRFX_DRIVERS}/nrfx_gpiote.c
    ${NRFX_DRIVERS}/nrfx_i2s.c
    ${NRFX_DRIVERS}/nrfx_lpcomp.c
    ${NRFX_DRIVERS}/nrfx_nfct.c
    ${NRFX_DRIVERS}/nrfx_nvmc.c
    ${NRFX_DRIVERS}/nrfx_pdm.c
    ${NRFX_DRIVERS}/nrfx_power.c
    ${NRFX_DRIVERS}/nrfx_ppi.c
    ${NRFX_DRIVERS}/nrfx_pwm.c
    ${NRFX_DRIVERS}/nrfx_qdec.c
    ${NRFX_DRIVERS}/nrfx_qspi.c
    ${NRFX_DRIVERS}/nrfx_rng.c
    ${NRFX_DRIVERS}/nrfx_rtc.c
    ${NRFX_DRIVERS}/nrfx_saadc.c
    ${NRFX_DRIVERS}/nrfx_spi.c
    ${NRFX_DRIVERS}/nrfx_spim.c
    ${NRFX_DRIVERS}/nrfx_spis.c
    ${NRFX_DRIVERS}/nrfx_swi.c
    ${NRFX_DRIVERS}/nrfx_systick.c
    ${NRFX_DRIVERS}/nrfx_temp.c
    ${NRFX_DRIVERS}/nrfx_timer.c
#    ${NRFX_DRIVERS}/nrfx_twi_twim.c
    ${NRFX_DRIVERS}/nrfx_twi.c
    ${NRFX_DRIVERS}/nrfx_twim.c
    ${NRFX_DRIVERS}/nrfx_twis.c
    ${NRFX_DRIVERS}/nrfx_uart.c
    ${NRFX_DRIVERS}/nrfx_uarte.c
    ${NRFX_DRIVERS}/nrfx_usbd.c
    ${NRFX_DRIVERS}/nrfx_wdt.c)

add_custom_command(
  OUTPUT ${SOURCE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ../work
  COMMAND tar -xf ${DIST} -m -C ../work
  DEPENDS ${DIST}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Unpacking SDK sources")

add_library(nrfx STATIC ${SOURCE})

target_include_directories(nrfx
  PUBLIC ${NRFX}/mdk
  ${NRFX}
  ${NRFX}/hal
  ${NRFX}/drivers/include
  ${COMPONENTS}/toolchain/cmsis/include
  PRIVATE ${DIR_CONFIG})

if(DEFINED NRF5_SOFTDEVICE)
   target_include_directories(nrfx PUBLIC ${COMPONENTS}/softdevice/${NRF5_SOFTDEVICE}/headers)
   string(TOUPPER ${NRF5_SOFTDEVICE} SDNAME)
   target_compile_definitions(nrfx
     PUBLIC
     ${SDNAME}
     NRF_SD_BLE_API_VERSION=7
     SOFTDEVICE_PRESENT)
else()
   target_include_directories(nrfx PUBLIC ${COMPONENTS}/drivers_nrf/nrf_soc_nosd)
endif()

target_compile_definitions(nrfx
  PUBLIC ${NRF5_CHIP} ${NRF5_DEFINES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(nrfx
  PUBLIC DEBUG)
endif()

target_compile_options(nrfx PUBLIC -Wno-cast-align)
