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

SRC_TXT +=	 app_timer.c
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

SRC_TXT += 	$(foreach D, \
      		$(NRF5_BLE), \
      		$(wildcard ../sdk/components/ble/$(D)/*.c))

DIR_USRINC +=	$(foreach D, \
	      	$(NRF5_BLE), \
      		../sdk/components/ble/$(D))

SRC_TXT +=  	$(foreach D, \
      		$(filter-out timer,$(NRF5_LIBRARIES)), \
      		$(filter-out ../sdk/components/libraries/bsp/bsp_btn_ant.c,$(wildcard ../sdk/components/libraries/$(D)/*.c)))

DIR_USRINC += 	$(foreach D, \
      		$(NRF5_LIBRARIES), \
      		../sdk/components/libraries/$(D))

DIR_USRINC +=	../sdk/components/boards

SRC_TXT +=	$(wildcard ../sdk/components/boards/*.c)

ifneq '$(strip $(NRF5_SOFTDEVICE))' ''
SRC_TXT += $(wildcard ../sdk/components/softdevice/common/softdevice_handler/*.c)
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
