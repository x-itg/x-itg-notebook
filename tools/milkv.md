# Docker 下编译 Milk-V Duo buildroot环境生成的镜像

1. 下载安装Docker（windows下需要安装wsl2）

2. cmd 命令行运行` docker pull ubuntu:20.04`

3. 运行`docker run -it ubuntu:20.04 /bin/bash ` exit 回车可退出容器命令行

4. 容器命令行下安装编译依赖环境
   
   `sudo apt install pkg-config build-essential ninja-build automake autoconf libtool wget curl git gcc libssl-dev bc slib squashfs-tools android-sdk-libsparse-utils android-sdk-ext4-utils jq python3-distutils tclsh scons parallel ssh-client tree python3-dev python3-pip device-tree-compiler ssh cpio fakeroot libncurses5 flex bison libncurses5-dev genext2fs rsync unzip dosfstools mtools `

5. 下载cmake安装包`wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4-linux-x86_64.sh`
   
   也可以从windows拷贝到容器
   
   `docker cp cmake-3.26.4-linux-x86_64.sh 7181ab9d91a7:/home`

6. 执行权限  
   `chmod +x cmake-3.26.4-linux-x86_64.sh`
   7.编译cmake
   `sudo ./cmake-3.26.4-linux-x86_64.sh --skip-license --prefix=/usr/local/ `

7. 下载sdk
   
   `git clone https://github.com/milkv-duo/duo-buildroot-sdk.git`

8. 执行编译镜像  
   `cd duo-buildroot-sdk`
   
   `./build.sh `
   
   # Docker 下图像识别
   
   - [配置 Docker 开发环境 | Milk-V (milkv.io)](https://milkv.io/zh/docs/duo/application-development/tpu/tpu-docker)

9. docker pull sophgo/tpuc_dev:v3.1

10. docker run --privileged --name DuoTPU -v /workspace -it  sophgo/tpuc_dev:v3.1

11. docker exec -it d982a5da9706263c85dc78eef4574c0717ecd254a3887da2d1d7e43bcde2d400 /bin/bash

12. git clone https://github.com/milkv-duo/tpu-mlir.git

13. source ./tpu-mlir/envsetup.sh

1. https://codeload.github.com/sophgo/tpu-mlir/tar.gz/refs/tags/v1.5-beta.0

2. `docker cp tpu-mlir-1.5-beta.0.tar.gz d982a5da9706263c85dc78eef4574c0717ecd254a3887da2d1d7e43bcde2d400:/workspace/tpu-mlir.tar.gz`

3. `tar -zxvf tpu-mlir.tar.gz`

4. `sudo su`

5. `source ./tpu-mlir-1.5-beta.0/envsetup.sh`

6. `cd ..`返回上级 和tpu-mlir-1.5-beta.0同级目录

7. `mkdir densenet121 && cd densenet121`

8. `wget https://github.com/onnx/models/raw/main/vision/classification/de121/model/densenet-12.tar.gz`
