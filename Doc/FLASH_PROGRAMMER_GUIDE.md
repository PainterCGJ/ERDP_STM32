# 烧录器切换指南

本项目支持多种调试/烧录器，您可以根据需要轻松切换。

## 支持的烧录器

1. **ST-Link V2** (默认) - 标准速度 (1000 kHz)
2. **ST-Link V2 (Slow)** - 慢速模式 (100 kHz)，用于连接不稳定时
3. **CMSIS-DAP** - DAPLink 或其他兼容的 DAP 调试器

## 切换方法

### 方法 1：使用 VS Code 调试配置（推荐用于调试）

在 VS Code 中按 `F5` 启动调试时，选择对应的配置：

- **Debug (OpenOCD + ST-Link V2)** - 使用 ST-Link V2 标准速度
- **Debug (OpenOCD + ST-Link V2 - Slow)** - 使用 ST-Link V2 慢速模式
- **Debug (OpenOCD + DAP)** - 使用 CMSIS-DAP 调试器

### 方法 2：使用 CMake 目标（推荐用于烧录）

#### 快速切换（使用独立目标）

```bash
# 使用 ST-Link V2（标准速度）
cmake --build Build/Debug --target flash_stlink

# 使用 ST-Link V2（慢速模式）
cmake --build Build/Debug --target flash_stlink_slow

# 使用 CMSIS-DAP
cmake --build Build/Debug --target flash_dap
```

#### 设置默认烧录器

在配置 CMake 时指定 `FLASH_PROGRAMMER` 变量：

```bash
# 使用 ST-Link V2（默认）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug

# 或显式指定
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK

# 使用 ST-Link V2（慢速）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK_SLOW

# 使用 CMSIS-DAP
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=DAP
```

设置后，使用默认的 `flash` 目标即可：

```bash
cmake --build Build/Debug --target flash
```

### 方法 3：使用 CMake Install 目标

```bash
# 使用当前配置的默认烧录器
cmake --install Build/Debug
```

## 配置文件位置

所有 OpenOCD 配置文件位于 `Scripts/OpenOCD/` 目录：

- `openocd_stlink.cfg` - ST-Link V2 标准配置
- `openocd_stlink_slow.cfg` - ST-Link V2 慢速配置
- `openocd_dap.cfg` - CMSIS-DAP 配置

## 常见问题

### Q: 如何知道当前使用的是哪个烧录器？

A: 在 CMake 配置时会显示：
```
-- Using flash programmer: ST-Link V2
```

### Q: 连接超时怎么办？

A: 尝试以下步骤：
1. 首先尝试慢速模式：`flash_stlink_slow`
2. 检查硬件连接（SWDIO, SWCLK, GND）
3. 检查目标板供电是否正常
4. 尝试降低速度（编辑对应的 .cfg 文件）

### Q: 如何添加新的烧录器？

A: 
1. 在 `Scripts/OpenOCD/` 目录创建新的配置文件（如 `openocd_jlink.cfg`）
2. 在 `cmake/flash_by_install.cmake` 中添加新的条件分支和目标
3. 在 `.vscode/launch.json` 中添加新的调试配置

## 示例：完整工作流程

```bash
# 1. 配置项目（使用 ST-Link V2）
cmake -B Build/Debug -DCMAKE_BUILD_TYPE=Debug -DFLASH_PROGRAMMER=STLINK

# 2. 编译项目
cmake --build Build/Debug

# 3. 烧录程序
cmake --build Build/Debug --target flash_stlink

# 或者切换到 DAP 并烧录
cmake --build Build/Debug --target flash_dap
```

## 调试配置切换

在 VS Code 中：
1. 按 `F5` 或点击调试按钮
2. 在调试配置下拉菜单中选择对应的烧录器配置
3. 开始调试

## 注意事项

- 确保对应的调试器已正确连接
- 不同烧录器的连接速度可能不同
- 如果遇到连接问题，优先尝试慢速模式

