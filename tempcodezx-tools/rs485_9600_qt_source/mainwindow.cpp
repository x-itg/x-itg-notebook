#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newdialog.h"
#include "qtimer.h"
#include <QMessageBox>


bool OK;


static const unsigned char aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const unsigned char aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};
unsigned short CRC16x( unsigned char * pucFrame, unsigned short usLen )
{
    unsigned char           ucCRCHi = 0xFF;
    unsigned char           ucCRCLo = 0xFF;
    int             iIndex;
    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( unsigned short )( ucCRCHi << 8 | ucCRCLo );
}
void appendCrcLH(unsigned char * ahead,unsigned short lens)
{
    unsigned short crc16tmpd=CRC16x(ahead,lens);
    *(ahead+lens+0)=(crc16tmpd&0x00ff)>>0;
    *(ahead+lens+1)=(crc16tmpd&0xff00)>>8;
}

MainWindow::MainWindow(QWidget *parent) :// 构造函数 特点：以类名作为函数名，无返回类型
    QMainWindow(parent),//多重继承方式继承了QT中的QMainWindow类
    ui(new Ui::MainWindow)//初始化列表 ：初始化一个MainWindow界面指针，其变量名为ui。
{
    ui->setupUi(this);
    QRegExp regExp("(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])");

    //ui->textEdit->setEnabled(false);
    //ui->textEdit->setEnabled(false);
    int i=0;
    InitTimer();
    InitReadTimer();
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());//
            qDebug() << serial.portName();
            serial.close();
            ui->PortBox->setCurrentIndex(i++);//选择最大的端口号
        }
    }
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(6);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    qDebug() << tr("界面设定成功！");
    on_openButton_clicked();
}

MainWindow::~MainWindow()
{
    qDebug() << tr("界面关闭成功！");
    delete ui;
}

void MainWindow::InitTimer()
{

    //if(NULL == m_timer)
    m_timer = new QTimer;
    //设置定时器是否为单次触发。默认为 false 多次触发
    m_timer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    //m_timer->start(100);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));

}

void MainWindow::InitReadTimer()
{

    //if(NULL == m_timer)
    read_timer = new QTimer;
    //设置定时器是否为单次触发。默认为 false 多次触发
    read_timer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    read_timer->start(1500);
    //定时器触发信号槽
    connect(read_timer, SIGNAL(timeout()), this, SLOT(read_timeUpdate()));

}

//清空接受窗口
void MainWindow::on_clearButton_clicked()
{
    qDebug() << tr("清空接受窗口");
    ui->textEdit->clear();
    ui->textEdit->clear();
    ui->p0Edit->clear();

}

QString MainWindow::SendAppend(QByteArray appQbyte)
{
    QString txstr(appQbyte.toHex());
    int len = txstr.length()/2;
    for(int i=1;i<len;i++)
    {
        txstr.insert(2*i+i-1," ");//每2个插入空格
    }
    return txstr;

}

// 将16进制字符串转换为对应的字节序列
QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();

    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{

                QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    return ret;
}

//发送数据
void MainWindow::on_sendButton_clicked()
{

    unsigned char sendReadCode[6]={0xfa,0x04,0x21,0x00,0x25,0xfb};//读取设备信息
    QByteArray QSsendData((char*)sendReadCode, 6);

    //获取发送区的数据
    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));


}

unsigned short sfwd=0;
unsigned short srfl=0;
unsigned char swarm=0;
unsigned char srebootcnt=0;
unsigned short menudacvalue=0;//同dacvalue
unsigned short ampv1=0;
unsigned short ampv2=0;
unsigned short curr1=0;
unsigned short curr2=0;
unsigned short curr3=0;
unsigned short curr4=0;
unsigned short curr5=0;
unsigned short curr6=0;
unsigned short curr7=0;
unsigned short curr8=0;
signed short ampT1=0;
signed short ampT2=0;
signed short ampT3=0;
unsigned short ampT4=0;//实际dac的数
unsigned char warmFwd=0;
unsigned char warmRFL=0;
unsigned char warmampv1=0;
unsigned char warmampv2=0;
unsigned char warmcurr1=0;
unsigned char warmcurr2=0;
unsigned char warmcurr3=0;
unsigned char warmcurr4=0;
unsigned char warmcurr5=0;
unsigned char warmcurr6=0;
unsigned char warmcurr7=0;
unsigned char warmcurr8=0;
unsigned char warmampT1=0;
unsigned char warmampT2=0;
unsigned char warmampT3=0;
unsigned char warmampT4=0;
unsigned char menupttmode=0;        //0ptt模式关 1ptt模式开
unsigned short menusetPercent=0;    //百分比
unsigned char  menucontrolMod=0;    //控制模式0百分比控制 1主机控制
unsigned char  menusignalmod=0;     //信号模式
unsigned short menusetfreq=0;       //设置的中心频率
unsigned char  menuprotectmod=0;    //0正常保护模式 1放开保护模式
unsigned char  menubaoliu=0;
#pragma pack(1)
typedef struct {
    signed int Ta;//三个温度
    signed int Tb;
    signed int Tc;
    signed int pTa;//断电前三个温度
    signed int pTb;
    signed int pTc;

    signed int FenH;//风机定时  每隔多少小时
    signed int FenM;//风机定时  持续转几分钟
    signed int Tcala1;//温度补偿
    signed int Tcala2;
    signed int Tcala3;
    signed int Tset1;//温度阈值
    signed int Tset2;
    signed int Tset3;
    signed int Tset4;
    signed int cshuiCha;//除湿回差 三个回差值
    signed int cwhuiCha;//超温报警回差
    signed int tzhuiCha;//跳闸回差
    signed int MnTest;//测试 温度值

    signed int addr;//0 无 1 偶  2 奇
    signed int jojy;
    signed int btlv;

    signed int k1;
    signed int k2;
    signed int k3;
    signed int b1;
    signed int b2;
    signed int b3;
    unsigned char macFlag[17];
}RUDATA;


typedef struct {
  unsigned char  head[4];
  unsigned short pack_n;
  unsigned short pack_i;
  unsigned short pack_l;
  unsigned short crcLH;
  unsigned char  tail[4];

}PACKACKDATA;
RUDATA      RunData;
PACKACKDATA PackRecData;

unsigned short cdrcoe[27]={
    5010,//0普通调频
    905,//1脉冲模式12
    2716,//2脉冲模式3
    6224,//3cdr  85  87
    6224,//4cdr  87  89
    6224,//5cdr  89  91
    6224,//6cdr  91  93
    6224,//7cdr  93  95
    6224,//8cdr  95  97
    6224,//9cdr  97  99
    6224,//10cdr 99  101
    6224,//11cdr 101 103
    6224,//12cdr 103 105
    6224,//13cdr 105 107
    6224,//14cdr 107 109
    6224,//15cde 109 110
    2744,//16电视频道1
    3100,//17电视频道2
    3100,//18电视频道3
    3500,//19电视频道4
    2328,//20脉冲模式12
    4601,//21脉冲模式3
    2808,//22拌音频道1
    2808,//23拌音频道2
    2808,//24拌音频道3
    4100, //25拌音频道4
    12000
  };
//根据收到的 处理update包
unsigned char MainWindow::SendAPart(unsigned char *recp,unsigned int reclen)
{
    count.stop();
    QByteArray DataPack;
    QByteArray SendHeadCode;
    unsigned char *pChar;
    unsigned short CrcTemp;
    unsigned int i=0;
    unsigned int j=0;
    unsigned int k=0;

    unsigned char txbuf[256];
    int Len;

    currprogress=(int)(each_part_bar*((double)part_index+1.0));
    ui->progressBar->setValue(currprogress);

    //delaymsec(200);
    if(reclen==0)//第一次发送接收到的长度是0，就是不管怎么样都是要发包的，第二次以后根据接收到的应答数据判断数据是否出错如果有问题就暂停了
    {
        part_index=0;
        DataPack=part_list[part_index];
        pChar=(unsigned char *)DataPack.data();
        part_curIndexLen=DataPack.length();
        i=0;
        txbuf[i++]=0xAA;
        txbuf[i++]=0xBB;
        txbuf[i++]=0xCC;
        txbuf[i++]=0x21+ui->Box_updateMode->currentIndex();

        txbuf[i++]=part_num>>0;//包个数
        txbuf[i++]=part_num>>8;
        txbuf[i++]=part_index>>0;//包序号
        txbuf[i++]=part_index>>8;
        txbuf[i++]=part_curIndexLen>>0;//这个包的长度
        txbuf[i++]=part_curIndexLen>>8;
        for(j=0;j<part_curIndexLen;)//数据部分
        {
            txbuf[i++]=pChar[j++];
        }
        appendCrcLH(txbuf+4,i-4);//追加校验
        i++;
        i++;
        txbuf[i++]=0xAB;
        txbuf[i++]=0xBC;
        txbuf[i++]=0xCD;
        txbuf[i++]=0xDE;
        for(k=0;k<i;k++)
        {
               SendPack[k]= txbuf[k];
        }
        SendPack.resize(i);
        ui->textEdit->append(SendAppend(SendPack));
        serial->write(SendPack);
        return 1;

    }else//收到后再发送都是有长度的
    {
        //得到数值

        pChar=(unsigned char *)&PackRecData;//
        for(j=0;j<sizeof(PACKACKDATA);)
        {
          *pChar++=recp[j++];
        }

        //计算校验
        CrcTemp=CRC16x(recp+4,6);

        if(PackRecData.head[0]!=0xAA        ||
           PackRecData.head[1]!=0xBB        ||
           PackRecData.head[2]!=0xCC        ||
           PackRecData.tail[0]!=0xAB        ||
           PackRecData.tail[1]!=0xBC        ||
           PackRecData.tail[2]!=0xCD        ||
           PackRecData.tail[3]!=0xDE        ||
           PackRecData.crcLH!=CrcTemp       ||
           PackRecData.pack_n!=part_num     ||
           PackRecData.pack_i!=part_index   ||
           PackRecData.pack_l!=part_curIndexLen
           )
        {
            failedflag=1;//有问题但是不打断进程
            part_index=part_num;//有问题直接跳到最后
            ui->textEdit->append("第"+QString::number(part_index)+"个Pack下发失败size:"+QString::number(PackRecData.pack_l)+"Bytes");
        }else
        {
            ui->textEdit->append("第"+QString::number(part_index)+"个Pack下发成功size:"+QString::number(PackRecData.pack_l)+"Bytes");
        }


        if(part_num>part_index+1)//还有包需要发送
        {
            part_index++;
            DataPack=part_list[part_index];
            pChar=(unsigned char *)DataPack.data();
            part_curIndexLen=DataPack.length();
            i=0;
            txbuf[i++]=0xAA;
            txbuf[i++]=0xBB;
            txbuf[i++]=0xCC;
            txbuf[i++]=0x21+ui->Box_updateMode->currentIndex();

            txbuf[i++]=part_num>>0;//包个数
            txbuf[i++]=part_num>>8;
            txbuf[i++]=part_index>>0;//包序号
            txbuf[i++]=part_index>>8;
            txbuf[i++]=part_curIndexLen>>0;//这个包的长度
            txbuf[i++]=part_curIndexLen>>8;
            for(j=0;j<part_curIndexLen;)//数据部分
            {
                txbuf[i++]=pChar[j++];
            }
            appendCrcLH(txbuf+4,i-4);//追加校验
            i++;
            i++;
            txbuf[i++]=0xAB;
            txbuf[i++]=0xBC;
            txbuf[i++]=0xCD;
            txbuf[i++]=0xDE;
            for(k=0;k<i;k++)
            {
                   SendPack[k]= txbuf[k];
            }
            SendPack.resize(i);
            ui->textEdit->append(SendAppend(SendPack));
            serial->write(SendPack);
            return 1;

        }else //发完了
        {
            on_escBoot_clicked();
            return 1;
        }
    }
    return 0;
}
//收到一条数据
void MainWindow::timeUpdate()
{
    unsigned char *buff;
    float a;
    m_timer->stop();
    if(ComRecBuf.length()!=0)
    {
        unsigned int rlen=ComRecBuf.length();
        buff=(unsigned char *)ComRecBuf.data();

        if(buff[0]==0xAA&&//升级过程中
           buff[1]==0xBB&&
           buff[2]==0xCC&&
           (buff[3]==0x21||buff[3]==0x24)
           )
        {
            SendAPart(buff,rlen);//在升级包处理函数里处理
        }else if(buff[0]==0xDD&&
                 buff[1]==0xCC&&
                 buff[2]==0xBB&&
                 (buff[3]==0x11||buff[3]==0x14)
                 )//读取版本信息的返回
        {
            char st[10];
            st[0]=buff[4];
            st[1]=buff[5];
            st[2]=buff[6];
            st[3]=buff[7];
            st[4]=buff[8];
            st[5]=buff[9];
            st[6]=buff[10];
            st[7]=buff[11];
            st[8]=buff[12];
            st[9]=buff[13];
            unsigned char bootflag=buff[14];
            QString string = QString(QLatin1String(st));
            ui->Edit_Version->setText(string);
            if(bootflag==0)
            {
                ui->BoxMode->setCurrentIndex(0);
                if(buff[3]==0x11)ui->textEdit->append("本机版本号："+string+" Bootloader");
                if(buff[3]==0x14)ui->textEdit->append("远程CAN版本号："+string+" Bootloader");

            }else
            {
                ui->BoxMode->setCurrentIndex(1);
                if(buff[3]==0x11)ui->textEdit->append("本机版本号："+string+" Application");
                if(buff[3]==0x14)ui->textEdit->append("远程CAN版本号："+string+" Application");
            }


        }
         else if(//boot 的返回
                 buff[0]==0xAA&&//升级码
                 buff[1]==0xBB&&
                 buff[2]==0xCC&&
                 (buff[3]==0x11||buff[3]==0x14)&&
                 buff[14]==0xAA&&
                 buff[15]==0xBB&&
                 buff[16]==0xCC&&
                 buff[17]==0xDD)
        {
            char st[10];
            st[0]=buff[4];
            st[1]=buff[5];
            st[2]=buff[6];
            st[3]=buff[7];
            st[4]=buff[8];
            st[5]=buff[9];
            st[6]=buff[10];
            st[7]=buff[11];
            st[8]=buff[12];
            st[9]=buff[13];
            unsigned char bootflag=buff[14];
            QString string = QString(QLatin1String(st));
            if(buff[3]==0x11)ui->textEdit->append("已经下发本机版本号："+string+" 本机重启");
            if(buff[3]==0x14)ui->textEdit->append("远程下发CAN版本号："+string+" CAN设备重启");

            failedflag=0;
            part_curIndexLen=0;
            part_index=0;

        }
        else if(//退出boot 的返回
                buff[0]==0xA0&&
                buff[1]==0xA1&&
                buff[2]==0xA2&&
                (buff[3]==0x11||buff[3]==0x14)&&
                buff[14]==0xA0&&
                buff[15]==0xA1&&
                buff[16]==0xA2&&
                buff[17]==0xA3)
       {
           char st[10];
           st[0]=buff[4];
           st[1]=buff[5];
           st[2]=buff[6];
           st[3]=buff[7];
           st[4]=buff[8];
           st[5]=buff[9];
           st[6]=buff[10];
           st[7]=buff[11];
           st[8]=buff[12];
           st[9]=buff[13];
           unsigned char bootflag=buff[14];
           QString string = QString(QLatin1String(st));
           if(buff[3]==0x11)ui->textEdit->append("本机已经退出Boot进入app版本："+string+" 本机重启");
           if(buff[3]==0x14)ui->textEdit->append("远程CAN已经退出Boot进入app版本"+string+" CAN设备重启");

           failedflag=0;
           part_curIndexLen=0;
           part_index=0;

       }
        else if(buff[0]==0xfa&&buff[1]==0x04&&buff[2]==0x21)//数据返回基本正常
        {
            //sfwd=buff[3];
            //sfwd=(sfwd<<8)+buff[4];
            //ui->Edit_FWD->setText(QString::number(sfwd));

            //srfl=buff[5];
            //srfl=(srfl<<8)+buff[6];
            //a=(float)srfl/10;
            //ui->Edit_RFL->setText(QString::number(a,'f', 1));



        }else if(buff[0]==0xfa&&buff[1]==0x04&&buff[2]==0xF1)
        {
                unsigned char *pchar;
                unsigned short i=3;
                unsigned short j=0;
                pchar=(unsigned char *)&RunData;
                for(j=0;j<sizeof(RUDATA);j++)
                {
                  *pchar++=buff[i++];
                }
                ui->p0Edit->clear();
                ui->p0Edit->append("A项温度  :"+QString::number((float)RunData.Ta/10,'f',1)+"℃");
                ui->p0Edit->append("B项温度  :"+QString::number((float)RunData.Tb/10,'f',1)+"℃");
                ui->p0Edit->append("C项温度  :"+QString::number((float)RunData.Tc/10,'f',1)+"℃");  //功放1 DAC
                ui->p0Edit->append("A项温度M :"+QString::number((float)RunData.pTa/10,'f',1)+"℃");
                ui->p0Edit->append("B项温度M :"+QString::number((float)RunData.pTb/10,'f',1)+"℃");
                ui->p0Edit->append("C项温度M :"+QString::number((float)RunData.pTc/10,'f',1)+"℃");
                ui->p0Edit->append("补转间隔 :"+QString::number(RunData.FenH)+"小时");
                ui->p0Edit->append("补转持续 :"+QString::number(RunData.FenM)+"分钟");
                ui->p0Edit->append("A温度补偿:"+QString::number((float)RunData.Tcala1/10,'f',1)+"℃");
                ui->p0Edit->append("B温度补偿:"+QString::number((float)RunData.Tcala2/10,'f',1)+"℃");
                ui->p0Edit->append("C温度补偿:"+QString::number((float)RunData.Tcala3/10,'f',1)+"℃");
                ui->p0Edit->append("启动阈值:"+QString::number((float)RunData.Tset1/10,'f',1)+"℃");
                ui->p0Edit->append("关闭阈值:"+QString::number((float)RunData.Tset2/10,'f',1)+"℃");
                ui->p0Edit->append("报警阈值:"+QString::number((float)RunData.Tset3/10,'f',1)+"℃");
                ui->p0Edit->append("跳闸阈值:"+QString::number((float)RunData.Tset4/10,'f',1)+"℃");
                ui->p0Edit->append("除湿回差:"+QString::number((float)RunData.cshuiCha/10,'f',1)+"%");
                ui->p0Edit->append("报警回差:"+QString::number((float)RunData.cwhuiCha/10,'f',1)+"℃");
                ui->p0Edit->append("跳闸回差:"+QString::number((float)RunData.tzhuiCha/10,'f',1)+"℃");
                ui->p0Edit->append("测试温度:"+QString::number((float)RunData.MnTest/10,'f',1)+"℃");
                ui->p0Edit->append("串口地址:"+QString::number(RunData.addr));
                ui->p0Edit->append("奇偶校验:"+QString::number(RunData.jojy));
                ui->p0Edit->append("波特率:"+QString::number(RunData.btlv));
                ui->p0Edit->append("系数K1 :"+QString::number(RunData.k1));
                ui->p0Edit->append("系数B1:"+QString::number(RunData.b1));
                ui->p0Edit->append("系数K2:"+QString::number(RunData.k2));
                ui->p0Edit->append("系数B2:"+QString::number(RunData.b2));
                ui->p0Edit->append("系数K3:"+QString::number(RunData.k3));//0正常保护模式 1放开保护模式
                ui->p0Edit->append("系数B3:"+QString::number(RunData.b3));
                if(RunData.macFlag[0]==0)ui->p0Edit->append("风机转标志:"+QString::number(RunData.macFlag[0])+"不转");
                if(RunData.macFlag[0]==1)ui->p0Edit->append("风机转标志:"+QString::number(RunData.macFlag[0])+"转动");
                ui->p0Edit->append("故障标志:"+QString::number(RunData.macFlag[1]));
                if(RunData.macFlag[5]==1)ui->p0Edit->append("A项温度故障:"+QString::number(RunData.macFlag[5])+"过低");
                if(RunData.macFlag[6]==1)ui->p0Edit->append("B项温度故障:"+QString::number(RunData.macFlag[6])+"过低");
                if(RunData.macFlag[7]==1)ui->p0Edit->append("C项温度故障:"+QString::number(RunData.macFlag[7])+"过低");

                if(RunData.macFlag[5]==2)ui->p0Edit->append("A项温度故障:"+QString::number(RunData.macFlag[5])+"过高");
                if(RunData.macFlag[6]==2)ui->p0Edit->append("B项温度故障:"+QString::number(RunData.macFlag[6])+"过高");
                if(RunData.macFlag[7]==2)ui->p0Edit->append("C项温度故障:"+QString::number(RunData.macFlag[7])+"过高");

                if(RunData.macFlag[8]!=0)ui->p0Edit->append("A项缺项故障:"+QString::number(RunData.macFlag[8])+"缺项");
                if(RunData.macFlag[9]!=0)ui->p0Edit->append("B项缺项故障:"+QString::number(RunData.macFlag[9])+"缺项");
                if(RunData.macFlag[4]!=0)ui->p0Edit->append("门开:"+QString::number(RunData.macFlag[4])+"门开");
                if(RunData.macFlag[4]==0)ui->p0Edit->append("门关:"+QString::number(RunData.macFlag[4])+"门关");
                if(RunData.macFlag[2]==0)ui->p0Edit->append("超温报警标志:"+QString::number(RunData.macFlag[2])+"无报警");
                if(RunData.macFlag[2]==1)ui->p0Edit->append("超温报警标志:"+QString::number(RunData.macFlag[2])+"报警");

                if(RunData.macFlag[3]==0)ui->p0Edit->append("跳闸标志:"+QString::number(RunData.macFlag[3])+"未跳闸");
                if(RunData.macFlag[3]==1)ui->p0Edit->append("跳闸标志:"+QString::number(RunData.macFlag[3])+"已跳闸");
                ui->setMacMODE->setCurrentIndex(RunData.macFlag[12]);
                ui->p0Edit->append("模式标志:"+QString::number(RunData.macFlag[12]));
                ui->p0Edit->append("巡回最大标志:"+QString::number(RunData.macFlag[13]));
                ui->p0Edit->append("蜂鸣器标志:"+QString::number(RunData.macFlag[14]));
                ui->p0Edit->append("消音标志:"+QString::number(RunData.macFlag[15]));
                ui->setMacMODE_XY->setCurrentIndex(RunData.macFlag[15]);
                ui->p0Edit->append("巡回最大标志:"+QString::number(RunData.macFlag[16]));

        }
    }
    ui->textEdit->append(SendAppend(ComRecBuf));//显示收到数据

    ComRecBuf.clear();
}
//读取接收到的数据
void MainWindow::Read_Data()
{
    m_timer->start(20);//继续延后100ms再结束

    ComRecBuf.append(serial->readAll());

}



