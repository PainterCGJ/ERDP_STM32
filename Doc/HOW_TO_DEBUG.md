# 如何使用方案1：优化后的调试配置

## 完整步骤指南

### 步骤 1：准备工作

#### 1.1 检查硬件连接
确保 ST-Link V2 已正确连接：
- ✅ USB 连接到电脑
- ✅ SWDIO 连接到目标板 SWDIO 引脚
- ✅ SWCLK 连接到目标板 SWCLK 引脚  
- ✅ GND 连接到目标板 GND
- ✅ 目标板供电正常（约 3.3V）

#### 1.2 确保项目已编译
```bash
# 如果还没有编译，运行：
cmake --build Build/Debug

# 检查可执行文件是否存在：
ls Build/Debug/erdp_stm32.elf
```

#### 1.3 检查 VS Code 扩展
确保已安装以下扩展：
- **Cortex-Debug** (作者：marus25)
- **CMake Tools** (可选，但推荐)

### 步骤 2：重启 VS Code

1. 完全关闭 VS Code（不是最小化）
2. 重新打开 VS Code
3. 打开项目文件夹：`D:\Projecrt\ERDP_STM32`

### 步骤 3：启动调试

#### 方法 A：使用快捷键（推荐）

1. **按 `F5` 键**
2. 如果弹出配置选择菜单，选择：
   ```
   Debug (OpenOCD + ST-Link V2)
   ```
3. 调试会自动启动

#### 方法 B：使用调试面板

1. **点击左侧活动栏的"运行和调试"图标**（或按 `Ctrl+Shift+D`）
2. **在顶部下拉菜单选择**：
   ```
   Debug (OpenOCD + ST-Link V2)
   ```
3. **点击绿色播放按钮**（或按 `F5`）

### 步骤 4：观察调试输出

调试启动后，在 VS Code 底部会显示几个面板：

#### 4.1 调试控制台（Debug Console）
查看 OpenOCD 和 GDB 的输出，应该看到：

**正常输出示例：**
```
Info : STLINK V2J43S7 (API v2) VID:PID 0483:3748
Info : Target voltage: 3.243415
Info : stm32f4x.cpu: Cortex-M4 r0p1 processor detected
Info : stm32f4x.cpu: target has 6 breakpoints, 4 watchpoints
Info : starting gdb server for stm32f4x.cpu on 3333
Info : Listening on port 3333 for gdb connections
```

#### 4.2 终端（Terminal）
查看编译和构建输出

#### 4.3 调试工具栏
- **继续** (F5) - 继续执行
- **单步跳过** (F10) - 执行下一行
- **单步进入** (F11) - 进入函数
- **单步跳出** (Shift+F11) - 跳出函数
- **重启** (Ctrl+Shift+F5) - 重新启动调试
- **停止** (Shift+F5) - 停止调试

### 步骤 5：验证调试是否成功

#### 成功标志：
1. ✅ 调试控制台显示 "Listening on port 3333 for gdb connections"
2. ✅ 程序停在 `main` 函数（如果设置了 `runToEntryPoint: "main"`）
3. ✅ 变量窗口可以查看变量值
4. ✅ 可以设置断点并暂停执行

#### 如果失败：
参考 `Doc/DEBUG_TROUBLESHOOTING.md` 进行故障排除

### 步骤 6：使用调试功能

#### 设置断点
1. 在代码行号左侧点击，出现红点
2. 程序运行到断点时会自动暂停
3. 可以查看变量值、调用栈等

#### 查看变量
- **自动显示**：鼠标悬停在变量上
- **变量窗口**：左侧面板的"变量"部分
- **监视窗口**：添加表达式进行监视

#### 单步调试
- **F10** - 单步跳过（不进入函数）
- **F11** - 单步进入（进入函数内部）
- **Shift+F11** - 单步跳出（跳出当前函数）
- **F5** - 继续执行到下一个断点

### 常见问题

#### Q: 调试启动后立即停止？
A: 检查：
1. 可执行文件路径是否正确
2. 目标芯片是否正常供电
3. 尝试使用慢速配置

#### Q: 无法连接到目标？
A: 尝试：
1. 使用慢速配置："Debug (OpenOCD + ST-Link V2 - Slow)"
2. 检查硬件连接
3. 重启 ST-Link（重新插拔 USB）

#### Q: 程序无法运行？
A: 检查：
1. 是否已烧录程序到芯片
2. 复位配置是否正确
3. 查看 OpenOCD 输出中的错误信息

### 快速参考

| 操作 | 快捷键 | 说明 |
|------|--------|------|
| 启动调试 | F5 | 开始或继续调试 |
| 停止调试 | Shift+F5 | 停止当前调试会话 |
| 重启调试 | Ctrl+Shift+F5 | 重新启动调试 |
| 单步跳过 | F10 | 执行下一行，不进入函数 |
| 单步进入 | F11 | 进入函数内部 |
| 单步跳出 | Shift+F11 | 跳出当前函数 |
| 切换断点 | F9 | 在当前行设置/取消断点 |

### 下一步

如果方案1不工作，尝试：
- **方案2**：使用慢速配置
- **方案3**：手动指定可执行文件路径
- **方案4**：检查 cortex-debug 扩展

详细故障排除请参考：`Doc/DEBUG_TROUBLESHOOTING.md`

