- 0x700+NID，DLC=2，DATA0:1 去到操作模式+DATA1:NID

- 0x700+NID，DLC=2，DATA0:2 去到停止模式+DATA1:NID

- 0x700+NID，DLC=2，DATA0:128去到预操作模式+DATA1:NID

- 0x700+NID，DLC=2，DATA0:129去到应用复位模式+DATA1:NID

- 0x700+NID，DLC=2，DATA0:130去到通讯复位模式+DATA1:NID

6种操作模式

上电自动切换到预操作状态，主机NMT使从机切换到操作状态，通过NMT操作可切换到其他状态

- 1初始化 

- 2应用复位 

- 3会话复位 

- 4预操作状态 可SDO和NMT 不可PDO

- 5 操作状态  PDO启动 可SDO 可NMT操作

- 6停止状态 收到NMT停止操作后PDO通讯停止 SDO NMT仍可操作
