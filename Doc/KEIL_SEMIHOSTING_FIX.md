# Keil 半主机问题快速修复指南

## 问题现象

程序停在 `BKPT 0xAB` 指令处：
```
0x0800112A BEAB      BKPT          0xAB
```

## 快速解决方案（推荐）

**不需要修改代码**，直接在 Keil 链接器选项中禁用半主机：

### 步骤：

1. 打开 Keil 项目
2. `Project` -> `Options for Target 'GD32'`（或按 `Alt+F7`）
3. 切换到 **`Linker`** 标签页
4. 在 **`Misc controls`** 文本框中添加：
   ```
   --no_semihosting
   ```
5. 点击 `OK` 保存
6. **重新编译项目**（`F7`）

### 验证

配置后，程序应该能够：
- ✅ 正常启动，不再停在 `BKPT 0xAB`
- ✅ 直接运行到 `main()` 函数

---

## 备选方案：添加 syscalls.c 文件

如果上述方法不起作用，可以添加 `syscalls.c` 文件到项目中：

### 方法 1：通过 Keil IDE 添加

1. 在 Keil 项目树中，找到 **`Driver`** 组
2. 右键点击 **`Driver`** 组
3. 选择 **`Add Existing Files to Group 'Driver'...`**
4. 浏览到 `Source\Kernel\Driver\syscalls.c`
5. 选择文件并点击 `Add`
6. 重新编译项目

### 方法 2：检查文件是否已添加但被排除

1. 在项目树中找到 `syscalls.c` 文件
2. 右键点击文件
3. 选择 **`Options for File 'syscalls.c'...`**
4. 确保 **`Include in Target Build`** 已勾选
5. 点击 `OK`
6. 重新编译项目

---

## 为什么需要禁用半主机？

半主机（Semihosting）是 ARM 调试器的一个功能，允许程序通过调试器与主机通信。但在嵌入式系统中：

- ❌ 半主机需要调试器支持，在没有调试器时会卡住
- ❌ 会降低程序执行速度
- ❌ 在发布版本中不应该使用

**禁用半主机**是嵌入式系统的标准做法。

---

## 如果问题仍然存在

如果添加了 `--no_semihosting` 后仍然有问题，检查：

1. **是否重新编译了项目**：修改链接器选项后必须重新编译
2. **链接器选项格式**：确保 `--no_semihosting` 前后没有多余的空格
3. **查看编译输出**：检查是否有链接错误

---

## 参考

- `Doc/SEMIHOSTING_BKPT_FIX.md`：详细的半主机问题分析
- `Doc/KEIL_DEBUG_RUN_TO_MAIN_FIX.md`：Keil 调试问题解决方案

