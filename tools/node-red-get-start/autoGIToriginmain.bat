bat
@echo off
chcp 65001
git fetch origin main:fetchmain
for /f "tokens=*" %%i in ('git rev-list --count origin/main') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count main') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo ==============================
  echo 远程仓库的提交数量较多，拉取
  echo ==============================
  git pull origin main -f
  echo echo 通过拉取远程仓库保持同步
  echo ------------------------------
)else  (
  echo ==============================
  echo 以本地文件为准，推送
  echo ==============================
  git add .
  git commit -am "count:%local_count%@%COMPUTERNAME%"
  git push origin main
)
git branch -D fetchmain
pause