if(CMAKE_C_COMPILER_ID MATCHES "ARMClang")
  add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_FROMELF} --i32 $<TARGET_FILE:${CMAKE_PROJECT_NAME}> --output
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_FROMELF} --bin $<TARGET_FILE:${CMAKE_PROJECT_NAME}> --output
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_FROMELF} --text -z $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)

elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
  add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    # 生成 Intel HEX 文件（调试 + 烧录都能用）
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.hex
    # 生成 BIN 文件（用于YMODEM传输到bootloader，包含必要的Flash段）
    COMMAND
      ${CMAKE_OBJCOPY} -O binary -j .isr_vector -j .text -j .rodata -j .ARM.extab
      -j .ARM.exidx -j .preinit_array -j .init_array -j .fini_array -j .data
      $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
      $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.bin

    COMMENT "Generating HEX and BIN files")

endif()



