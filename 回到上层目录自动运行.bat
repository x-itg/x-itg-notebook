@echo off
chcp 65001
setlocal enabledelayedexpansion
cd ..
for /d %%i in (*) do (
  if exist "%%i\自动推送拉取脚本.bat" (
    cd "%%i"
    自动推送拉取脚本.bat
    cd ..
  )
)