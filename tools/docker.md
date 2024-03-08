![](.\2024-03-07-22-29-00-image.png)

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


wsl -l -v
wsl --export Ubuntu-22.04 D:\Ubuntu-22.04.tar
wsl --import Ubuntu-22.04 D:\Ubuntu_22.04_WSL_DIR D:\Ubuntu-22.04.tar --version 2
