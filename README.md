NRF5 SDK 17.0.2 from Nordic Semiconductor.

It can be downloaded from https://developer.nordicsemi.com/. CMake config expects
to find downloaded zip in ../distfiles.

To include the SDK in project, add following to the CMakeLists.txt of the project:

```
set(HAL_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../nrf5-sdk/nrfx)
add_peer_directory(../nrf5-sdk/nrfstack)
target_link_libraries(${PROJECT_NAME} nrfstack picoos)
```

'nrfx' module only contains the CMSIS, low-level hal and drivers. Bluetooth
stack is in 'nrfstack' module. If you are not using bluetooth, then nrfx module
is sufficient.

To control what components from SDK are available to project, following
knobs (Makefile variables) can be set in project MakeCMakeLists.txt:

```
set(NRF5_CHIP NRF52832_XXAB)
set(NRF5_SOFTDEVICE s132)
set(NRF5_THINLOG 1)
set(NRF5_BLE_SERVICES ble_bas ble_dis)
```
NRF5_THINLOG uses simple pico[OS nano layer logging instead of rather complex
one from SDK. It doesn't log from interrupt handlers - if that is needed use
standard SDK logging instead.

Knobs will automatically set correct include path for project.