int str2inthandle(char * pstr,unsigned char dilen)
{
  int res;
  res=0;
  unsigned char fuflag=0;
  unsigned char cnt=0;
  unsigned char dilentemp=dilen;
  if(*pstr=='-')
  {
    fuflag=1;
    pstr++;
  }
  while(*pstr>='0' && *pstr<='9')//如果是数字才进行转换，数字0~9的ASCII码：48~57
  {
    res = 10*res+  (*pstr++) -'0';//字符'0'的ASCII码为48,48-48=0刚好转化为数字0
  }

  if(*pstr=='.')//第一次遇到可能是小数点
  {
    pstr++;
    cnt=0;
    while(*pstr>='0' && *pstr<='9'&&dilen>0)//如果是数字才进行转换，数字0~9的ASCII码：48~57
    {
      cnt++;
      dilen--;
      res = 10*res+  (*pstr++) -'0';//字符'0'的ASCII码为48,48-48=0刚好转化为数字0
    }
    while(cnt<dilentemp)//97.50只输入97.5
    {
      cnt++;
      res = 10*res;
    }
  }else//没有小数点还要追加
  {
    while(dilen>0)
    {
      dilen--;
      res = 10*res;
    }
  }

  if(fuflag==1)
  {
    res=0-res;
  }
  return res;
}
//发送设置的数据
void MainWindow::on_setButton_clicked()
{
#if 0
    QString tmp;
    float   ftmp;
    char*  ch;
    QByteArray bytedata ;
    unsigned char sendReadCode[15]={0xfa,0x04,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfb};//设置


    tmp=ui->Edit_Perc->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    menusetPercent=(unsigned short)str2inthandle(ch,1);
    sendReadCode[3]=menusetPercent>>8;
    sendReadCode[4]=menusetPercent;

    menucontrolMod=ui->Box_Auto->currentIndex();


    if(menucontrolMod==0)
    {
        ui->Edit_Dacvalue->setEnabled(false);
    }
    else
    {
        ui->Edit_Dacvalue->setEnabled(true);
    }

    sendReadCode[5]=menucontrolMod;
    menusignalmod=ui->Box_Signal->currentIndex();
    sendReadCode[6]=menusignalmod;

    tmp=ui->Edit_Dacvalue->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();

    menudacvalue=(unsigned short)str2inthandle(ch,0);
    sendReadCode[7]=menudacvalue>>8;
    sendReadCode[8]=menudacvalue;


    tmp=ui->Edit_Freq->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();

    menusetfreq=(unsigned short)str2inthandle(ch,2);
    sendReadCode[9]=menusetfreq>>8;
    sendReadCode[10]=menusetfreq;

    menuprotectmod=ui->Box_ProtectMod->currentIndex();
    sendReadCode[11]=menuprotectmod;
    menupttmode=ui->Box_PttMod->currentIndex();
    sendReadCode[12]=menupttmode;

    unsigned int sumhe=0;
    for(int i=1;i<13;i++)
    {
        sumhe=sumhe+sendReadCode[i];
    }
    sendReadCode[13]=sumhe;

    QByteArray QSsendData((char*)sendReadCode, 15);

    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));
