#
# Copyright (c) 2016, Ari Suutari <ari@stonepile.fi>.
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

RELROOT = ../../picoos/
PORT ?= cortex-m
BUILD ?= RELEASE

include $(RELROOT)make/common.mak

TARGET ?= cmsis

SRC_TXT +=	../sdk/components/toolchain/system_nrf51.c log_backend.c
SRC_HDR +=	../sdk/components/sdk_validation.h
SRC_OBJ +=

#
# There are a lot of compiler warnings in NRF5 SDK.
# Silence those for now and hope that situation gets better
# in new versions.
#
EXTRA_CFLAGS =	-Wno-cast-align \
		-Wno-missing-prototypes \
		-Wno-bad-function-cast \
		-Wno-sign-compare \
		-Wno-missing-field-initializers

NRF5_DEFINES ?= NRF51 NRF51822

#
# This is always needed.
#
DIR_USRINC +=	../sdk/components/device \
		../sdk/components/toolchain \
		../sdk/components/toolchain/cmsis/include \
		../sdk/components/drivers_nrf/hal

#
# Driver sources and headers.
#
SRC_TXT +=  	$(foreach D, \
      		$(NRF5_DRIVERS), \
      		$(wildcard ../sdk/components/drivers_nrf/$(D)/*.c))

DIR_USRINC += 	$(foreach D, \
      		$(NRF5_DRIVERS), \
      		../sdk/components/drivers_nrf/$(D))

#
# Minimal libraries: util & logging
#
SRC_TXT +=  	$(wildcard ../sdk/components/libraries/util/*.c) \
		../sdk/components/libraries/log/src/nrf_log_frontend.c

DIR_USRINC +=	../sdk/components/libraries/util \
		../sdk/components/libraries/log \
		../sdk/components/libraries/log/src

#
# Softdevice headers
#
ifeq '$(strip $(NRF5_SOFTDEVICE))' ''
DIR_USRINC +=	../sdk/components/drivers_nrf/nrf_soc_nosd
else
DIR_USRINC +=	../sdk/components/softdevice/$(NRF5_SOFTDEVICE)/headers \
		../sdk/components/softdevice/common/softdevice_handler

NRF5_SOFTDEVICE_UPPER := $(shell echo $(NRF5_SOFTDEVICE) | tr '[:lower:]' '[:upper:]')
CDEFINES += SOFTDEVICE_PRESENT $(NRF5_SOFTDEVICE_UPPER)
endif

CDEFINES += $(NRF5_DEFINES)
ifeq '$(BUILD)' 'DEBUG'
CDEFINES += DEBUG
endif

# 
# Set output directory.
#
#
ifeq '$(strip $(DIR_OUTPUT))' ''
DIR_OUTPUT = $(CURRENTDIR)/bin
endif

include $(MAKE_LIB)

extract: ../sdk/components/sdk_validation.h

../sdk/components/sdk_validation.h:
	mkdir -p ../sdk
	(cd ../sdk; unzip -q ../../distfiles/nRF5_SDK_12.2.0_f012efa.zip)
