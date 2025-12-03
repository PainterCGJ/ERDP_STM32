@echo off
REM OpenOCD 连接测试脚本
REM 用于诊断 ST-Link V2 连接问题

echo ========================================
echo OpenOCD ST-Link V2 连接测试
echo ========================================
echo.

echo [1/3] 检查 OpenOCD 是否可用...
where openocd >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 openocd.exe，请确保 OpenOCD 已安装并在 PATH 中
    pause
    exit /b 1
)
echo [OK] OpenOCD 已找到
echo.

echo [2/3] 检查配置文件是否存在...
if not exist "Scripts\OpenOCD\openocd_stlink_slow.cfg" (
    echo [错误] 配置文件不存在: Scripts\OpenOCD\openocd_stlink_slow.cfg
    pause
    exit /b 1
)
echo [OK] 配置文件存在
echo.

echo [3/3] 尝试连接 ST-Link V2...
echo.
echo 提示：如果连接成功，您应该看到类似以下信息：
echo   - Info : STLINK V2J... (API v2)
echo   - Info : Target voltage: 3.xxx
echo   - Info : stm32f4x.cpu: Cortex-M4 ...
echo   - Info : Listening on port 3333 for gdb connections
echo.
echo 按 Ctrl+C 可以停止测试
echo.
echo ========================================
echo.

cd /d "%~dp0\.."
openocd -f Scripts/OpenOCD/openocd_stlink_slow.cfg

pause


