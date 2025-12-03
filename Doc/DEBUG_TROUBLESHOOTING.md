# ST-Link V2 调试故障排除指南

## 问题：无法通过 ST-Link V2 加入仿真

### 诊断步骤

#### 1. 检查硬件连接

确保以下连接正确：
- **SWDIO** (数据线) - 连接到目标板的 SWDIO
- **SWCLK** (时钟线) - 连接到目标板的 SWCLK  
- **GND** (地线) - 连接到目标板的 GND
- **3.3V** (可选) - 如果目标板不自供电

#### 2. 测试 OpenOCD 连接

在终端中运行以下命令测试连接：

```bash
openocd -f Scripts/OpenOCD/openocd_stlink.cfg
```

**成功输出示例：**
```
Info : STLINK V2J43S7 (API v2) VID:PID 0483:3748
Info : Target voltage: 3.243415
Info : stm32f4x.cpu: Cortex-M4 r0p1 processor detected
Info : stm32f4x.cpu: target has 6 breakpoints, 4 watchpoints
Info : starting gdb server for stm32f4x.cpu on 3333
Info : Listening on port 3333 for gdb connections
```

如果看到以上输出，说明硬件连接正常。

#### 3. 检查可执行文件路径

确保 CMake 已正确生成可执行文件：

```bash
# 检查文件是否存在
ls Build/Debug/erdp_stm32.elf

# 如果不存在，重新编译
cmake --build Build/Debug
```

#### 4. VS Code 调试配置问题

如果 OpenOCD 能连接但 VS Code 无法调试，尝试以下方法：

**方法 A：使用慢速配置**
- 在调试配置中选择 "Debug (OpenOCD + ST-Link V2 - Slow)"

**方法 B：检查 cortex-debug 扩展**
- 确保已安装 "Cortex-Debug" 扩展
- 重启 VS Code

**方法 C：手动指定可执行文件路径**
编辑 `.vscode/launch.json`，将：
```json
"executable": "${command:cmake.launchTargetPath}",
```
改为：
```json
"executable": "${workspaceFolder}/Build/Debug/erdp_stm32.elf",
```

#### 5. 复位配置问题

如果连接超时，尝试修改 `Scripts/OpenOCD/openocd_stlink.cfg`：

**选项 1：硬件复位（当前配置）**
```cfg
reset_config srst_only srst_nogate
```

**选项 2：软件复位**
```cfg
reset_config none separate
```

**选项 3：连接复位**
```cfg
reset_config connect_assert_srst
```

#### 6. 降低适配器速度

如果连接不稳定，降低速度：

编辑 `Scripts/OpenOCD/openocd_stlink.cfg`：
```cfg
adapter speed 500  # 或 100
```

#### 7. 检查目标芯片状态

**芯片被锁定：**
- 尝试按住复位按钮，然后启动调试
- 或使用 ST-Link Utility 解锁芯片

**芯片处于低功耗模式：**
- 尝试硬件复位
- 检查目标板供电是否正常

#### 8. 使用 GDB 手动连接

如果 VS Code 无法连接，可以手动使用 GDB：

```bash
# 终端1：启动 OpenOCD
openocd -f Scripts/OpenOCD/openocd_stlink.cfg

# 终端2：启动 GDB
arm-none-eabi-gdb Build/Debug/erdp_stm32.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```

### 常见错误及解决方案

#### 错误：`timed out while waiting for target halted`

**解决方案：**
1. 降低适配器速度到 500 或 100 kHz
2. 尝试不同的复位配置
3. 检查硬件连接
4. 按住复位按钮，然后启动调试

#### 错误：`Error: open failed`

**解决方案：**
1. 检查 ST-Link 驱动是否正确安装
2. 检查 USB 连接
3. 尝试重新插拔 ST-Link
4. 检查是否有其他程序占用 ST-Link

#### 错误：`No target found` 或 `Target voltage: 0.000000`

**解决方案：**
1. 检查目标板供电
2. 检查 SWDIO/SWCLK 连接
3. 检查 GND 连接
4. 尝试不同的复位配置

#### 错误：`executable not found`

**解决方案：**
1. 确保已编译项目：`cmake --build Build/Debug`
2. 检查可执行文件路径是否正确
3. 在 `launch.json` 中使用绝对路径

### 调试技巧

1. **查看详细输出**
   - 在 VS Code 调试控制台查看 OpenOCD 输出
   - 设置 `"showDevDebugOutput": "raw"` 查看原始输出

2. **分步调试**
   - 先确保 OpenOCD 能连接
   - 再确保 GDB 能连接
   - 最后确保 VS Code 能启动调试

3. **使用慢速模式**
   - 如果标准速度不稳定，始终使用慢速模式
   - 慢速模式更可靠，但调试速度较慢

### 联系支持

如果以上方法都无法解决问题，请提供：
1. OpenOCD 的完整输出
2. VS Code 调试控制台的错误信息
3. ST-Link 版本信息（从 OpenOCD 输出中获取）
4. 目标板型号和配置

