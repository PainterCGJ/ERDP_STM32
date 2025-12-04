# printf 宏展开机制说明

## 问题分析

### 1. `erdp_assert.h` 中的 `printf` 调用

`erdp_assert.h` 中包含了 `printf.h`，并在宏定义中使用了 `printf`：

```c
#include "printf.h"
#define erdp_assert(condition) \
    do { \
        if (!(condition)) { \
            printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__); \
            while (1) {} \
        } \
    } while (0)
```

### 2. `printf.h` 中的宏定义

`printf.h` 中定义了宏，将 `printf` 重定义为 `printf_`：

```c
// 只在 C 代码中重定义，避免与 C++ 标准库冲突
#ifndef __cplusplus
#define printf printf_
#endif
int printf_(const char* format, ...);
```

### 3. 调用机制

#### 情况 1：在 C 文件中使用 `erdp_assert`

**示例**：`Source/Interface/Hardware/src/erdp_if_uart.c`

```c
#include "erdp_assert.h"  // 包含了 printf.h

void erdp_if_uart_init(...) {
    erdp_assert(uart > ERDP_UART0 && uart < ERDP_UART_NUM);
    // ...
}
```

**宏展开过程**：

1. 预处理器处理 `erdp_assert.h`：
   - 包含 `printf.h`
   - `printf.h` 定义 `#define printf printf_`（因为这是 C 文件）

2. 预处理器展开 `erdp_assert` 宏：
   ```c
   do {
       if (!(uart > ERDP_UART0 && uart < ERDP_UART_NUM)) {
           printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__);
           // ↑ 这里的 printf 会被替换为 printf_
           while (1) {}
       }
   } while (0)
   ```

3. 最终展开结果：
   ```c
   do {
       if (!(uart > ERDP_UART0 && uart < ERDP_UART_NUM)) {
           printf_("Assertion failed: %s, line %d\n", __FILE__, __LINE__);
           // ↑ 实际调用的是 printf_ 函数
           while (1) {}
       }
   } while (0)
   ```

**结果**：在 C 文件中，`erdp_assert` 中的 `printf` 会被替换为 `printf_`，实际调用的是 `printf_` 函数（来自 `printf.c`）。

#### 情况 2：在 C++ 文件中使用 `erdp_assert`

**示例**：`Source/OSAL/erdp_osal.hpp`

```cpp
#include "erdp_assert.h"  // 包含了 printf.h

namespace erdp {
    void Thread::start() {
        erdp_assert(__handler != nullptr);
        // ...
    }
}
```

**宏展开过程**：

1. 预处理器处理 `erdp_assert.h`：
   - 包含 `printf.h`
   - `printf.h` 中的 `#define printf printf_` **不会生效**（因为 `#ifndef __cplusplus` 条件不满足）

2. 预处理器展开 `erdp_assert` 宏：
   ```cpp
   do {
       if (!(__handler != nullptr)) {
           printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__);
           // ↑ 这里的 printf 不会被替换，保持原样
           while (1) {}
       }
   } while (0)
   ```

3. 最终展开结果：
   ```cpp
   do {
       if (!(__handler != nullptr)) {
           printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__);
           // ↑ 尝试调用标准库的 printf
           while (1) {}
       }
   } while (0)
   ```

**问题**：

- 如果 C++ 文件包含了 `<cstdio>` 或 `<stdio.h>`（通过其他头文件间接包含），会尝试调用标准库的 `printf`
- 但是标准库的 `printf` 可能：
  - 使用半主机（semihosting），导致程序卡在 `BKPT 0xAB`
  - 或者没有实现，导致链接错误

## 解决方案

### 方案 1：在 C++ 中也使用 `printf_`（推荐）

修改 `erdp_assert.h`，直接使用 `printf_`：

```c
#define erdp_assert(condition) \
    do { \
        if (!(condition)) { \
            printf_("Assertion failed: %s, line %d\n", __FILE__, __LINE__); \
            while (1) {} \
        } \
    } while (0)
```

**优点**：
- C 和 C++ 中都使用自定义的 `printf_` 函数
- 避免与标准库冲突
- 统一行为

**缺点**：
- 需要修改代码

### 方案 2：在 C++ 中禁用 `printf` 宏，但提供替代方案

保持 `printf.h` 的当前实现（只在 C 中定义宏），但在 C++ 中提供命名空间版本的函数。

### 方案 3：使用条件编译

在 `erdp_assert.h` 中使用条件编译：

```c
#define erdp_assert(condition) \
    do { \
        if (!(condition)) { \
#ifdef __cplusplus
            printf_("Assertion failed: %s, line %d\n", __FILE__, __LINE__); \
#else
            printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__); \
#endif
            while (1) {} \
        } \
    } while (0)
```

## 当前状态

根据当前的代码：

- **C 文件**：`erdp_assert` 中的 `printf` → `printf_`（自定义实现）
- **C++ 文件**：`erdp_assert` 中的 `printf` → 标准库的 `printf`（如果可用）

这可能导致：
1. C++ 文件中如果包含了标准库头文件，会使用标准库的 `printf`（可能有问题）
2. C++ 文件中如果没有包含标准库头文件，会报链接错误

## 推荐做法

**统一使用 `printf_`**，确保在所有文件中都使用自定义的 `printf` 实现，避免与标准库冲突。

