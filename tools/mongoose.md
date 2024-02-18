#### 安装好msys2 在msys命令行中运行以下指令安装gcc

```
pacman -S base-devel
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-i686-toolchain
pacman -S mingw-w64-clang-i686-toolchain
pacman -Sy
```



#### makefsdata

- 示例：makefsdata -i
- 它是lwip提供的把网页做成二进制格式的工具，
- 它会遍历fs文件夹下的文件自动生成fsdata.c，把生成后的fsdata.c移到lwip http工程当中使用
- 

#### pack

- 示例：pack.exe fs/index.shtml fs/info.shtml > packed_fs.c
- 它会生成packed_fs.c文件可以放到mongoose工程当中使用
- pack.exe 是mongoose提供的，源码位置：mongoose/test/pack.c 
- 编译指令：cc -o pack pack.c 
- 电脑事先需要安装gcc编译器，具体安装方法看 安装msys.md中的描述
  
  

#### window下编译mongoose example

- 环境变量加入 C:\msys64\mingw32\bin

- makefile文件的 -o前面加入 -lws2_32这个静态库选项
