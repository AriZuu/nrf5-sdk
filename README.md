NRF5 SDK from Nordic Semiconductor.

It can be downloaded from https://developer.nordicsemi.com/. Makefile expects
to find downloaded zip in ../distfiles.

To include the SDK in project, add following to the Makefile of the project:

```
CMSIS_MODULES		= $(CURRENTDIR)/../nrf5-sdk/hal
MODULES			= ../nrf5-sdk/stack
```

To control what components from SDK are available to project, following
knobs (Makefile variables) can be set in project Makefile:


```
NRF5_SOFTDEVICE	= s130
NRF5_DEFINES  	= NRF51 NRF51822 BOARD_PCA10001 BLE_STACK_SUPPORT_REQD NRF_SD_BLE_API_VERSION=2
NRF5_DRIVERS	= common clock gpiote
NRF5_BLE	= common ble_advertising ble_services/ble_bas ble_services/ble_hrs
NRF5_LIBRARIES	= timer fstorage experimental_section_vars sensorsim button fds scheduler bsp
```

Knobs will automatically set correct include path for project.
Don't include util or log libraries in NRF5_LIBRARIES, they are included by default.

Makefile in "hal" subdirectory builds only low-level stuff, including CMSIS support
and selected device drivers. "stack" subdirectory build rest of the sdk sources.
For projects that don't use bluetooth, hal is sufficient.
