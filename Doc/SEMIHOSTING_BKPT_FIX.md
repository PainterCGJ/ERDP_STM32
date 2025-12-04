# BKPT 0xAB 半主机调用问题解决方案

## 问题现象

在 Keil 调试时，程序停在 `BKPT 0xAB` 指令处，无法继续运行。

```
0x0800112A BEAB      BKPT          0xAB
```

## 问题原因

`BKPT 0xAB` 是 **ARM 半主机（Semihosting）调用**。ARM C 库在执行 `__main` 函数时，可能调用了某些需要半主机功能的函数，例如：

1. **标准 I/O 操作**：`printf`, `fopen`, `fclose` 等
2. **文件系统操作**：文件读写操作
3. **调试输出**：`_ttywrch` 函数（字符输出到调试器）
4. **程序退出**：`exit()` 函数

在嵌入式系统中，通常**不使用半主机功能**，因为：
- 半主机需要调试器支持
- 会降低程序执行速度
- 在没有调试器的情况下会导致程序卡住

## 解决方案

### 方案 1：禁用半主机（已实现）

在 `syscalls.c` 文件中添加：

```c
// 禁用半主机（Semihosting）功能
#pragma import(__use_no_semihosting)
```

这会告诉 ARM C 库**不要使用半主机功能**。

### 方案 2：实现半主机存根函数

即使禁用了半主机，ARM C 库仍可能调用某些半主机函数。需要实现存根函数：

```c
/**
 * @brief  Semihosting stub function
 * @note   This function is called when semihosting is disabled
 */
void _ttywrch(int ch)
{
    (void)ch;
    // Do nothing - semihosting is disabled
}

/**
 * @brief  Semihosting stub function for file operations
 */
void _sys_exit(int status)
{
    (void)status;
    while (1)
    {
        // Infinite loop - embedded systems don't exit
    }
}
```

### 方案 3：在 Keil 链接器选项中禁用半主机（ARM Compiler 6）

对于 ARM Compiler 6 (armclang)，需要在链接器选项中添加 `--no_semihosting`：

1. 打开 Keil 项目
2. `Project` -> `Options for Target` -> `Linker` 标签页
3. 在 `Misc controls` 中添加：`--no_semihosting`
4. 重新编译项目

### 方案 4：在 Keil 项目选项中禁用半主机（ARM Compiler 5）

对于 ARM Compiler 5 (armcc)：

1. 打开 Keil 项目
2. `Project` -> `Options for Target` -> `Target` 标签页
3. 在 `Code Generation` 区域，确保：
   - **Use MicroLIB**：如果使用 MicroLIB，半主机通常已禁用
   - **或者**：在 `C/C++` 标签页的 `Define` 中添加：`__MICROLIB`

### 方案 5：使用 MicroLIB（可选）

MicroLIB 是 ARM 提供的精简 C 库，**默认禁用半主机**：

1. `Project` -> `Options for Target` -> `Target` 标签页
2. 勾选 **Use MicroLIB**
3. 重新编译项目

**注意**：MicroLIB 功能较少，如果使用标准 C++ 库（如 `<vector>`, `<functional>`），可能需要标准 C 库。**不推荐**使用 MicroLIB，因为项目使用了 C++ 标准库。

## 已实现的修复

在 `Source/Kernel/Driver/syscalls.c` 中：

1. ✅ 添加了 `#pragma import(__use_no_semihosting)` 禁用半主机
2. ✅ 实现了 `_ttywrch()` 存根函数
3. ✅ 实现了 `_sys_exit()` 存根函数

## 验证修复

修复后，程序应该能够：
1. ✅ 正常启动，不再停在 `BKPT 0xAB`
2. ✅ 直接运行到 `main()` 函数
3. ✅ 不再需要多次点击运行

## 如果问题仍然存在

如果添加了上述代码后仍然停在 `BKPT 0xAB`，检查：

1. **是否使用了标准 C++ 库**：
   - `<vector>`, `<string>`, `<iostream>` 等可能使用半主机
   - 考虑使用嵌入式友好的替代方案

2. **是否在全局构造函数中使用了 I/O**：
   - C++ 全局对象的构造函数可能在 `__main` 执行时调用
   - 避免在全局构造函数中使用 `printf` 等函数

3. **检查链接器设置**：
   - 确保链接器没有链接半主机相关的库
   - 检查 `Linker` 标签页的 `Misc Controls` 中是否有 `--semihosting` 选项

4. **使用调试器初始化脚本**：
   - 在 `Scripts/debug_init.ini` 中添加跳过半主机调用的代码

## 参考

- ARM 半主机文档：ARM Compiler User Guide
- Keil MDK 用户手册：Semihosting 章节
- `Doc/KEIL_DEBUG_RUN_TO_MAIN_FIX.md`：Keil 调试问题解决方案

