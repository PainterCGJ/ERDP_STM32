# 如何在下载（烧录）时选择烧录器

本项目支持三种烧录方式，您可以根据需要选择。

## 方法 1：使用独立目标（推荐，最简单）

这是最简单的方法，无需修改配置，直接选择对应的目标即可。

### 可用的烧录目标

| 烧录器 | 命令 | 说明 |
|--------|------|------|
| **ST-Link V2** | `flash_stlink` | 标准速度（1000 kHz） |
| **ST-Link V2 (慢速)** | `flash_stlink_slow` | 慢速模式（100 kHz），连接不稳定时使用 |
| **CMSIS-DAP** | `flash_dap` | DAPLink 或其他兼容的 DAP 调试器 |

### 使用方法

#### 在终端中运行：

```bash
# 使用 ST-Link V2（标准速度）
cmake --build Build/Debug --target flash_stlink

# 使用 ST-Link V2（慢速模式）
cmake --build Build/Debug --target flash_stlink_slow

# 使用 CMSIS-DAP
cmake --build Build/Debug --target flash_dap
```

#### 在 VS Code 中：

1. **按 `Ctrl+Shift+P`** 打开命令面板
2. 输入 `CMake: Build Target`
3. 选择对应的目标：
   - `flash_stlink` - ST-Link V2
   - `flash_stlink_slow` - ST-Link V2 (慢速)
   - `flash_dap` - CMSIS-DAP

#### 使用 CMake Tools 扩展：

1. 点击底部状态栏的 **"Build"** 按钮
2. 选择 **"Select a target to build"**
3. 选择对应的 `flash_*` 目标

## 方法 2：设置默认烧录器（一次性配置）

如果您经常使用同一个烧录器，可以设置它为默认值。

### 步骤 1：配置 CMake

在配置 CMake 时指定 `FLASH_PROGRAMMER` 变量：

```bash
# 使用 ST-Link V2（默认）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK

# 使用 ST-Link V2（慢速）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK_SLOW

# 使用 CMSIS-DAP
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=DAP
```

### 步骤 2：使用默认 flash 目标

配置后，使用默认的 `flash` 目标即可：

```bash
# 使用当前配置的默认烧录器
cmake --build Build/Debug --target flash

# 或使用 install 目标
cmake --install Build/Debug
```

### 查看当前默认烧录器

配置时会显示：
```
-- Using flash programmer: ST-Link V2
```

## 方法 3：使用 Install 目标

```bash
# 使用当前配置的默认烧录器
cmake --install Build/Debug
```

## 完整示例

### 示例 1：快速切换到 DAP 并烧录

```bash
# 1. 编译项目
cmake --build Build/Debug

# 2. 使用 DAP 烧录
cmake --build Build/Debug --target flash_dap
```

### 示例 2：设置默认使用 ST-Link V2

```bash
# 1. 配置项目（设置默认烧录器）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK

# 2. 编译项目
cmake --build Build/Debug

# 3. 烧录（自动使用 ST-Link V2）
cmake --build Build/Debug --target flash
```

### 示例 3：连接不稳定时使用慢速模式

```bash
# 直接使用慢速模式烧录
cmake --build Build/Debug --target flash_stlink_slow
```

## 如何选择烧录器？

### 选择 ST-Link V2 (`flash_stlink`)
- ✅ 您有 ST-Link V2 调试器
- ✅ 连接稳定，速度要求较高
- ✅ 标准使用场景

### 选择 ST-Link V2 慢速 (`flash_stlink_slow`)
- ✅ 连接不稳定，经常超时
- ✅ 目标板供电不稳定
- ✅ 调试时遇到连接问题
- ⚠️ 速度较慢，但更稳定

### 选择 CMSIS-DAP (`flash_dap`)
- ✅ 您有 DAPLink 或其他兼容的 DAP 调试器
- ✅ 需要更高的速度（2000 kHz）
- ✅ 支持硬件复位

## 常见问题

### Q: 如何知道当前使用的是哪个烧录器？

A: 查看烧录时的输出信息：
```
Flashing firmware to target via ST-Link V2...
```
或查看 CMake 配置时的输出：
```
-- Using flash programmer: ST-Link V2
```

### Q: 可以同时连接多个烧录器吗？

A: 可以，但每次只能使用一个。OpenOCD 会自动检测连接的调试器。

### Q: 烧录失败怎么办？

A: 尝试以下方法：
1. 使用慢速模式：`flash_stlink_slow`
2. 检查硬件连接
3. 重新插拔 USB
4. 检查目标板供电

### Q: 如何添加新的烧录器？

A: 
1. 在 `Scripts/OpenOCD/` 创建新的配置文件
2. 在 `cmake/flash_by_install.cmake` 中添加新的目标
3. 参考现有配置进行修改

## 快速参考

```bash
# 最常用的命令
cmake --build Build/Debug --target flash_stlink      # ST-Link V2
cmake --build Build/Debug --target flash_stlink_slow # ST-Link V2 (慢速)
cmake --build Build/Debug --target flash_dap         # CMSIS-DAP
```

## 相关文档

- 详细故障排除：`Doc/DEBUG_TROUBLESHOOTING.md`
- 烧录器切换指南：`Doc/FLASH_PROGRAMMER_GUIDE.md`

