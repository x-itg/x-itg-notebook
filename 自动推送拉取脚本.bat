@echo off
chcp 65001
echo 自动推送拉取脚本.bat
echo 自动识别最新的仓库 
echo 任意键【继续】，关闭命令行【取消】操作
pause
setlocal enabledelayedexpansion
git fetch -q origin main:fetchmain
for /f "tokens=*" %%i in ('git rev-list --count origin/main') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count main') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  git log -1 --pretty=format:"%%s"
  echo 远程仓库的提交数量较多，拉取
  git pull origin main -f
  echo echo 通过拉取远程仓库保持同步
  git log -1 --pretty=format:"%%s"
  echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
)else  (
  echo TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  git log -1 --pretty=format:"%%s"
  set /a Change=0
  git status | findstr /C:"Untracked files:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在未跟踪的文件 准备重新添加推送
  )  
  git status | findstr /C:"modified:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在修改的文件 准备推送 !Change! 
  ) 
  git status | findstr /C:"deleted:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在删除的文件 准备推送 !Change! 
  )
  if "!Change!"=="0" (
    echo 本地仓库无变化，不推送 !Change!
    git log -1 --pretty=format:"%%s"
    echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
  )
  if "!Change!"=="1" (
    echo 本地仓库有变化，推送 !Change!
    git add .
    git commit -am "count:%local_count%@%COMPUTERNAME%"
    git push -q origin main
    git log -1 --pretty=format:"%%s"
    echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

  )
  
)
git branch -D -q fetchmain
pause