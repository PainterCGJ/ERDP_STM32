# Keil 调试需要多次运行才能到达 main 的解决方案

## 问题描述

在 Keil 仿真调试时，需要连续点击三次全速运行才能运行到 main 函数。

## 问题原因

### 1. Keil 调试器未启用 "Run to main" 选项

默认情况下，Keil 调试器会在 `Reset_Handler` 处停止，需要手动运行才能到达 main。

### 2. HSE 时钟等待超时

在 `SystemInit()` -> `SetSysClock()` 函数中，有等待 HSE 时钟稳定的循环：

```c
do
{
  HSEStatus = RCC->CR & RCC_CR_HSERDY;
  StartUpCounter++;
} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
```

`HSE_STARTUP_TIMEOUT` 定义为 `0x5000`（20480），在仿真环境中如果 HSE 时钟没有正确模拟，这个循环会等待很长时间。

### 3. PLL 等待循环

在 `SetSysClock()` 中还有等待 PLL 稳定的循环：

```c
while((RCC->CR & RCC_CR_PLLRDY) == 0)
{
}
```

这些等待循环在仿真环境中可能导致程序执行缓慢。

## 解决方案

### 方案 1：检查并配置调试器初始化脚本（推荐）

**注意**：检查发现项目中 `tGomain=1`（Run to main 已启用），但可能仍需要初始化脚本来处理 HSE 等待问题。

1. 打开 Keil 项目
2. 点击菜单 `Project` -> `Options for Target 'GD32'`（或按 `Alt+F7`）
3. 切换到 `Debug` 标签页
4. 点击 `Settings` 按钮（在 `Use` 下拉框旁边）
5. 切换到 `Initialization` 标签页
6. 在初始化文件中添加以下内容（如果没有初始化文件，创建一个新文件，例如 `Scripts/debug_init.ini`）：

```
// 调试器初始化脚本
// 用于解决 HSE 等待超时导致的调试问题

FUNC void Setup(void) {
    // 在仿真环境中，可以跳过 HSE 等待
    // 注意：仅用于调试，实际硬件上不应使用
    
    // 设置 HSE 就绪标志（仅用于仿真）
    // 实际硬件上应该等待 HSE 稳定
    // RCC->CR |= RCC_CR_HSERDY;  // 取消注释以在仿真中跳过 HSE 等待
}

// 在复位后执行 Setup 函数
Setup();
```

7. 在 `Debug` 标签页的 `Initialization File` 中指定该文件路径

**或者使用更简单的方法**：

在初始化命令中直接添加：

```
LOAD %L INCREMENTAL
```

这样可以确保程序加载后直接运行到 main。

### 方案 2：修改调试器初始化脚本

在 Keil 项目选项的 `Debug` 标签页中：

1. 点击 `Settings` 按钮（在 `Use` 下拉框旁边）
2. 切换到 `Initialization` 标签页
3. 在初始化命令中添加：

```
LOAD %L INCREMENTAL
Setup();
```

其中 `Setup()` 函数定义如下（添加到初始化文件或直接在命令中）：

```
FUNC void Setup(void) {
    // 直接跳转到 main 函数
    __set_MSP(*(unsigned long*)0x08000000);  // 设置堆栈指针
    __set_PC(0x08000000 + 4);                // 设置程序计数器到 Reset_Handler
    // 或者直接运行到 main
    __breakpoint(0);
}
```

### 方案 3：减少 HSE 等待超时时间（仅用于仿真）

如果只是用于仿真调试，可以临时减少 `HSE_STARTUP_TIMEOUT` 的值：

在 Keil 项目选项中：
1. `Project` -> `Options for Target` -> `C/C++` 标签页
2. 在 `Define` 中添加：`HSE_STARTUP_TIMEOUT=0x0100`（256，用于快速仿真）

或者在代码中直接修改 `stm32f4xx.h`（不推荐，会影响所有配置）：

```c
#define HSE_STARTUP_TIMEOUT    ((uint16_t)0x0100)   // 从 0x5000 改为 0x0100
```

**注意**：此方法仅适用于仿真环境，实际硬件上可能导致 HSE 启动失败。

### 方案 4：使用 HSI 作为系统时钟源（仅用于仿真）

如果只是用于仿真调试，可以修改 `SetSysClock()` 函数，使用 HSI（内部时钟）而不是 HSE（外部时钟），这样可以避免等待外部晶振。

**注意**：此方法会改变系统时钟配置，仅用于调试目的。

## 推荐配置步骤

### 步骤 1：检查当前配置

项目配置文件中已显示 `tGomain=1`（Run to main 已启用），但问题可能在于：

1. HSE 等待超时导致程序执行缓慢
2. 调试器初始化脚本缺失

### 步骤 2：创建调试器初始化脚本（推荐）

1. 创建文件 `Scripts/debug_init.ini`，内容如下：

```
// Keil 调试器初始化脚本
// 用于解决需要多次运行才能到达 main 的问题

// 注意：此脚本仅用于调试，实际硬件烧录时不应使用

// 加载程序
LOAD %L INCREMENTAL

// 如果需要，可以在这里设置断点
// 例如：在 main 函数处设置断点
// BS main
```

2. 在 Keil 中配置：
   - `Project` -> `Options for Target` -> `Debug` 标签页
   - 点击 `Settings` -> `Initialization` 标签页
   - 在 `Initialization File` 中指定 `Scripts/debug_init.ini`

### 步骤 3：配置调试器超时设置

1. 在 `Debug` 标签页点击 `Settings`
2. 切换到 `Debug` 标签页（在 Settings 对话框中）
3. 增加超时时间（例如：从 10 秒增加到 30 秒）
4. 确保 `Reset and Run` 选项已启用

### 步骤 4：验证配置

1. 重新编译项目
2. 启动调试（`F5`）
3. 应该直接运行到 main 函数，无需多次点击运行

如果仍然需要多次运行，尝试以下方法：

- **方法 A**：在初始化脚本中添加跳过 HSE 等待的代码（仅用于仿真）
- **方法 B**：减少 `HSE_STARTUP_TIMEOUT` 的值（见方案 3）
- **方法 C**：使用 HSI 而不是 HSE（见方案 4）

## 其他可能的问题

如果上述方法仍然无法解决问题，检查以下项目：

1. **启动文件配置**
   - 确认使用的是 `arm` 目录下的启动文件（用于 armclang）
   - 路径：`Source/Kernel/Driver/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm/startup_stm32f40_41xxx.s`

2. **链接器脚本**
   - 确认链接器脚本正确配置了入口点

3. **调试器连接**
   - 确认调试器（ST-Link/J-Link）连接正常
   - 检查调试器驱动是否最新

4. **仿真器设置**
   - 如果使用软件仿真，确认仿真器配置正确
   - 某些仿真器可能不完全模拟硬件时钟行为

## 参考

- Keil MDK 用户手册：Debug 章节
- STM32F4xx 参考手册：RCC（复位和时钟控制）章节
- CMSIS 系统初始化文档

