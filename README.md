# ERDP
Embedded rapid development platform

## 简介
ERDP（嵌入式快速开发平台）是一个基于C++构建的通用单片机开发框架。其**核心目标**是实现软硬件的彻底解耦，从而**提升开发效率、降低维护成本**。
 
**关键特性：**
- **统一的硬件抽象层（HAL）**：将芯片特定外设（如GPIO、UART、I2C）封装为标准化API，使应用代码与底层硬件隔离。
- **面向软件工程师的友好设计**：通过提供清晰的、面向对象的C++接口，屏蔽底层硬件的复杂性，让不具备深厚硬件背景的开发者也能快速上手并实现功能。
- **高度的可移植性与可复用性**：基于本平台编写的应用程序，可以无缝地在不同架构的单片机间迁移，极大提升了代码的复用性和项目迭代速度。

ERDP（嵌入式快速开发平台）致力于解决嵌入式开发中固有的软硬件耦合难题。我们通过构建一个标准化的C++开发框架，将复杂的硬件细节封装成易于理解的软件接口。这不仅显著提高了开发效率，更打破了技术的壁垒，让软件开发者能够专注于业务逻辑与创新，无需深入底层即可快速实现强大的单片机应用。

**ERDP：让嵌入式开发，像软件一样简单。**

**传统嵌入式开发的痛点：**
- 应用代码与芯片绑定，更换硬件成本高昂。
- 开发门槛高，软件工程师需深入钻研数据手册。
- 代码复用性差，项目维护困难。

**ERDP的解决方案：**
ERDP（嵌入式快速开发平台）是一个创新的C++框架，它通过**硬件抽象化**和**模块组件化**设计，实现了：
- **对软件开发者**：提供清晰的、高级别的C++ API，让您像操作软件对象一样控制硬件，快速构建应用，无需关心底层寄存器配置。
- **对项目与团队**：建立开发标准，提升代码复用率和可维护性，使项目能在不同硬件平台间平滑迁移，最终达成**降低开发成本、加速产品上市**的核心目标。

## 项目使用的工具

本项目基于以下工具链构建，提供了完整的开发、编译、调试和烧录解决方案：

### 构建系统
- **CMake** (3.22+) - 跨平台构建系统，用于管理项目配置和编译流程
- **Ninja** - 快速构建生成器，用于加速编译过程

### 编译器工具链
- **ARM GCC Toolchain** (arm-none-eabi-gcc/g++) - ARM Cortex-M 系列微控制器的交叉编译工具链
  - 支持 C11 标准
  - 支持 C++17 标准（禁用异常和 RTTI，适合嵌入式环境）
  - 目标芯片：STM32F407VET6 (Cortex-M4F)

### 调试与烧录工具
- **OpenOCD** - 开源的片上调试器，支持多种调试器接口
  - 支持 ST-Link V2 调试器
  - 支持 CMSIS-DAP 调试器
  - 提供 GDB 服务器接口，用于调试
- **GDB** - GNU 调试器，通过 OpenOCD 连接进行源码级调试

### RTOS
- **FreeRTOS** - 实时操作系统内核，提供任务调度、队列、信号量等机制

### 芯片支持库
- **STM32F4xx Standard Peripheral Library** - STM32F4 系列标准外设库
- **CMSIS** - ARM Cortex 微控制器软件接口标准

### 开发环境
- **VS Code** - 推荐的代码编辑器
  - **Cortex-Debug** 扩展 - 提供 ARM Cortex-M 调试支持
  - **CMake Tools** 扩展 - 提供 CMake 项目集成
  - **clangd** - 语言服务器，提供代码补全和智能提示（通过 compile_commands.json）

### 可选工具
- **Keil MDK** - 项目包含 Keil 工程文件（.uvprojx），可作为备选 IDE

## Windows 开发环境搭建指南

### 前置要求
- Windows 10/11 操作系统
- 管理员权限（部分安装步骤需要）

### 步骤 1：安装 Git（如果尚未安装）

