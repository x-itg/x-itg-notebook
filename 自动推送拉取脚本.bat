@echo off
chcp 65001
setlocal enabledelayedexpansion
git fetch -q origin main:fetchmain
for /f "tokens=*" %%i in ('git rev-list --count origin/main') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count main') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  git log -1 --pretty=format:"%%s"
  echo Զ�ֿ̲���ύ�����϶࣬��ȡ
  git pull origin main -f
  echo echo ͨ����ȡԶ�ֿ̲Ᵽ��ͬ��
  git log -1 --pretty=format:"%%s"
  echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
)else  (
  echo TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  git log -1 --pretty=format:"%%s"
  set /a Change=0
  git status | findstr /C:"Untracked files:"> nul
  if not errorlevel 1 (
    set Change=1
    echo ����δ���ٵ��ļ� ׼�������������
  )  
  git status | findstr /C:"modified:"> nul
  if not errorlevel 1 (
    set Change=1
    echo �����޸ĵ��ļ� ׼������ !Change! 
  ) 
  git status | findstr /C:"deleted:"> nul
  if not errorlevel 1 (
    set Change=1
    echo ����ɾ�����ļ� ׼������ !Change! 
  )
  if "!Change!"=="0" (
    echo ���زֿ��ޱ仯�������� !Change!
    git log -1 --pretty=format:"%%s"
    echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
  )
  if "!Change!"=="1" (
    echo ���زֿ��б仯������ !Change!
    git add .
    git commit -am "count:%local_count%@%COMPUTERNAME%"
    git push -q origin main
    git log -1 --pretty=format:"%%s"
    echo LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

  )
  
)
git branch -D -q fetchmain