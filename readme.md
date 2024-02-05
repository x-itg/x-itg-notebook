## 自动推送拉取脚本.bat

这个脚本是简易仓库维护脚本

- 双击运行此脚本

- 当远程提交数大于本地提交数时系统调用git拉取远程提交合并到本地

- 当本地提交数大于远程提交数时系统调用git推送本地提交到远程

## 一、ubuntu下编译调试stm32f1

```
---------------------下载安装stm32cubeclt------------------------

ubuntu下安装stm32cubeclt之前有安装过的可以卸载掉：https://www.st.com/en/development-tools/stm32cubeclt.html
下载好后 sudo bash stm32cubeclt.sh 安装

---------------------launch.json文件修改------------------------

.vscode文件夹下launch.json:
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "0.2.0",
    "configurations": [
        {
            "name": "wsl-ubuntu1804-stm32f1-openocd", //调试入口显示的名字，随便起
            "cwd": "${workspaceRoot}", //工作目录，根路径
            "armToolchainPath": "/opt/st/stm32cubeclt/GNU-tools-for-STM32/bin/arm-none-eabi-gcc",
            "gdbPath": "/opt/st/stm32cubeclt/GNU-tools-for-STM32/bin/arm-none-eabi-gdb",
            "executable": "${workspaceFolder}/build/EC20103RB.elf", //调试文件
            "request": "launch",
            "type": "cortex-debug", //配置为使用插件调试
            "servertype": "openocd", //映射openocd
            "configFiles": [
                "${workspaceRoot}/openocd_lnx.cfg"
            ], //openocd配置
            "postDebugTask": "Reset", //同上，调试结束执行的任务

        },
        {
            // For the Cortex-Debug extension
            "name": "win-ST-Link",
            "type": "cortex-debug",
            "servertype": "openocd",
            "request": "launch",
            "executable": "${workspaceRoot}/build/EC20103RB.elf",
            "configFiles": [
                "C:/Program Files/OpenOCD/0.10.0-13/scripts/interface/stlink-v2.cfg",
                "C:/Program Files/OpenOCD/0.10.0-13/scripts/target/stm32f1x_stlink.cfg",
            ],
            "cwd": "${workspaceRoot}"
        }
    ]
}

---------------------下载安装openocd--------------

根据这篇文章 安装openocd：https://blog.csdn.net/daoshengtianxia/article/details/115038674
git clone https://gitee.com/x-itg/openocd.git
下载openocd及其子模块后编译安装
sudo ./bootstrap
./configure --enable-stlink
sudo make
sudo make install
安装在了/usr/local/bin
配置文件在 /usr/local/share/openocd/scripts

--------------------补充 可能需要另外安装的软件-------------------------
sudo apt-get install libncurses5
sudo apt-get install lsb-core
sudo apt-get install build-essential pkg-config autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev
sudo apt-get install libtool libsysfs-dev  


--------------------补充 可能需要拷贝的文件------------------------------
sudo cp stm32f1discovery.cfg /usr/local/share/openocd/scripts/board 

--------------------makefile文件修改-------------
upload: build/$(PROJECT).bin
    openocd -f board/stm32f1discovery.cfg -c "reset_config trst_only combined" -c "program build/$(PROJECT).elf verify reset exit"
ifeq ($(OS),Windows_NT)
debug-start:
    openocd -f openocd_win.cfg
reset:
    openocd -f openocd_win.cfg -c init -c halt -reset -c shutdown
else
debug-start:
    openocd -f openocd_lnx.cfg
reset:
    openocd -f openocd_lnx.cfg -c init -c halt -reset -c shu


在工作目录下添加openocd.cfg文件，内容：

source [find /usr/local/share/openocd/scripts/interface/stlink.cfg]
source [find /usr/local/share/openocd/scripts/target/stm32f1x.cfg]### 
```