1. 访问 [Git 官网](https://git-scm.com/download/win)
2. 下载并安装 Git for Windows
3. 安装时选择默认选项即可
4. 验证安装：
```powershell
git --version
```

### 步骤 2：安装 CMake

1. 访问 [CMake 官网](https://cmake.org/download/)
2. 下载 Windows x64 Installer（推荐版本 3.22 或更高）
3. 运行安装程序：
   - ✅ 勾选 "Add CMake to system PATH"
   - ✅ 勾选 "Add CMake to system PATH for all users"（如果使用管理员权限）
4. 验证安装：
```powershell
cmake --version
```

### 步骤 3：安装 Ninja

**方法 A：使用预编译二进制文件（推荐）**

1. 访问 [Ninja Releases](https://github.com/ninja-build/ninja/releases)
2. 下载 `ninja-win.zip`
3. 解压到任意目录（如 `C:\Tools\ninja`）
4. 将该目录添加到系统 PATH 环境变量：
   - 右键"此电脑" → "属性" → "高级系统设置" → "环境变量"
   - 在"系统变量"中找到 `Path`，点击"编辑"
   - 点击"新建"，添加 Ninja 所在目录（如 `C:\Tools\ninja`）
   - 点击"确定"保存
5. 验证安装：
```powershell
ninja --version
```

**方法 B：使用包管理器**

如果已安装 Chocolatey：
```powershell
choco install ninja
```

如果已安装 Scoop：
```powershell
scoop install ninja
```

### 步骤 4：安装 ARM GCC 工具链

1. 访问 [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
2. 选择 "AArch32 bare-metal target (arm-none-eabi)" → Windows
3. 下载最新版本的 Windows ZIP 文件（如 `arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi.zip`）
4. 解压到目录（如 `C:\Tools\arm-gnu-toolchain`）
5. 将 `bin` 目录添加到系统 PATH：
   - 完整路径示例：`C:\Tools\arm-gnu-toolchain\arm-none-eabi\bin`
   - 添加到 PATH 的方法同步骤 3
6. 验证安装：
```powershell
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
```

### 步骤 5：安装 OpenOCD

**方法 A：使用预编译二进制文件（推荐）**

1. 访问 [OpenOCD Releases](https://github.com/openocd-org/openocd/releases)
2. 下载最新版本的 Windows ZIP 文件（如 `openocd-v0.12.0-i686-w64-mingw32.zip`）
3. 解压到目录（如 `C:\Tools\openocd`）
4. 将 `bin` 目录添加到系统 PATH：
   - 完整路径示例：`C:\Tools\openocd\bin`
5. 验证安装：
```powershell
openocd --version
```

**方法 B：使用包管理器**

如果已安装 Chocolatey：
```powershell
choco install openocd
```

如果已安装 Scoop：
```powershell
scoop install openocd
```

### 步骤 6：安装 VS Code 及扩展

1. **安装 VS Code**
   - 访问 [VS Code 官网](https://code.visualstudio.com/)
   - 下载并安装 Windows 版本
   - 安装时建议勾选"添加到 PATH"

2. **安装必需扩展**
   - 打开 VS Code
   - 按 `Ctrl+Shift+X` 打开扩展市场
   - 搜索并安装以下扩展：
     - **Cortex-Debug** (作者：marus25) - ARM Cortex-M 调试支持
     - **CMake Tools** (作者：Microsoft) - CMake 项目集成
     - **C/C++** (作者：Microsoft) - C/C++ 语言支持（可选，但推荐）

3. **配置 clangd（可选，用于更好的代码补全）**
   - 在扩展市场搜索并安装 **clangd** (作者：llvm-vs-code-extensions)
   - 如果同时安装了 C/C++ 扩展，建议禁用其 IntelliSense 以避免冲突：
     - 打开设置（`Ctrl+,`）
     - 搜索 "C_Cpp.intelliSenseEngine"
     - 设置为 "Disabled"

### 步骤 7：验证开发环境

打开 PowerShell 或命令提示符，依次运行以下命令验证所有工具是否正确安装：

```powershell
# 验证 Git
git --version

# 验证 CMake
cmake --version

# 验证 Ninja
ninja --version

# 验证 ARM GCC
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version

# 验证 OpenOCD
openocd --version
```

如果所有命令都能正确显示版本信息，说明环境配置成功！

### 步骤 8：克隆并构建项目

1. **克隆项目**（如果从 Git 仓库获取）：
```powershell
git clone <repository-url>
cd ERDP_STM32
```

2. **配置项目**：
```powershell
# 使用 CMake Presets（推荐）
cmake --preset Debug

# 或者手动配置
cmake -S . -B Build/Debug -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake
```

3. **编译项目**：
```powershell
cmake --build Build/Debug
```

4. **烧录程序**（需要连接调试器）：
```powershell
# 使用默认烧录器（DAP）
cmake --build Build/Debug --target flash

# 或使用 ST-Link V2
cmake --build Build/Debug --target flash_stlink
```

### 步骤 9：配置调试器

1. **硬件连接**
   - 将 ST-Link V2 或 CMSIS-DAP 调试器通过 USB 连接到电脑
   - 将调试器的 SWDIO、SWCLK、GND 连接到目标板对应引脚
   - 确保目标板正常供电

2. **测试调试器连接**
```powershell
# 测试 ST-Link V2 连接
openocd -f Scripts/OpenOCD/openocd_stlink.cfg

# 测试 CMSIS-DAP 连接
openocd -f Scripts/OpenOCD/openocd_dap.cfg
```

如果看到 "Listening on port 3333 for gdb connections" 说明连接成功，按 `Ctrl+C` 退出。

3. **在 VS Code 中调试**
   - 打开项目文件夹
   - 按 `F5` 启动调试
   - 选择对应的调试配置（如 "Debug (OpenOCD + ST-Link V2)"）
   - 详细调试指南请参考 `Doc/HOW_TO_DEBUG.md`

### 常见问题排查

#### PATH 环境变量未生效
- 重启 PowerShell/命令提示符窗口
- 重启 VS Code
- 如果仍无效，重启电脑

#### 找不到工具命令
- 检查 PATH 环境变量是否正确添加
- 检查工具是否安装在指定目录
- 使用完整路径测试工具是否可执行

#### OpenOCD 连接失败
- 检查调试器驱动是否正确安装
- 尝试使用慢速配置（`openocd_stlink_slow.cfg` 或 `openocd_dap_slow.cfg`）
- 参考 `Doc/DEBUG_TROUBLESHOOTING.md` 进行故障排除

#### 编译错误
- 确认 ARM GCC 工具链版本兼容
- 检查 CMake 版本是否满足要求（3.22+）
- 清理构建目录后重新配置：`rm -r Build/Debug && cmake --preset Debug`

### 参考文档

- [调试指南](Doc/HOW_TO_DEBUG.md) - 详细的调试配置和使用说明
- [调试故障排除](Doc/DEBUG_TROUBLESHOOTING.md) - 常见调试问题解决方案
- [烧录器选择指南](Doc/HOW_TO_SELECT_FLASHER.md) - 如何选择合适的调试器
- [烧录器使用指南](Doc/FLASH_PROGRAMMER_GUIDE.md) - 烧录器详细配置说明

