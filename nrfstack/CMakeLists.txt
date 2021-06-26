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
set(LIBS ${WORK}/components/libraries)
set(EXT ${WORK}/external)
set(BLE ${WORK}/components/ble)

add_peer_directory(${PICOOS_DIR})
add_peer_directory(../nrfx)

if(NRF5_THINLOG)
set(LOCAL_LOG_SOURCE
    thinlog/thinlog.c
)
else()
set(LOCAL_LOG_SOURCE
    src/log_console.c
)
set(SDK_LOG_SOURCE
    ${LIBS}/memobj/nrf_memobj.c
    ${LIBS}/balloc/nrf_balloc.c
    ${LIBS}/ringbuf/nrf_ringbuf.c

    ${LIBS}/log/src/nrf_log_backend_rtt.c
    ${LIBS}/log/src/nrf_log_backend_serial.c
    ${LIBS}/log/src/nrf_log_frontend.c
    ${LIBS}/log/src/nrf_log_str_formatter.c
    ${EXT}/fprintf/nrf_fprintf.c
    ${EXT}/fprintf/nrf_fprintf_format.c
)

endif()

foreach(SRV_NAME ${NRF5_BLE_SERVICES})
  file(GLOB SRV_SRC ${BLE}/ble_services/${SRV_NAME}/*.c)
  list(APPEND BLE_SERVICE_SOURCE ${SRV_SRC})
  list(APPEND BLE_SERVICE_INCLUDE ${BLE}/ble_services/${SRV_NAME})
endforeach()

set(SDK_SOURCE ${SDK_LOG_SOURCE}
    ${LIBS}/util/app_error_handler_gcc.c
    ${LIBS}/util/app_error_weak.c
    ${LIBS}/util/app_error.c
    ${LIBS}/util/app_util_platform.c
    ${LIBS}/util/nrf_assert.c
    ${LIBS}/util/sdk_mapped_flags.c
    ${LIBS}/atomic/nrf_atomic.c
    ${LIBS}/pwr_mgmt/nrf_pwr_mgmt.c
    ${LIBS}/atomic_flags/nrf_atflags.c

    ${LIBS}/strerror/nrf_strerror.c
    ${LIBS}/experimental_section_vars/nrf_section_iter.c

    ${WORK}/components/softdevice/common/nrf_sdh.c
    ${WORK}/components/softdevice/common/nrf_sdh_ble.c
    ${WORK}/components/softdevice/common/nrf_sdh_soc.c
    ${WORK}/components/ble/ble_db_discovery/ble_db_discovery.c

    ${BLE}/common/ble_advdata.c
    ${BLE}/common/ble_srv_common.c
    ${BLE}/common/ble_conn_params.c
    ${BLE}/common/ble_conn_state.c

    ${BLE}/ble_advertising/ble_advertising.c
    ${BLE}/nrf_ble_gatt/nrf_ble_gatt.c
    ${BLE}/ble_link_ctx_manager/ble_link_ctx_manager.c

    ${BLE}/nrf_ble_qwr/nrf_ble_qwr.c
    ${BLE_SERVICE_SOURCE}
)

set(SOURCE
  src/app_timer.c
  src/nrf_sdh_picoos.c
  ${LOCAL_LOG_SOURCE}
  ${LOCAL_SOFTDEVICE_SOURCE}
  ${SDK_SOURCE}
)

add_custom_command(
  OUTPUT ${SDK_SOURCE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ../work
  COMMAND tar -xf ${DIST} -m -C ../work
  DEPENDS ${DIST}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Unpacking SDK sources")

add_library(nrfstack STATIC ${SOURCE})
target_link_libraries(nrfstack picoos nrfx)

target_include_directories(nrfstack
  PUBLIC include
  ${LIBS}/util
  ${LIBS}/strerror
  ${LIBS}/timer
  ${LIBS}/mutex
  ${LIBS}/pwr_mgmt
  ${WORK}/components/softdevice/${NRF5_SOFTDEVICE}/headers
  ${WORK}/components/softdevice/${NRF5_SOFTDEVICE}/headers/nrf52
  ${WORK}/components/softdevice/common
  ${WORK}/components/ble/common
  ${WORK}/components/ble/ble_db_discovery
  ${LIBS}/experimental_section_vars
  ${LIBS}/atomic
  ${LIBS}/atomic_flags
  ${BLE}/nrf_ble_qwr
  ${BLE}/nrf_ble_gatt
  ${BLE}/ble_link_ctx_manager
  ${BLE}/ble_advertising
  ${BLE_SERVICE_INCLUDE}
  PRIVATE ${DIR_CONFIG})

target_compile_definitions(nrfstack
  PUBLIC BLE_STACK_SUPPORT_REQD)

if(NRF5_THINLOG)

target_include_directories(nrfstack
  PUBLIC thinlog ${LIBS}/log)
target_compile_definitions(nrfstack
  PUBLIC NRF5_THINLOG)

else()

target_include_directories(nrfstack
  PUBLIC ${LIBS}/log
  ${LIBS}/memobj
  ${LIBS}/balloc
  ${LIBS}/ringbuf
  ${EXT}/fprintf
  ${LIBS}/log/src)

endif()

target_compile_options(nrfstack PUBLIC -Wno-expansion-to-defined
  PRIVATE -Wno-missing-declarations -Wno-unused-variable -Wno-missing-prototypes -Wno-unused-function -Wno-aggregate-return -Wno-bad-function-cast)
