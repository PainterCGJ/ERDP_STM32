cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Setup C++ settings - C++17 with no exceptions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(erdp_stm32 C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add startup file
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Kernel/Driver/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f40_41xxx.s
)

# Add system files
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Kernel/Driver/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
)

# Add interrupt handlers
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Kernel/Driver/stm32f4xx_it.c
)

# Add system call stubs
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Kernel/syscalls.c
)

# Add STM32 Standard Peripheral Library sources
# Exclude FMC (Flexible Memory Controller) as STM32F407VET6 only supports FSMC
file(GLOB_RECURSE STM32_STDPERIPH_SOURCES
    "Source/Kernel/Driver/STM32F4xx_StdPeriph_Driver/src/*.c"
)
# Remove FMC files (not supported on STM32F407VET6)
list(FILTER STM32_STDPERIPH_SOURCES EXCLUDE REGEX ".*stm32f4xx_fmc\\.c$")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${STM32_STDPERIPH_SOURCES})

# Add FreeRTOS sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Kernel/RTOS/FreeRTOS/src/croutine.c
    Source/Kernel/RTOS/FreeRTOS/src/event_groups.c
    Source/Kernel/RTOS/FreeRTOS/src/list.c
    Source/Kernel/RTOS/FreeRTOS/src/queue.c
    Source/Kernel/RTOS/FreeRTOS/src/stream_buffer.c
    Source/Kernel/RTOS/FreeRTOS/src/tasks.c
    Source/Kernel/RTOS/FreeRTOS/src/timers.c
    Source/Kernel/RTOS/FreeRTOS/port/GCC/ARM_CM4F/port.c
    Source/Kernel/RTOS/FreeRTOS/port/MemMang/heap_4.c
)

# Add Interface sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Interface/Hardware/src/erdp_if_exti.c
    Source/Interface/Hardware/src/erdp_if_gpio.c
    Source/Interface/Hardware/src/erdp_if_spi.c
    Source/Interface/Hardware/src/erdp_if_uart.c
    Source/Interface/RTOS/erdp_if_rtos.c
)

# Add HAL sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/HAL/GPIO/erdp_hal_gpio.cpp
    Source/HAL/UART/erdp_hal_uart.cpp
    Source/HAL/SPI/erdp_hal_spi.cpp
    Source/HAL/EXTI/erdp_hal_exti.cpp
)

# Add OSAL sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/OSAL/erdp_osal.cpp
)

# Add Adapter sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Adapter/log/log_adapter.cpp
)

# Add Library sources
# Note: printf library removed, using standard library printf instead

# Add Application sources
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Source/Application/main.cpp
)

# Add include paths
target_include_directories(
  ${CMAKE_PROJECT_NAME} PRIVATE 
  Source
  Source/Kernel/Driver
  Source/Kernel/Driver/CMSIS/Include
  Source/Kernel/Driver/CMSIS/Device/ST/STM32F4xx/Include
  Source/Kernel/Driver/STM32F4xx_StdPeriph_Driver/inc
  Source/Kernel/RTOS/FreeRTOS
  Source/Kernel/RTOS/FreeRTOS/inc
  Source/Kernel/RTOS/FreeRTOS/port/GCC/ARM_CM4F
  Source/Kernel/RTOS/FreeRTOS/port/MemMang
  Source/Interface
  Source/Interface/Hardware/inc
  Source/Interface/RTOS
  Source/HAL
  Source/HAL/GPIO
  Source/HAL/UART
  Source/HAL/SPI
  Source/HAL/EXTI
  Source/OSAL
  Source/Adapter/log
  Source/Library
  Source/Library/log
  Source/Common
  Source/Board
  Source/Application
)

# Add project symbols (macros)
target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PRIVATE
  STM32F40_41xxx
  USE_STDPERIPH_DRIVER
  ARM_MATH_CM4
  __FPU_PRESENT=1
  __FPU_USED=1
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    m
)

# 设置cmake install指令，烧录程序
include(${PROJECT_SOURCE_DIR}/cmake/flash_by_install.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/create_bin.cmake)


