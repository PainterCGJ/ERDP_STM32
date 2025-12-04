# 在 C++ 代码中使用 printf.h 宏的方法

## 问题

在 C++ 代码中，如果标准库头文件（如 `<cstdio>`, `<functional>`, `<vector>` 等）在 `printf.h` 之前被包含，标准库会声明 `printf` 函数，导致与 `printf.h` 的宏定义冲突。

## 解决方案

### 方法 1：确保 `printf.h` 在标准库头文件之前包含（推荐）

**关键原则**：`printf.h` 必须在所有标准库头文件之前包含。

```cpp
// ✅ 正确：printf.h 在标准库之前
#include "printf.h"
#include <functional>
#include <vector>

// ❌ 错误：标准库在 printf.h 之前
#include <functional>
#include <vector>
#include "printf.h"
```

**示例**：

```cpp
// main.cpp
#include "printf.h"        // 1. 首先包含 printf.h
#include "erdp_assert.h"   // 2. 然后包含其他项目头文件
#include "erdp_hal_uart.hpp"

#include <functional>      // 3. 最后包含标准库头文件
#include <vector>

int main() {
    printf("Hello World\n");  // 会调用 printf_() 函数
    return 0;
}
```

### 方法 2：在头文件中使用前向声明

如果头文件需要包含标准库，但也要使用 `printf`，可以这样组织：

```cpp
// my_header.hpp
#ifndef MY_HEADER_HPP
#define MY_HEADER_HPP

// 如果需要使用 printf，先包含 printf.h
#include "printf.h"

// 然后包含标准库
#include <functional>
#include <vector>

// 使用 printf
void my_function() {
    printf("Debug: %d\n", 42);
}

#endif
```

### 方法 3：在源文件中包含

如果头文件不需要直接使用 `printf`，可以在源文件中包含：

```cpp
// my_header.hpp
#ifndef MY_HEADER_HPP
#define MY_HEADER_HPP

#include <functional>
#include <vector>

// 不在这里包含 printf.h

#endif

// my_source.cpp
#include "printf.h"      // 在源文件中包含
#include "my_header.hpp"

void my_function() {
    printf("Debug: %d\n", 42);  // 可以使用 printf
}
```

## printf.h 的修改

`printf.h` 已经添加了 `#undef` 来取消标准库可能已经定义的宏：

```c
#ifdef __cplusplus
// 取消标准库可能已经定义的宏（如果存在）
#undef printf
#undef sprintf
#undef snprintf
#undef vsnprintf
#undef vprintf
#endif

// 然后定义我们的宏
#define printf printf_
int printf_(const char* format, ...);
```

## 宏展开机制

### 在 C 代码中：

```c
#include "printf.h"

void test() {
    printf("Hello\n");  // 宏展开：printf_("Hello\n")
}
```

### 在 C++ 代码中（正确包含顺序）：

```cpp
#include "printf.h"      // 宏定义生效
#include <functional>   // 标准库头文件

void test() {
    printf("Hello\n");  // 宏展开：printf_("Hello\n")
}
```

### 在 C++ 代码中（错误包含顺序）：

```cpp
#include <functional>   // 标准库先包含，声明了 printf
#include "printf.h"     // 宏定义与标准库声明冲突

void test() {
    printf("Hello\n");  // ❌ 编译错误：冲突
}
```

## 最佳实践

1. **统一包含顺序**：
   ```cpp
   // 1. 系统/标准库头文件（如果需要）
   // 2. 项目头文件（包含 printf.h）
   #include "printf.h"
   #include "project_header.h"
   // 3. 标准库头文件（如果需要）
   #include <functional>
   ```

2. **在公共头文件中优先包含**：
   如果多个文件都需要使用 `printf`，可以在公共头文件中包含：
   ```cpp
   // erdp_config.h 或类似的公共头文件
   #include "printf.h"
   ```

3. **使用 `printf_` 直接调用**（如果宏有问题）：
   ```cpp
   printf_("Hello\n");  // 直接调用，不依赖宏
   ```

## 验证方法

编译时检查是否使用了自定义的 `printf_`：

```bash
# 查看符号表
arm-none-eabi-nm your_program.elf | grep printf
# 应该看到 printf_ 而不是 printf
```

## 常见问题

### Q: 为什么在 C++ 中宏不生效？

A: 可能是因为标准库头文件在 `printf.h` 之前被包含。确保 `printf.h` 在所有标准库头文件之前。

### Q: 如何知道使用的是哪个 printf？

A: 编译后查看符号表，如果看到 `printf_` 说明使用的是自定义实现，如果看到 `printf` 说明使用的是标准库。

### Q: 可以在命名空间中使用吗？

A: `printf_` 函数是 C 函数（`extern "C"`），可以在 C++ 命名空间中使用，但宏定义是全局的。