## 二、windows下工程搭建

 [参考]([【精选】Makerbase VESC 第三课 VSCODE环境搭建-CSDN博客](https://blog.csdn.net/gjy_skyblue/article/details/119669725?spm=1001.2014.3001.5502))

- 安装的软件：
- - vscode cortex-debug插件；
- - tup-latest
- - openocd 
- - arm-gdb/gcc 
- - GNU MCU Eclipse安装到C:\Program Files文件夹，它们的bin文件夹路径加入到path环境变量当中。

## 三、编译与版本控制

`make g`

```makefile
# make g 先判断_backup.bin是否存在，不存在就先make再cp一个_backup.bin
# 文件，判断.c .h 文件是否发生改变，如果改变就make，make前cp一个
# _backup.bin文件再在make后比对bin文件和_backup.bin文件；bin文件发
# 生变化就打tag 并往远程推送，未改变就不打tag也不推送；
# 判断仓库是否dirty如果dirty则进行推送（此时不打tag）
```

`autoGITorigino2main.bat`

```bat
# 事先建立本地分支main和远程主机origin
# 获取远程仓库并比较远程仓库和本地仓库的提交数
# 本地仓库提交数多或一样多的话提送本地到远程仓库
# 远程仓库提交数多的话用远程仓库的内容替换本地仓库的内容
```

## 四、SSH自动登录

```
--------------ubuntu开启ssh----------------------------
1. 查看是否安装SSHServer：ps -e|grep ssh
2. 安装SSHServer：sudo apt-get install openssh-server
3. 修改sshd_config: sudo nano /etc/ssh/sshd_config
- port 22
- PermitRootLogin prohibit-password
- PermitRootLogin yes
4. 启动SSH：/etc/init.d/ssh start
5. 设置开机自启SSH：sudo systemctl enable ssh

生成密钥：
ssh-keygen -t rsa 
------------------------------
powershell下运行： 
function ssh-copy-id([string]$userAtMachine, $args){   
    $publicKey = "$ENV:USERPROFILE" + "/.ssh/id_rsa.pub"
    if (!(Test-Path "$publicKey")){
        Write-Error "ERROR: failed to open ID file '$publicKey': No such file"            
    }
    else {
        & cat "$publicKey" | ssh $args $userAtMachine "umask 077; test -d .ssh || mkdir .ssh ; cat >> .ssh/authorized_keys || exit 1"      
    }
}
-------------------------------
powershell下运行(上传公钥):
ssh-copy-id ubuntu@IP地址
-------------------------------
修改C:\Users\mason\.ssh\config
Host 001
 HostName 远程IP地址
 User ubuntu
 IdentityFile C:/Users/mason/.ssh/id_rsa
-------------------------------
登录
ssh 001
```

## 五、远程ubuntu用本地windows的usb

本地windows电脑使用usbipd-win做usbip的服务器
远程ubuntu使用usbip做usbip的客户端  
利用ssh将远程ubuntu的3240端口转发到本地主机的3240端口
参考：
[云服务器 Linux 系统使用 USB/IP 远程共享 USB 设备-最佳实践-文档中心-腾讯云](https://cloud.tencent.com/document/product/213/43016)
[安装usbipd-win](https://github.com/dorssel/usbipd-win)

#### 本地USB服务器含有物理USB,：

```
usbipd list （本地windows安装usbipd-win后powershell里运行查看usb）
usbipd bind --force -b 2-1（把本地usb分享出去）
ssh -Nf -R 3240:localhost:3240 ubuntu@XXX.XXX.XXX.XXX （创建SSH隧道）
```

#### 远程USB客户端物理USB：

```
sudo modprobe usbip-core
sudo modprobe usbip-host
sudo modprobe usbip-vudc  # 服务端非必须
sudo modprobe vhci-hcd #必须attach失败就再试这条
sudo modprobe usbip-host
sudo usbip list --remote 127.0.0.1(远程查看端口有没有打通)
sudo usbip attach -r 127.0.0.1 -b 2-1 （载入与detach命令对应相反）usbip attach --remote=127.0.0.1 --busid=2-2
sudo lsusb（查看已经对接上的USB）# sudo mknod /dev/ttyUSB0 c 1A86 7523
dmesg | grep tty
sudo minicom -s #设置下波特率 就可以收发com数据了 
ps aux | grep minicom
sudo kill PID
```

## 六、WSL UBUNTU使用WINDOWS的USB口

参考：
[连接 USB 设备 | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/wsl/connect-usb) 
[安装usbipd-win：](https://github.com/dorssel/usbipd-win)

```
wsl ubuntu:
sudo apt install linux-tools-generic hwdata(ubunutu下安装)
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
---------------------------------
windows powershell:
usbipd wsl list
usbipd wsl attach --busid 2-1(powershell下连接usb连到wsl：)
usbipd wsl detach --busid 2-1(powershell下连接usb断开wsl：)
---------------------------------
wsl ubuntu:
lsusb
----------------------------------
将wsl端口共享到局域网，端口转发：
wsl ubuntu 
ip addr show eth0
windows powershell：
netsh interface portproxy add v4tov4 listenport=3390 listenaddress=0.0.0.0 connectport=3390 connectaddress=192.168.92.156 protocol=tcp
netsh interface portproxy add v4tov4 listenport=2222 listenaddress=0.0.0.0 connectport=2222 connectaddress=192.168.92.156
netsh interface portproxy add v4tov4 listenport=22 listenaddress=0.0.0.0 connectport=22 connectaddress=192.168.92.156
netsh interface portproxy show all
netsh interface portproxy reset #删除所有端口转发
```

## 七、WSL 图形界面

- https://learn.microsoft.com/zh-cn/windows/wsl/install-manual#step-4--download-the-linux-kernel-update-package
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
- 内核更新下载安装：https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi
- 管理员运行powershell：wsl --set-default-version 2
- 下载ubuntu1804： https://wslstorestorage.blob.core.windows.net/wslblob/Ubuntu_1804.2019.522.0_x64.appx
- 管理员运行powershell：Add-AppxPackage .\Ubuntu_1804.2019.522.0_x64.appx
- 双击安装Ubuntu_1804.2019.522.0_x64.appx

#### wsl2 应用程序直接打开gui远程桌面连接

```
- windows下载VxSrv安装时注意勾选Disable access control:    https://nchc.dl.sourceforge.net/project/vcxsrv/vcxsrv/1.20.14.0/vcxsrv-64.1.20.14.0.installer.exe
- sudo apt install -y x11-apps
- echo 'export DISPLAY=172.23.80.1:0' >> ~/.bashrc
- source ~/.bashrc
- [(14条消息) 超详细Windows10/Windows11 子系统（WSL2）安装Ubuntu20.04（带桌面环境）_萌褚的博客-CSDN博客_wsl ubuntu 桌面](https://blog.csdn.net/m0_60028455/article/details/125316625)
- [(14条消息) wsl安装xrdp（可视化界面并远程），解决闪退、黑屏_xrdp闪退_daboluo520的博客-CSDN博客](https://blog.csdn.net/guorong520/article/details/124749625)
- [(14条消息) WSL（Ubuntu20.04）与其图形界面安装配置_sandonz的博客-CSDN博客_wsl ubuntu图形界面](https://blog.csdn.net/sandonz/article/details/120854876?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EOPENSEARCH%7ERate-1-120854876-blog-113616883.pc_relevant_vip_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EOPENSEARCH%7ERate-1-120854876-blog-113616883.pc_relevant_vip_default&utm_relevant_index=2)
- git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
- cd ubuntu-wsl2-systemd-script/
- bash ubuntu-wsl2-systemd-script.sh --force
- wsl --shutdown  #去windows cmd下重启wsl
- wsl #启动ubuntu
- systemctl
- sudo apt update
- sudo apt install -y xubuntu-desktop
- sudo apt install -y xrdp
- sudo adduser xrdp ssl-cert
- sudo ufw allow 3390
- sudo sed -i 's/port=3389/port=3390/g' /etc/xrdp/xrdp.ini
- sudo echo xfce4-session > ~/.xsession
- sudo nano /etc/xrdp/sesman.ini   #将 `KillDisconnected`的值修改为 `true`,保存退出
- sudo systemctl restart xrdp
-----------------------------------------------------------------------------------
- ubuntu安装SYSTEMCTL：git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
- ubuntu安装SYSTEMCTL：cd ubuntu-wsl2-systemd-script/
- ubuntu安装SYSTEMCTL：bash ubuntu-wsl2-systemd-script.sh --force
- ubuntu安装SYSTEMCTL：wsl --shutdown  #去windows cmd下重启wsl
- ubuntu安装SYSTEMCTL：wsl #启动ubuntu
- ubuntu安装SYSTEMCTL：sudo apt update
- sudo apt install -y ubuntu-desktop
- sudo apt install -y xubuntu-desktop
- sudo apt install -y xrdp
- sudo adduser xrdp ssl-cert
- sudo ufw allow 3390
- sudo sed -i 's/port=3389/port=3390/g' /etc/xrdp/xrdp.ini
- sudo echo xfce4-session > ~/.xsession
- sudo nano /etc/xrdp/sesman.ini   #将 `KillDisconnected`的值修改为 `true`,保存退出
- sudo systemctl restart xrdp
```

## 八、ROS、Python

把pip的安装源设置为国内的清华源

pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple/

#### 8.1 WSL2 下 ROS 添加ROS官方源添加密钥

[(14条消息) win10 wsl2 + ubuntu20.04 配置 ROS-Noetic_lainegates的博客-CSDN博客_wsl2 rosnoetic](https://blog.csdn.net/LaineGates/article/details/120910628)

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

#### 8.2 安装配套工具初始化rosdep开机自动进行ROS环境配置

```
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
sudo apt-get install python3-pip
sudo pip install rosdepc
sudo apt-get install python3-roslaunch
sudo apt-get install ros-noetic-roslaunch
sudo rosdepc init
rosdepc update
sudo apt install python3-rosnode #安装节点查看工具rosnode list #查看节点
sudo apt-get install ros-noetic-rqt #rqt_graph
sudo apt-get install ros-noetic-rqt-common-plugins #rqt_graph
sudo apt-get install ros-noetic-rqt ros-noetic-rqt-common-plugins ros-noetic-turtlesim
source /opt/ros/noetic/setup.bash
```

- 安装ros-tutorials程序包
- 软件包查找如
- 软件包定位如

```
sudo apt-get install ros-noetic-ros-tutorials
rospack find roscpp
roscd roscpp
roscd roscpp/cmake
```

- 创建软件包 快速开始

```
my_package/
  CMakeLists.txt#配置Catkin元包的CMakeLists.txt模板文件
  package.xml#提供软件包元信息

mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
git clone https://gitee.com/ros1-learning/beginner_tutorials.git
cd ~/catkin_ws/
catkin_make
roscore
rosrun beginner_tutorials listener
rosrun beginner_tutorials talker
```

#### 8.3 创建Catkin工作空间

```
cd ~/catkin_ws/
catkin_make #构建一个catkin工作区并生效配置文件
source devel/setup.bash
cd ~/catkin_ws/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp #在catkin_ws/src创建软件包
rospack depends1 beginner_tutorials #查看包依赖
roscore #开启核心节点
rosrun turtlesim turtlesim_node #启动小乌龟节点
rosrun turtlesim turtle_teleop_key #开启小乌龟遥控节点
rosrun rqt_graph rqt_graph #打开画图图形
rosmsg show geometry_msgs/Twist #查看消息类型的详细信息

rostopic echo /turtle1/cmd_vel ##### 显示话题内容
rostopic type /turtle1/cmd_vel ##### 查看话题消息类型
rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]'##### 发布消息
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist -r 1 -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, -1.8]'##### 1Hz频率发送话题消息

rosrun rqt_plot rqt_plot # rqt_plot命令可以在滚动时间图上显示发布到某个话题上的数据。这里我们将使用rqt_plot命令来绘制正被发布到/
rosrun rqt_console rqt_console #连接到了ROS的日志框架
rosrun rqt_logger_level rqt_logger_level #节点运行时改变输出信息的详细级别

rosservice list #服务
rosservice call /clear #调用/call服务

rosparam list #参数服务器
rosparam set /turtlesim/background_r 150
rosparam get /turtlesim/background_r 
rosservice call /clear
rosparam get / #获取参数服务器上所有内容
--------------------------------------------------------------
mkdir msg
echo "int64 num" > msg/Num.msg
msg package.xml 加入依赖项：
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>

msg CMakeLists.txt find_package中加入message_generation
# 不要直接复制这一大段，只需将message_generation加在括号闭合前即可
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation #加入这个
)
#添加编译
add_message_files(
  FILES
  Num.msg
)
#查看消息是否在ROS中起作用了
rosmsg show beginner_tutorials/Num

-------------------------------------------------------------
roscd beginner_tutorials
mkdir srv
roscp rospy_tutorials AddTwoInts.srv srv/AddTwoInts.srv
#package.xml中确保开启message创建和message运行依赖
<build_depend>message_generation</build_depend
msg CMakeLists.txt find_package中加入message_generation
# 不要直接复制这一大段，只需将message_generation加在括号闭合前即可
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation #加入这个
)
#添加编译
add_service_files(
  FILES
  AddTwoInts.srv
)
generate_messages(
  DEPENDENCIES
  std_msgs
)
#查看服务
rossrv show beginner_tutorials/AddTwoInts
```

##### 一、工程搭建

- makefile文件由stm32cube生成后修改

- 事先安装的软件：vscode cortex-debug插件；tup-latest、 openocd 、arm-gdb/gcc 、GNU MCU Eclipse安装到C:\Program Files文件夹，它们的bin文件夹路径加入到path环境变量当中。

- 修改的stm32f1discovery.cfg文件放到C:\Program Files\OpenOCD\0.10.0-13\scripts\board的文件夹；这个make upload烧入的时候用到

##### 二、编译指令

`make`

##### 三、烧入指令

`make upload`

##### 四、进入调试

`Fn+F5`
