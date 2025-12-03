# 定义自定义构建目标：下载到目标单片机
set(OPENOCD_EXECUTABLE "openocd")

# 定义默认烧录器（可以通过 CMake 变量 FLASH_PROGRAMMER 来切换）
# 可选值: STLINK, STLINK_SLOW, DAP, DAP_SLOW
# 默认使用慢速模式以提高稳定性
if(NOT DEFINED FLASH_PROGRAMMER)
    set(FLASH_PROGRAMMER "DAP" CACHE STRING "Flash programmer type: STLINK, STLINK_SLOW, DAP, or DAP_SLOW")
endif()

# 根据选择的烧录器设置配置文件路径
if(FLASH_PROGRAMMER STREQUAL "STLINK")
    set(FLASH_CFG_FILE "${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_stlink.cfg")
    set(FLASH_PROGRAMMER_NAME "ST-Link V2")
elseif(FLASH_PROGRAMMER STREQUAL "STLINK_SLOW")
    set(FLASH_CFG_FILE "${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_stlink_slow.cfg")
    set(FLASH_PROGRAMMER_NAME "ST-Link V2 (Slow)")
elseif(FLASH_PROGRAMMER STREQUAL "DAP")
    set(FLASH_CFG_FILE "${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_dap.cfg")
    set(FLASH_PROGRAMMER_NAME "CMSIS-DAP")
elseif(FLASH_PROGRAMMER STREQUAL "DAP_SLOW")
    set(FLASH_CFG_FILE "${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_dap_slow.cfg")
    set(FLASH_PROGRAMMER_NAME "CMSIS-DAP (Slow)")
else()
    message(FATAL_ERROR "Unknown flash programmer: ${FLASH_PROGRAMMER}. Use STLINK, STLINK_SLOW, DAP, or DAP_SLOW")
endif()

message(STATUS "Using flash programmer: ${FLASH_PROGRAMMER_NAME}")

# 创建默认 flash 目标（使用当前选择的烧录器）
add_custom_target(flash
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware to target via ${FLASH_PROGRAMMER_NAME}..."
    COMMAND ${OPENOCD_EXECUTABLE} 
            -f ${FLASH_CFG_FILE}
            -c "init"
            -c "reset halt"
            -c "program $<TARGET_FILE:${CMAKE_PROJECT_NAME}> verify reset"
            -c "shutdown"
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Flashing ${CMAKE_PROJECT_NAME}.elf to target via ${FLASH_PROGRAMMER_NAME}"
    USES_TERMINAL
)

# 创建独立的烧录器目标（方便快速切换）
add_custom_target(flash_stlink
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware to target via ST-Link V2..."
    COMMAND ${OPENOCD_EXECUTABLE} 
            -f ${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_stlink.cfg
            -c "init"
            -c "reset halt"
            -c "program $<TARGET_FILE:${CMAKE_PROJECT_NAME}> verify reset"
            -c "shutdown"
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Flashing ${CMAKE_PROJECT_NAME}.elf to target via ST-Link V2"
    USES_TERMINAL
)

add_custom_target(flash_stlink_slow
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware to target via ST-Link V2 (Slow)..."
    COMMAND ${OPENOCD_EXECUTABLE} 
            -f ${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_stlink_slow.cfg
            -c "init"
            -c "reset halt"
            -c "program $<TARGET_FILE:${CMAKE_PROJECT_NAME}> verify reset"
            -c "shutdown"
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Flashing ${CMAKE_PROJECT_NAME}.elf to target via ST-Link V2 (Slow)"
    USES_TERMINAL
)

add_custom_target(flash_dap
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware to target via CMSIS-DAP..."
    COMMAND ${OPENOCD_EXECUTABLE} 
            -f ${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_dap.cfg
            -c "init"
            -c "reset halt"
            -c "program $<TARGET_FILE:${CMAKE_PROJECT_NAME}> verify reset"
            -c "shutdown"
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Flashing ${CMAKE_PROJECT_NAME}.elf to target via CMSIS-DAP"
    USES_TERMINAL
)

add_custom_target(flash_dap_slow
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware to target via CMSIS-DAP (Slow)..."
    COMMAND ${OPENOCD_EXECUTABLE} 
            -f ${PROJECT_SOURCE_DIR}/Scripts/OpenOCD/openocd_dap_slow.cfg
            -c "init"
            -c "reset halt"
            -c "program $<TARGET_FILE:${CMAKE_PROJECT_NAME}> verify reset"
            -c "shutdown"
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Flashing ${CMAKE_PROJECT_NAME}.elf to target via CMSIS-DAP (Slow)"
    USES_TERMINAL
)

# Install 目标（保持兼容性，使用绝对路径）
# 使用当前选择的烧录器（通过 FLASH_PROGRAMMER 变量控制）
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(ELF_PATH "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf")
  install(
    CODE 
    "MESSAGE(\"Flash Debug firmware to target via ${FLASH_PROGRAMMER_NAME}...\")\n
    execute_process(\n
      COMMAND ${OPENOCD_EXECUTABLE} -f ${FLASH_CFG_FILE}\n
              -c init\n
              -c \"reset halt\"\n
              -c \"program ${ELF_PATH} verify reset\"\n
              -c shutdown\n
      RESULT_VARIABLE OPENOCD_RESULT\n
      ERROR_VARIABLE OPENOCD_ERROR\n
      OUTPUT_VARIABLE OPENOCD_OUTPUT\n
    )\n
    if(OPENOCD_RESULT)\n
      message(FATAL_ERROR \"OpenOCD flash failed: \${OPENOCD_ERROR}\")\n
    else()\n
      message(STATUS \"Flash completed successfully\")\n
    endif()"
  )
else()
  set(ELF_PATH "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf")
  install(
    CODE 
    "MESSAGE(\"Flash Release firmware to target via ${FLASH_PROGRAMMER_NAME}...\")\n
    execute_process(\n
      COMMAND ${OPENOCD_EXECUTABLE} -f ${FLASH_CFG_FILE}\n
              -c init\n
              -c \"reset halt\"\n
              -c \"program ${ELF_PATH} verify reset\"\n
              -c shutdown\n
      RESULT_VARIABLE OPENOCD_RESULT\n
      ERROR_VARIABLE OPENOCD_ERROR\n
      OUTPUT_VARIABLE OPENOCD_OUTPUT\n
    )\n
    if(OPENOCD_RESULT)\n
      message(FATAL_ERROR \"OpenOCD flash failed: \${OPENOCD_ERROR}\")\n
    else()\n
      message(STATUS \"Flash completed successfully\")\n
    endif()"
  )
endif()