#endif
}

void MainWindow::on_openButton_clicked()
{

    if(ui->openButton->text()==tr("打开串口"))
    {
        qDebug() << tr("打开串口");
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->BaudBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->BitNumBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->ParityBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->StopBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->openButton->setText(tr("关闭串口"));//打开后  变成 关闭串口
        ui->sendButton->setEnabled(true);
        ui->clearButton->setEnabled(true);

        serial->setReadBufferSize(1000);

        //连接信号槽 当串口收到数据并且接收完毕后，会发出一个QSerialPort::readyRead()的信号
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);//把信号和槽函数连接起来

        on_sendButton_clicked();
    }
    else
    {
        qDebug() << tr("关闭串口");
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
        ui->clearButton->setEnabled(false);
    }
}









unsigned int rdex=0;
unsigned int wdex=0;
#define  rwmax 10
void MainWindow::on_PutMessage_clicked()
{

    QString debugstring("Write@");
    if(((wdex+1)%rwmax)!=rdex)//满
    {
        wdex++;
        if(wdex>=rwmax)
        {
            wdex=0;
        }

        debugstring=debugstring+"["+QString::number((wdex>0)?(wdex-1):(rwmax-1))+"] +1--> "+QString::number(wdex);
    }

}

void MainWindow::on_SendMessage_clicked()
{
    QString debugstring("Read @");
    if(rdex!=wdex)//不空
    {
        rdex++;
        if(rdex>=rwmax)
        {
            rdex=0;
        }
        debugstring=debugstring+"["+QString::number((rdex>0)?(rdex-1):(rwmax-1))+"] +1--> "+QString::number(rdex);

    }
}

















void MainWindow::read_timeUpdate()
{
    if(ui->Box_ReadMode->currentIndex()==0)//手动
    {
        read_timer->stop();
    }else
    {
        on_sendButton_clicked();//自动读取
    }


}

void MainWindow::on_Box_ReadMode_activated(const QString &arg1)
{
    if(ui->Box_ReadMode->currentIndex()==0)//手动
    {
        read_timer->stop();
    }else
    {
        read_timer->start(1500);
    }
}



void MainWindow::on_readOtherB_clicked()
{
    unsigned char sendReadCode[6]={0xfa,0x04,0xF1,0x00,0xF5,0xfb};//读取设备调试信息    QByteArray QSsendData((char*)sendReadCode, 6);

    QByteArray QSsendData((char*)sendReadCode, 6);

    //获取发送区的数据
    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));

}

void MainWindow::on_readd0_clicked()
{
    unsigned char sendReadCode[6]={0xfa,0x04,0xF1,0x00,0xF5,0xfb};//读取设备调试信息    QByteArray QSsendData((char*)sendReadCode, 6);

    QByteArray QSsendData((char*)sendReadCode, 6);

    //获取发送区的数据
    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));
}





void MainWindow::on_setMacMODE_activated(const QString &arg1)
{
    QString tmp;
    float   ftmp;
    unsigned char sendReadCode[6]={0xfa,0x04,0xD1,0x00,0xD5,0xFB};//设置

    sendReadCode[3]=ui->setMacMODE->currentIndex();

    unsigned int sumhe=0;
    for(int i=1;i<3;i++)
    {
        sumhe=sumhe+sendReadCode[i];
    }
    sendReadCode[4]=sumhe;
    sendReadCode[5]=0xFB;
    QByteArray QSsendData((char*)sendReadCode, 6);

    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));
}

void MainWindow::on_ChooseFile_clicked()
{
    ui->progressBar->setValue(0);
    fileName = QFileDialog::getOpenFileName( this,tr("文件对话框！"),"D:",tr("烧录文件(*bin);;""所有文件(*)"));
    ui->FilePath->setText(fileName);
}
//遍历文件夹
QString MainWindow::Traversal(QString path)                //遍历文件夹里面的 .h 文件的方法
{
    QDir dir(path);
    QString  retpath;
    int flag=0;
    QFileInfoList list = dir.entryInfoList();
    QDateTime dtcompare=QDateTime::fromString(QString("2020-01-01 23:59:59"), "yyyy-MM-dd hh:mm:ss");

    for (int i = 0; i < list.size(); i++) {

        QFileInfo info = list.at(i);
        if(info.suffix()=="bin")
        {
            QString file_size = tr("%1").arg(info.size());

            QDateTime dt = info.created();

            QString create_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));

            dt = info.lastModified();
            if(dt>dtcompare)
            {
                dtcompare=dt;//取得最新的时间

                retpath=info.path()+"/"+info.fileName();;
                flag=1;

            }

            QString modify_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));

            qDebug() << "文件名:" << info.fileName()  << " 大小(Byte) :" << file_size << " 创建时间 : " << create_time << " 创建时间 : "<<modify_time ;
        }


    }
    if(flag==1)
    {
        ui->textEdit->append(tr("获取文件成功，当前文件路径：%1").arg(retpath));
        return retpath;
    }else
    {
        retpath="Get xxxxxxxxxx.Bin File Failed";
        ui->textEdit->append(tr("获取文件失败，当前文件路径 %1").arg(path));
        return retpath;
    }
}
void MainWindow::on_GetNewFile_clicked()
{
    QString curretFiepath = QDir::currentPath();
    ui->FilePath->setText(Traversal(curretFiepath));
}
void MainWindow::delaymsec(int msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}
void MainWindow::on_UpdateCode_clicked()
{
    ui->textEdit->clear();
    QMessageBox::StandardButton rb ;
    ui->escBoot->setEnabled(false);
    fileName=ui->FilePath->text();


    if(fileName.contains(".bin",Qt::CaseSensitive))
    {

        ui->textEdit->append("文件正在分包...");
        on_qPack_clicked();//分包
        delaymsec(100);
        SendAPart(0,0);//串口模式升级自身


    }else
    {
        ui->textEdit->append("文件无效请重新选择");
        rb= QMessageBox::question(NULL, "提示", "无效文件，请重新选择！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(rb == QMessageBox::Yes)
        {

            on_ChooseFile_clicked();//选择文件
        }


    }
}

void MainWindow::on_EnterBoot_clicked()
{
    {
        ui->textEdit->append("进入Bootloader指令已发送");
        ui->textEdit->append("新版本信息已经保存");
        QByteArray bytes = ui->Edit_Version->text().toUtf8();
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xAA;
        msg[i++]=0xBB;
        msg[i++]=0xCC;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=bytes[0];//0x37;//下面版本号从这里
        msg[i++]=bytes[1];//0x41;
        msg[i++]=bytes[2];//0x2E;
        msg[i++]=bytes[3];//0x62;
        msg[i++]=bytes[4];//0x69;
        msg[i++]=bytes[5];//0x6E;
        msg[i++]=bytes[6];//0x00;
        msg[i++]=bytes[7];//0x6F;
        msg[i++]=bytes[8];//0x63;
        msg[i++]=bytes[9];//0x74;//
        msg[i++]=0xAA;
        msg[i++]=0xBB;
        msg[i++]=0xCC;
        msg[i++]=0xDD;
        serial->write(msg);
        ui->textEdit->append(SendAppend(msg));
        failedflag=0;
        part_curIndexLen=0;
        part_index=0;
    }
}

void MainWindow::on_qPack_clicked()
{
    part_list.clear();//清除分包
    QFile bin_file(ui->FilePath->text());
    QByteArray file_array;
    if(bin_file.open(QIODevice::ReadOnly))
    {//成功读取文件
        ui->textEdit->append("文件打开成功! 确认设备处于升级模式，并连接后即可升级");
        file_array = bin_file.readAll();
    }
    else
    {//读取文件失败
        ui->textEdit->append("文件打开失败，请检查文件路径");
        return;
    }
    ui->textEdit->append("请等待......");
    /************文件分包******************/
    a_part_size=128;//一包纯数据部分大小是1024字节
    allpartsize=file_array.length();
    part_num = file_array.length()/a_part_size + 1;//包的个数
    each_part_bar = 100.0/((double)part_num-1);//每个包占进度条的大小比率
    ui->textEdit->append("分包个数: "+QString::number(part_num));
    qDebug()<<"this data has :" << file_array.length() << "could divide:" << part_num << "each part" << each_part_bar;
    if(!part_list.isEmpty())
    {
        qDebug()<< "not empty," << part_list;
    }
    int part=0;
    for(part = 0; part < part_num; part++)//进行分包
    {
        part_list << file_array.mid( part*a_part_size, a_part_size);


    }
    ui->textEdit->append("文件分包: "+QString::number(part_num-1)+"个128字节+1个"+QString::number(part_list[part-1].length())+"字节");
    part_index=0;//从第0个开始发
    statusCode=0;

    file_array.clear();

}

void MainWindow::on_escBoot_clicked()
{
    if(failedflag==0)
    {

        ui->textEdit->append("退出Boot进入APP指令已发送");
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xA0;
        msg[i++]=0xA1;
        msg[i++]=0xA2;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=0;//0x37;//下面版本号从这里
        msg[i++]=0;//0x41;
        msg[i++]=0;//0x2E;
        msg[i++]=0;//0x62;
        msg[i++]=0;//0x69;
        msg[i++]=0;//0x6E;
        msg[i++]=0;//0x00;
        msg[i++]=0;//0x6F;
        msg[i++]=0;//0x63;
        msg[i++]=0;//0x74;//
        msg[i++]=0xA0;
        msg[i++]=0xA1;
        msg[i++]=0xA2;
        msg[i++]=0xA3;
        serial->write(msg);
        ui->textEdit->append(SendAppend(msg));
        failedflag=0;
        part_curIndexLen=0;
        part_index=0;

    }else
    {
        ui->escBoot->setEnabled(false);
        ui->textEdit->append("升级失败，请按照第①步 使设备进入升级模式，如设备已处于升级模式，请断电重启设备并重新尝试升级。");
    }
}


//读取版本号
void MainWindow::on_readVersion_clicked()
{
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xDD;
        msg[i++]=0xCC;
        msg[i++]=0xBB;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=0x00;
        msg[i++]=0x01;
        msg[i++]=0x02;
        msg[i++]=0x03;
        msg[i++]=0x04;
        msg[i++]=0x05;
        msg[i++]=0x06;
        msg[i++]=0x07;
        msg[i++]=0x08;
        msg[i++]=0x09;
        serial->write(msg);
        ui->textEdit->append("正在读取版本信息");
        ui->textEdit->append(SendAppend(msg));

}



























void MainWindow::on_setMacMODE_XY_activated(const QString &arg1)
{
    QString tmp;
    float   ftmp;
    unsigned char sendReadCode[6]={0xfa,0x04,0xD2,0x00,0xD5,0xFB};//设置

    sendReadCode[3]=ui->setMacMODE_XY->currentIndex();

    unsigned int sumhe=0;
    for(int i=1;i<3;i++)
    {
        sumhe=sumhe+sendReadCode[i];
    }
    sendReadCode[4]=sumhe;
    sendReadCode[5]=0xFB;
    QByteArray QSsendData((char*)sendReadCode, 6);

    serial->write(QSsendData);

    ui->textEdit->append(SendAppend(QSsendData));
}
