# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/amin/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# enable usb output, disable uart output
#pico_enable_stdio_uart(AutoWaterOOP 0)
#pico_enable_stdio_usb(AutoWaterOOP 1)

FILE(GLOB FreeRTOS_src FreeRTOS-Kernel/*.c)
FILE(GLOB_RECURSE API_src include/*.cpp)
#FILE(GLOB Ezo_i2c_src Ezo_i2c/*.cpp)

add_library(
  FreeRTOS STATIC 
  ${FreeRTOS_src}
  FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c
  FreeRTOS-Kernel/portable/MemMang/heap_4.c
)  

add_library(
  API STATIC
  ${API_src}
)

#add_library(EzoI2C STATIC
#  ${Ezo_i2c_src}
#)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(AutoWaterOOP C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(AutoWaterOOP AutoWaterOOP.cpp ${API_src})

pico_set_program_name(AutoWaterOOP "AutoWaterOOP")
pico_set_program_version(AutoWaterOOP "0.1")

pico_enable_stdio_uart(AutoWaterOOP 0)
pico_enable_stdio_usb(AutoWaterOOP 1)

add_subdirectory(modules/pico-onewire)
add_subdirectory(modules/pi-pico-LCD/lcd_display)

# Add the standard library to the build
target_link_libraries(AutoWaterOOP
        pico_stdlib
        hardware_gpio
        FreeRTOS
        hardware_i2c
        pico_time
        API
        pico_one_wire
        lcd_display
        )

  target_link_libraries(API
        pico_stdlib
        hardware_gpio
        FreeRTOS
        hardware_i2c
        pico_time
        pico_one_wire
        lcd_display
        )

# Add the standard include files to the build
target_include_directories(AutoWaterOOP PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  )

target_include_directories( FreeRTOS PUBLIC
  FreeRTOS-Kernel/include
  include/
  FreeRTOS-Kernel/portable/GCC/ARM_CM0
  modules/pico-onewire/api
)

#target_include_directories( EzoI2C PUBLIC
#  Ezo_i2c
#)
# Add any user requested libraries
target_link_libraries(AutoWaterOOP 
        )

pico_add_extra_outputs(AutoWaterOOP)

