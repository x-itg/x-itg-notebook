- https://milkv.io/zh/docs/duo/getting-started/buildroot-sdk

- Docker->Settings->Docker Engine:
  
  ```
  {
    "builder": {
      "gc": {
        "defaultKeepStorage": "20GB",
        "enabled": true
      }
    },
    "experimental": false,
    "registry-mirrors": [
      "http://dockerproxy.com",
      "http://mirror.baidubce.com",
      "https://docker.m.daocloud.io"
    ]
  }
  ```
1. 在目录：C:\x-itg\milkv 打开cmd
   git config core.protectNTFS false   #Git 在 Windows 下默认开启了 NTFS 保护机制，导致包含不满足 NTFS 文件名的项目无法被成功拉取，且无法切换到这些不满足 NTFS 文件名规范的文件夹中
2. git clone https://gitee.com/x-itg/duo-buildroot-sdk.git --depth=1
3. cd duo-buildroot-sdk
4. docker run -itd --name duodocker -v C:\x-itg\milkv\duo-buildroot-sdk:/home/work milkvtech/milkv-duo:latest /bin/bash
   docker run -itd --name duodocker -v D:\SYNC\duo\duo-buildroot-sdk-develop:/home/work milkvtech/milkv-duo:latest /bin/bash
5. docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo"