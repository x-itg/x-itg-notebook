#### ��װ��msys2 ��msys����������������ָ�װgcc

```
pacman -S base-devel
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-i686-toolchain
pacman -S mingw-w64-clang-i686-toolchain
pacman -Sy
```



#### makefsdata

- ʾ����makefsdata -i
- ����lwip�ṩ�İ���ҳ���ɶ����Ƹ�ʽ�Ĺ��ߣ�
- �������fs�ļ����µ��ļ��Զ�����fsdata.c�������ɺ��fsdata.c�Ƶ�lwip http���̵���ʹ��
- 

#### pack

- ʾ����pack.exe fs/index.shtml fs/info.shtml > packed_fs.c
- ��������packed_fs.c�ļ����Էŵ�mongoose���̵���ʹ��
- pack.exe ��mongoose�ṩ�ģ�Դ��λ�ã�mongoose/test/pack.c 
- ����ָ�cc -o pack pack.c 
- ����������Ҫ��װgcc�����������尲װ������ ��װmsys.md�е�����
  
  

#### window�±���mongoose example

- ������������ C:\msys64\mingw32\bin

- makefile�ļ��� -oǰ����� -lws2_32�����̬��ѡ��
