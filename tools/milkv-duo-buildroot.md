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
2. git clone https://gitee.com/x-itg/duo-buildroot-sdk.git --depth=1
3. cd duo-buildroot-sdk
4. docker run -itd --name duodocker -v C:\x-itg\milkv\duo-buildroot-sdk:/home/work milkvtech/milkv-duo:latest /bin/bash
5. docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo"