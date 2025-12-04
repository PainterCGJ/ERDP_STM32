# ST-Link + GDB Server 调试配置指南

## 概述

本项目使用 **ST-Link** 硬件调试器配合 **OpenOCD**（作为 GDB Server）进行调试。这是 ARM Cortex-M 调试的标准方案。

### 工作原理

```
VS Code (Cortex-Debug) 
    ↓ (GDB 协议)
GDB 客户端 (arm-none-eabi-gdb)
    ↓ (TCP/IP localhost:3333)
OpenOCD (GDB Server)
    ↓ (ST-Link API)
ST-Link V2 硬件
    ↓ (SWD 接口)
STM32F407VE 目标芯片
```

**关键组件：**
- **ST-Link V2**：硬件调试器，通过 USB 连接到电脑
- **OpenOCD**：作为 GDB Server，在端口 3333 上监听 GDB 连接
- **GDB**：调试器客户端，通过 TCP/IP 连接到 OpenOCD
- **Cortex-Debug**：VS Code 扩展，自动管理整个调试流程

## 当前配置

### launch.json 配置

项目已配置了以下调试选项：

#### 1. Debug (OpenOCD + ST-Link V2) - 标准配置

```json
{
    "name": "Debug (OpenOCD + ST-Link V2)",
    "servertype": "openocd",  // 使用 OpenOCD 作为 GDB Server
    "configFiles": [
        "${workspaceFolder}/Scripts/OpenOCD/openocd_stlink.cfg"
    ],
    "device": "STM32F407VE",
    "runToEntryPoint": "main"
}
```

**特点：**
- SWD 速度：1000 kHz（1 MHz）
- 自动运行到 `main()` 函数
- 支持 FreeRTOS 调试
- 实时监视变量

#### 2. Debug (OpenOCD + ST-Link V2 - Slow) - 慢速配置

用于连接不稳定时，SWD 速度降低到 100 kHz。

### OpenOCD 配置文件

`Scripts/OpenOCD/openocd_stlink.cfg`：

```cfg
source [find interface/stlink.cfg]    # 加载 ST-Link 接口配置
transport select hla_swd              # 选择 SWD 传输协议
source [find target/stm32f4x.cfg]     # 加载 STM32F4 目标配置

adapter speed 1000                    # SWD 速度：1 MHz
reset_config none separate            # 复位配置：软件复位
```

## 使用方法

### 方法 1：VS Code 自动调试（推荐）

1. **连接硬件**
   - ST-Link V2 USB 连接到电脑
   - SWDIO、SWCLK、GND 连接到目标板

2. **编译项目**
   ```bash
   cmake --build Build/Debug
   ```

3. **启动调试**
   - 按 `F5` 键
   - 选择 "Debug (OpenOCD + ST-Link V2)"
   - 调试会自动启动

**调试流程：**
1. Cortex-Debug 扩展启动 OpenOCD
2. OpenOCD 连接到 ST-Link，然后连接到目标芯片
3. OpenOCD 启动 GDB Server（端口 3333）
4. Cortex-Debug 启动 GDB 客户端
5. GDB 连接到 OpenOCD 的 GDB Server
6. 程序自动运行到 `main()` 函数并暂停

### 方法 2：手动使用 GDB Server（用于故障排除）

如果 VS Code 自动调试失败，可以手动使用：

#### 步骤 1：启动 OpenOCD（GDB Server）

在终端中运行：

```bash
openocd -f Scripts/OpenOCD/openocd_stlink.cfg
```

**成功输出示例：**
```
Open On-Chip Debugger 0.12.0
Licensed under GNU GPL v2
For bug reports, read
    http://openocd.org/doc/doxygen/bugs.html
Info : STLINK V2J43S7 (API v2) VID:PID 0483:3748
Info : Target voltage: 3.243415
Info : stm32f4x.cpu: Cortex-M4 r0p1 processor detected
Info : stm32f4x.cpu: target has 6 breakpoints, 4 watchpoints
Info : starting gdb server for stm32f4x.cpu on 3333
Info : Listening on port 3333 for gdb connections
```

**关键信息：**
- `Info : Listening on port 3333 for gdb connections` - GDB Server 已启动
- 端口 3333 是默认的 GDB Server 端口

#### 步骤 2：启动 GDB 客户端

在另一个终端中运行：

```bash
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
```

#### 步骤 3：连接到 GDB Server

在 GDB 提示符下输入：

```gdb
(gdb) target remote localhost:3333
```

**成功输出：**
```
Remote debugging using localhost:3333
0x08000000 in Reset_Handler ()
```

#### 步骤 4：加载程序并开始调试

