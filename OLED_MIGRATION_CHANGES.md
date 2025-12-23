# OLED库移植更改对比

## 对比参考项目：graduation-design-master/RCT6_FreeRTOS_arm_v1/Project

## 一、头文件更改 (oled_iic.h)

### 1. 头文件包含
- **参考项目**：`#include "sys.h"`
- **当前项目**：`#include "stm32f4xx.h"`
- **说明**：替换为标准库头文件

### 2. C++兼容性
- **参考项目**：无 C++ 保护
- **当前项目**：添加了 `#ifdef __cplusplus` 保护（第4-6行和第80-82行）
- **说明**：支持 C++ 调用

### 3. 嵌套头文件定义
- **参考项目**：在 `_OLED_IIC_H` 内嵌套定义了 `__OLED_H`
- **当前项目**：移除了嵌套的 `__OLED_H` 定义
- **说明**：简化头文件结构

## 二、源文件更改 (oled_iic.c)

### 1. 头文件包含
- **参考项目**：
  - `#include "oled_iic.h"`
  - `#include "oled.h"`
  - `#include "stdlib.h"`
  - `#include "oledfont.h"`
  - `#include "main.h"`

- **当前项目**：
  - `#include "oled_iic.h"`
  - `#include "stdlib.h"`
  - `#include "oledfont.h"`
  - `#include "erdp_if_rtos.h"` ⭐ **新增**
  - `#include <string.h>` ⭐ **新增**
  - `#include <math.h>` ⭐ **新增**
  - 移除了 `#include "oled.h"` 和 `#include "main.h"`

### 2. 延迟函数定义
- **参考项目**：使用外部定义的 `delay_ms()`（在 main.h 中）
- **当前项目**：添加了宏定义 `#define delay_ms erdp_if_rtos_delay_ms` ⭐ **新增**
- **说明**：使用 RTOS 延迟函数

### 3. GPIO 时钟配置（硬件I2C模式）
- **参考项目**（第93行）：
  ```c
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
  ```
- **当前项目**（第95行）：
  ```c
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  ```
- **说明**：STM32F4 的 GPIO 时钟在 AHB1 总线上，且不需要 AFIO

### 4. GPIO 模式配置（硬件I2C模式）
- **参考项目**（第95行）：
  ```c
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  ```
- **当前项目**（第99-101行）：
  ```c
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  ```
- **说明**：STM32F4 将模式、输出类型、上下拉分离配置

### 5. GPIO 复用功能配置
- **参考项目**：无显式配置复用功能
- **当前项目**（第96-97行）：⭐ **新增**
  ```c
  GPIO_PinAFConfig(GPIOB, GPIO_Pin_10, GPIO_AF_I2C2);
  GPIO_PinAFConfig(GPIOB, GPIO_Pin_11, GPIO_AF_I2C2);
  ```
- **说明**：STM32F4 需要显式配置 GPIO 复用功能

### 6. I2C 时钟速度
- **参考项目**（第116行）：`I2C_InitStruct.I2C_ClockSpeed = 400000;` // 400kHz
- **当前项目**（第122行）：`I2C_InitStruct.I2C_ClockSpeed = 100000;` // 100kHz ⭐ **更改**
- **说明**：降低 I2C 速度以提高稳定性

### 7. GPIO 时钟配置（软件I2C模式）
- **参考项目**（第563行）：
  ```c
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  ```
- **当前项目**（第569行）：
  ```c
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  ```
- **说明**：同样改为 AHB1 总线

### 8. GPIO 模式配置（软件I2C模式）
- **参考项目**（第565行）：
  ```c
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  ```
- **当前项目**（第571-573行）：
  ```c
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  ```
- **说明**：STM32F4 分离配置模式、输出类型和上下拉

## 三、算法层面的更改

经过详细对比，**驱动算法逻辑完全一致**，没有算法层面的更改。所有显示、绘制、刷新等核心算法都保持原样：

### 1. 软件I2C时序算法
- `IIC_Start()` - 起始信号时序：**完全一致**
- `IIC_Stop()` - 停止信号时序：**完全一致**
- `IIC_Wait_Ack()` - 等待应答时序：**完全一致**
- `Write_IIC_Byte()` - 字节发送时序：**完全一致**

### 2. 硬件I2C通信流程
- `HI2C_Start()` - 起始信号流程：**完全一致**
- `HI2C_SendAddress()` - 地址发送流程：**完全一致**
- `HI2C_WriteByte()` - 字节写入流程：**完全一致**
- `HI2C_Stop()` - 停止信号流程：**完全一致**（增加了超时保护，但逻辑一致）
- `HI2C_WriteData()` - 数据写入流程：**完全一致**

### 3. 显存管理算法
- `OLED_Refresh_Gram()` - 显存刷新算法：**完全一致**
- `OLED_ClearGram()` - 显存清空算法：**完全一致**（使用 `memset`，但效果相同）
- `OLED_DrawPoint()` - 画点算法：**完全一致**
- `OLED_Fill()` - 区域填充算法：**完全一致**

### 4. 字符显示算法
- `OLED_ShowChar()` - 字符显示算法：**完全一致**
  - 字体数据访问方式：`oled_asc2_1206[chr][t]` 和 `oled_asc2_1608[chr][t]` **完全一致**
  - 位操作逻辑：**完全一致**
  - 反白显示逻辑：**完全一致**

### 5. 数字/字符串显示算法
- `OLED_ShowNum()` - 数字显示算法：**完全一致**
  - 数字提取逻辑：`(num/oled_pow(10,len-t-1))%10` **完全一致**
  - 前导零处理：**完全一致**
- `OLED_ShowString()` - 字符串显示算法：**完全一致**
  - 换行处理逻辑：**完全一致**
- `OLED_ShowFloat()` - 浮点数显示算法：**完全一致**
  - 精度控制逻辑：**完全一致**

### 6. 图片处理算法
- `OLED_FillImage()` - 图片填充算法：**完全一致**
  - 字节对齐计算：**完全一致**
  - 位偏移处理：**完全一致**
  - 多页处理逻辑：**完全一致**
- `OLED_InvertBlock()` - 反色算法：**完全一致**
  - 边界处理逻辑：**完全一致**
  - 位操作逻辑：**完全一致**

### 7. OLED初始化序列
- `OLED_Init()` - 初始化命令序列：**完全一致**
  - 所有SSD1306配置命令：**完全一致**
  - 命令顺序：**完全一致**

### 8. 核心写入函数
- `OLED_WR_Byte()` - 字节写入函数：**完全一致**
  - 硬件I2C分支：**完全一致**
  - 软件I2C分支：**完全一致**
  - 命令/数据区分逻辑：**完全一致**

## 四、总结

除了替换 STM32F4 标准库函数外，主要更改包括：

1. **RTOS 集成**：使用 RTOS 延迟函数替代外部延迟函数
2. **GPIO 配置方式**：适配 STM32F4 的 GPIO 配置结构（分离模式、输出类型、上下拉）
3. **时钟总线**：从 APB2 改为 AHB1（GPIO 时钟）
4. **复用功能配置**：显式配置 GPIO 复用功能
5. **I2C 速度**：从 400kHz 降低到 100kHz
6. **C++ 兼容性**：添加 C++ 保护
7. **头文件清理**：移除不必要的头文件包含

### 重要结论：
**所有驱动算法逻辑完全保持原样，没有任何算法层面的更改。** 所有的修改都是：
- 硬件抽象层（HAL）的适配
- 库函数调用的替换
- 配置参数的调整

核心的显示算法、I2C通信协议、显存管理、字符渲染等算法逻辑都**100%保持一致**。

