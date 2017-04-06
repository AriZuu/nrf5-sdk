
DIR_MODINC +=	$(foreach D, \
      		$(NRF5_BLE), \
      		$(MOD)/../sdk/components/ble/$(D))

DIR_MODINC +=	$(foreach D, \
      		$(NRF5_LIBRARIES), \
      		$(MOD)/../sdk/components/libraries/$(D))

DIR_MODINC +=	$(MOD)/../sdk/components/boards

ifeq '$(BUILD)' 'DEBUG'
CDEFINES += DEBUG
endif

EXTRA_CFLAGS += -Wno-cast-align

