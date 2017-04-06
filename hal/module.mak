NRF5_DEFINES ?= NRF51 NRF51822

#
# Common stuff that is always needed.
#
DIR_MODINC +=	$(MOD)/../sdk/components/device \
		$(MOD)/../sdk/components/toolchain \
		$(MOD)/../sdk/components/toolchain/cmsis/include \
		$(MOD)/../sdk/components/drivers_nrf/hal

#
# Driver headers.
#
DIR_MODINC +=	$(foreach D, \
      		$(NRF5_DRIVERS), \
      		$(MOD)/../sdk/components/drivers_nrf/$(D))

#
# Minimal libraries: util & logging.
#
DIR_MODINC += 	$(MOD)/../sdk/components/libraries/util \
		$(MOD)/../sdk/components/libraries/log \
		$(MOD)/../sdk/components/libraries/log/src

#
# Softdevice headers.
#
ifeq '$(strip $(NRF5_SOFTDEVICE))' ''
DIR_MODINC +=	$(MOD)/../sdk/components/drivers_nrf/nrf_soc_nosd
else
DIR_MODINC +=	$(MOD)/../sdk/components/softdevice/$(NRF5_SOFTDEVICE)/headers \
		$(MOD)/../sdk/components/softdevice/common/softdevice_handler

NRF5_SOFTDEVICE_UPPER := $(shell echo $(NRF5_SOFTDEVICE) | tr '[:lower:]' '[:upper:]')
CDEFINES += SOFTDEVICE_PRESENT $(NRF5_SOFTDEVICE_UPPER)
endif

CDEFINES += $(NRF5_DEFINES)
ifeq '$(BUILD)' 'DEBUG'
CDEFINES += DEBUG
endif

EXTRA_CFLAGS += -Wno-cast-align

export NRF5_DEFINES
export NRF5_SOFTDEVICE
export NRF5_DRIVERS
export NRF5_BLE
export NRF5_LIBRARIES
