- C->S:0x600+NID，CS（Data0） MUX（Data1，2索引 Data3子索引）Data4，5，6，7

- S->C:0x580+NID，CS（Data0） MUX（Data1，2索引 Data3子索引）Data4，5，6，7



- SDO 读 CS=0x60

- SDO 读接收了1字节 CS=0x4F

- SDO 读接收了2字节 CS=0x4B

- SDO 读接收了4字节 CS=0x43

- SDO 读接错误 CS=0x80



- SDO写1字节CS=0x2F

- SDO写2字节CS=0x2B

- SDO写4字节CS=0x23

- SDO写成功接收0x60

- SDO写失败接收0x80
