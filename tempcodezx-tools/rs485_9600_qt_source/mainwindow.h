#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileDialog>
#include <QTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_clearButton_clicked();

    void on_sendButton_clicked();

    void on_openButton_clicked();

    void Read_Data();



    void on_PutMessage_clicked();

    void on_SendMessage_clicked();

    void timeUpdate(void);//串口收完回调
    void read_timeUpdate();
    void on_setButton_clicked();
    void on_Box_ReadMode_activated(const QString &arg1);

    void on_readOtherB_clicked();

    void on_readd0_clicked();

    void on_setMacMODE_activated(const QString &arg1);

    void on_ChooseFile_clicked();

    void on_GetNewFile_clicked();

    void on_UpdateCode_clicked();

    void on_EnterBoot_clicked();

    void on_qPack_clicked();

    void on_escBoot_clicked();

    void on_readVersion_clicked();








    void on_setMacMODE_XY_activated(const QString &arg1);

private:
    QByteArray HexStringToByteArray(QString HexString);

    QTimer *m_timer;
    QTimer *read_timer;
    void InitTimer(void);
    void InitReadTimer(void);
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTcpServer *mServer;//tcpf服务器
    QTcpSocket *mSocket;//套接字
    void sendset (unsigned short regaddr,unsigned short value);
    QString SendAppend(QByteArray appQbyte);
    QByteArray ComRecBuf;

    QString fileName;
    QString Traversal(QString path);
    void delaymsec(int msec);
    QList<QByteArray> part_list;//升级所用分包
    QTimer count ;
    double progress_value = 0.0;//进度条
    double each_part_bar;//每包所占进度条比率大小
    int a_part_size = 128;//一包所占字节数
    int allpartsize=0;
    int part_num;//包数
    int part_index;//包序号
    int part_curIndexLen=0;//当前包长度
    int currprogress;//当前进度条显示
    int statusCode=1;//0 开始第一包  1发的过程当中  2发完了
    QByteArray SendPack;//当前发送的内容
    int failedflag=0;
    int readcoeflag=0;
    unsigned char  SendAPart(unsigned char *recp,unsigned int reclen);
};

typedef struct uS0
{
    unsigned short Freq;
    unsigned short Dbuv;
    unsigned short Swon;
}unitPara;
typedef struct uS1
{
    unitPara nPara[8];
    unsigned short allsw;
}RDNAdata;
#endif // MAINWINDOW_H