```gdb
(gdb) monitor reset halt          # 复位并暂停
(gdb) load                         # 加载程序到 Flash
(gdb) monitor reset halt          # 再次复位并暂停
(gdb) break main                   # 在 main 函数设置断点
(gdb) continue                     # 继续执行到断点
```

## 配置优化

### 调整 SWD 速度

如果连接不稳定，可以降低速度：

编辑 `Scripts/OpenOCD/openocd_stlink.cfg`：
```cfg
adapter speed 500   # 降低到 500 kHz
# 或
adapter speed 100  # 降低到 100 kHz（最稳定）
```

### 修改 GDB Server 端口

如果需要使用不同的端口，可以在 OpenOCD 配置文件中添加：

```cfg
gdb_port 3334  # 使用端口 3334 而不是默认的 3333
```

然后在 `launch.json` 中添加：
```json
"gdbTargetIpAddress": "localhost",
"gdbTargetPort": 3334
```

### 添加复位配置

如果需要硬件复位，修改 `openocd_stlink.cfg`：

```cfg
reset_config srst_only srst_nogate  # 使用硬件复位（如果 ST-Link 支持）
```

## 验证 GDB Server 连接

### 测试 OpenOCD 连接

```bash
openocd -f Scripts/OpenOCD/openocd_stlink.cfg
```

如果看到以下输出，说明连接正常：
- `Info : Target voltage: 3.xxx` - 目标电压正常
- `Info : stm32f4x.cpu: Cortex-M4 r0p1 processor detected` - 检测到目标芯片
- `Info : Listening on port 3333 for gdb connections` - GDB Server 已启动

### 测试 GDB 连接

```bash
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
(gdb) target remote localhost:3333
```

如果连接成功，GDB 会显示：
```
Remote debugging using localhost:3333
```

## 常见问题

### 问题 1：OpenOCD 无法启动 GDB Server

**症状：** OpenOCD 启动但没有显示 "Listening on port 3333"

**解决方案：**
1. 检查 ST-Link 驱动是否正确安装
2. 检查 USB 连接
3. 尝试重新插拔 ST-Link
4. 检查是否有其他程序占用 ST-Link

### 问题 2：GDB 无法连接到 GDB Server

**症状：** `target remote localhost:3333` 失败

**解决方案：**
1. 确认 OpenOCD 已启动并显示 "Listening on port 3333"
2. 检查防火墙是否阻止了端口 3333
3. 尝试使用 `127.0.0.1:3333` 而不是 `localhost:3333`
4. 检查是否有其他程序占用端口 3333

### 问题 3：连接超时

**症状：** `timed out while waiting for target halted`

**解决方案：**
1. 降低 SWD 速度（使用慢速配置）
2. 检查硬件连接（SWDIO、SWCLK、GND）
3. 尝试按住复位按钮，然后启动调试
4. 检查目标板供电是否正常

### 问题 4：GDB Server 端口被占用

**症状：** OpenOCD 提示端口已被占用

**解决方案：**
1. 关闭占用端口的其他程序
2. 修改 OpenOCD 配置使用其他端口
3. 使用 `netstat -ano | findstr 3333`（Windows）查找占用端口的进程

## 高级用法

### 使用多个 GDB 客户端

OpenOCD 支持多个 GDB 客户端同时连接：

```bash
# 终端 1：启动 OpenOCD
openocd -f Scripts/OpenOCD/openocd_stlink.cfg

# 终端 2：GDB 客户端 1
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
(gdb) target remote localhost:3333

# 终端 3：GDB 客户端 2
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
(gdb) target remote localhost:3333
```

### 使用 Telnet 接口

OpenOCD 还提供 Telnet 接口（端口 4444），可以直接发送命令：

```bash
telnet localhost 4444
```

然后可以发送 OpenOCD 命令：
```
> reset halt
> flash write_image erase Build/Debug/erdp_stm32.elf
> reset run
```

### 自定义 GDB 启动脚本

创建 `.gdbinit` 文件：

```gdb
target remote localhost:3333
monitor reset halt
load
monitor reset halt
break main
continue
```

然后启动 GDB：
```bash
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
```

GDB 会自动执行 `.gdbinit` 中的命令。

## 参考资源

- **OpenOCD 官方文档**：http://openocd.org/doc/
- **ST-Link 用户手册**：https://www.st.com/resource/en/user_manual/um2609-stlinkv2-stlinkv2b-stlinkv2-1-stlinkv3-setup-and-user-guide-stmicroelectronics.pdf
- **GDB 文档**：https://sourceware.org/gdb/documentation/
- **Cortex-Debug 扩展**：https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug


