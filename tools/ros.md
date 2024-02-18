# 三、伺服驱动器的控制逻辑规划

## 3.1 读发 或 写发

- 定时发送读取指令 ：阻塞等待读到的标志被置位或超时则退出阻塞
- 根据[指令]和[状态]写入：发送后阻塞等待设置应答标志被置位或超时则退出阻塞

## 3.2 读收 或 设收

- 串口空闲中断中将接收读取和设置的应答 
- 如果接收到设置的应答则设置应答标志置位
- 如果接收到读取的应答则将数据解析成电压电流状态等

## 3.3 状态校验

- 根据读取到数据比对[状态]，状态不匹配重发 
- 在线状态检测

# 四、canopen CANFestival

## 4.1 驱动器字典 SetupMoman6_8_0.exe

- [FAULHABER Motion Manager](https://faulhaber.com.cn/%E6%94%AF%E6%8C%81/faulhaber-motion-manager/)  
- usb0虚拟设备  
- Configuration Object Browse  
- Open Configuration file   （open .xdc file）
- Download

## 4.2 单片机Festival字典

- install python27  环境变量 
- install wxPython包
- 字典工具安装包(32位和64位 选取安装)\Mongo-canfestival-3-asc-1a25f5151a8d\objdictgen\Gnosis_Utils-1.2.2.tar\Gnosis_Utils-1.2.2\gnosis拷贝到objdictgen文件夹下
- 字典工具安装包(32位和64位 选取安装)\Mongo-canfestival-3-asc-1a25f5151a8d\objdictgen\objdictedit.py   运行

## 4.3 子协议

- 核心子协议 DS301 其他子协议：由不同行业起草，对DS301的补充和扩展，编号DS4xx

## 4.4 SDO 和 PDO

- SDO使用对象字典的Index和SubIndex来访问CAN节点的对象字典，可以读或者写。
- PDO则不使用Index和SubIndex，它是两个CAN节点间约定好传输的数据内容对应哪些对象字典索引，收到数据后就直接读写相应的对象字典索引里的值

# 五、WSL2 下 ROS

- sudo pip install rosdepc

- sudo apt-get install python3-pip

- sudo pip3 install rosdepc

- <mark>sudo rosdepc init</mark>

- <mark>rosdepc update</mark>

- 

- sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" - /etc/apt/sources.list.d/ros-latest.list'

- sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654 

- sudo apt update #更新ROS源的内容

- <mark>echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc</mark>

- source ~/.bashrc

- sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential

- sudo apt-get install python3-pip

- sudo pip install rosdepc

- rosdepc update

- 

- sudo apt-get install python3-roslaunch

- sudo apt-get install ros-noetic-roslaunch

- 

- source /opt/ros/noetic/setup.bash

- sudo apt-get install ros-noetic-ros-tutorials

- roscd roscpp

- source ~/.bashrc

- <mark>mkdir -p ~/catkin_ws/src</mark>

- cd ~/catkin_ws/

- catkin_make

- source devel/setup.bash

- cd ~/catkin_ws/src

- <mark>catkin_create_pkg beginner_tutorials std_msgs rospy roscpp</mark>  #创建包

- cd ~/catkin_ws

- catkin_make #构建

- . ~/catkin_ws/devel/setup.bash #配置环境一下

- rospack depends1 beginner_tutorials #查看包依赖

- sudo apt install python3-rosnode #安装节点查看工具

- rosnode list #查看节点

- roscore #开启核心节点

- rosrun turtlesim turtlesim_node #启动小乌龟节点

- rosrun turtlesim turtle_teleop_key #开启小乌龟遥控节点

- sudo apt-get install ros-noetic-rqt #rqt_graph

- sudo apt-get install ros-noetic-rqt-common-plugins #rqt_graph

- rosrun rqt_graph rqt_graph #打开画图图形

- rostopic echo /turtle1/cmd_vel #显示话题内容

- rostopic type /turtle1/cmd_vel #查看话题消息类型 

- rosmsg show geometry_msgs/Twist #查看消息类型的详细信息

- rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]' ## 发布消息

- rostopic pub /turtle1/cmd_vel geometry_msgs/Twist -r 1 -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, -1.8]'## 1Hz频率发送话题消息

- rosrun rqt_plot rqt_plot # rqt_plot命令可以在滚动时间图上显示发布到某个话题上的数据。这里我们将使用rqt_plot命令来绘制正被发布到/turtle1/pose话题上的数据。首先，在一个**新终端**中输入： 

- sudo apt-get install ros-noetic-rqt ros-noetic-rqt-common-plugins ros-noetic-turtlesim

- rosrun rqt_console rqt_console #连接到了ROS的日志框架

- rosrun rqt_logger_level rqt_logger_level #节点运行时改变输出信息的详细级别

- msg（消息）：msg文件就是文本文件，用于描述ROS消息的字段。它们用于为不同编程语言编写的消息生成源代码。

- srv（服务）：一个srv文件描述一个服务。它由两部分组成：请求（request）和响应（response）。

- source ~/catkin_ws/devel/setup.bash

- roscd beginner_tutorials

- mkdir msg

- echo "int64 num" > msg/Num.msg  #我们将在之前创建的软件包里定义一个新的消息。

- <build_depend>message_generation</build_depend> 

- <exec_depend>message_runtime</exec_depend> #添加到/home/z/catkin_ws/src/beginner_tutorials/package.xml

- rosmsg show beginner_tutorials/Num

# 六 、WSL 图形界面
- https://learn.microsoft.com/zh-cn/windows/wsl/install-manual#step-4--download-the-linux-kernel-update-package
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
- 管理员运行powershell：dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
- 内核更新下载安装：https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi
- 管理员运行powershell：wsl --set-default-version 2
- 下载ubuntu1804： https://wslstorestorage.blob.core.windows.net/wslblob/Ubuntu_1804.2019.522.0_x64.appx
- 管理员运行powershell：Add-AppxPackage .\Ubuntu_1804.2019.522.0_x64.appx
- 双击安装Ubuntu_1804.2019.522.0_x64.appx
## 安装SYSTEMCTL
- git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
- cd ubuntu-wsl2-systemd-script/
- bash ubuntu-wsl2-systemd-script.sh --force
- wsl --shutdown  #去windows cmd下重启wsl
- wsl #启动ubuntu
- sudo apt update 

## 安装 Xubuntu xrdp
- sudo apt install -y ubuntu-desktop
- sudo apt install -y xubuntu-desktop
- sudo apt install -y xrdp
- sudo adduser xrdp ssl-cert
- sudo ufw allow 3390
- sudo sed -i 's/port=3389/port=3390/g' /etc/xrdp/xrdp.ini
- sudo echo xfce4-session > ~/.xsession
- sudo nano /etc/xrdp/sesman.ini   #将`KillDisconnected`的值修改为`true`,保存退出
- sudo systemctl restart xrdp 


## wsl2应用程序直接打开gui
- 下载VxSrv安装时注意勾选Disable access control:    https://nchc.dl.sourceforge.net/project/vcxsrv/vcxsrv/1.20.14.0/vcxsrv-64.1.20.14.0.installer.exe
- sudo apt install -y x11-apps
- echo 'export DISPLAY=172.23.80.1:0' >> ~/.bashrc
- source ~/.bashrc