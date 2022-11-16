
#if 1

#    include "readRfid.h"

unsigned int id;
/*
在设计串口程序之前，首先要明确需求中的串口配置的要求：
UART 接口一帧的数据格式为1 个起始位，8 个数据位、无奇偶校验位、1 个停止位，波特率固定为 9600。

*/
#    define COM_NAME "/dev/ttySAC1"   // con2

int init_tty(int fd)
{
    struct termios old_cfg, new_cfg;

    // 1.要配置串口之前，要把原来的串口信息获取，在此基础上再增加新的配置
    // int tcgetattr (int __fd, struct termios *__termios_p)
    if (tcgetattr(fd, &old_cfg) != 0)
    {

        perror("tcgetattr");
        return -1;
    }
    // 2.设置串口的通信模式  void cfmakeraw (struct termios *__termios_p)
    //将要配置的串口信息结构体清零 void bzero(void *s, size_t n);
    bzero(&new_cfg, sizeof(new_cfg));
    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);

    // 3. 配置串口的波特率  数据位  停止位  奇偶校验位
    // cfsetospeed (struct termios *__termios_p, speed_t __speed) 和 int cfsetispeed (struct termios
    // *__termios_p, speed_t __speed)
    cfsetispeed(&new_cfg, B9600);
    cfsetospeed(&new_cfg, B9600);

    //激活选项：CLOCAL | CREAD
    new_cfg.c_cflag |= CLOCAL | CREAD;

    //设置数据位:8位
    new_cfg.c_cflag &= ~CSIZE; /* 用数据位掩码清空数据位设置 */
    new_cfg.c_cflag |= CS8;
    //设置无校验
    new_cfg.c_cflag &= ~PARENB;

    //设置停止位：1位
    new_cfg.c_cflag &= ~CSTOPB;

    // 4.清空缓冲区
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN]  = 4;
    // int tcflush (int __fd, int __queue_selector)
    tcflush(fd, TCIFLUSH);

    // 5.将新配置好的串口信息，设置给串口 int tcsetattr (int __fd, int __optional_actions,const
    // struct termios *__termios_p)
    if (tcsetattr(fd, TCSANOW, &new_cfg) != 0)
    {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}
//计算校验和
unsigned char calc_BCC(unsigned char* SerBfr)
{
    unsigned char BCC = 0;
    int           i   = 0;
    for (; i < (SerBfr[0] - 2); i++)
    {

        BCC ^= *(SerBfr + i);
    }
    return ~BCC;
}


//请求A
int piccRequest(int fd)
{
    // 1. 根据数据帧的格式，定义数据格式
    unsigned char  wbuf[128], rbuf[128];
    fd_set         rfds;
    struct timeval tv;
    int            retval;

    //清零数组


    memset(wbuf, 0, 128);
    memset(rbuf, 0, 128);
    // 2.根据具体的要求，给数组赋值
    //主机向从机发起请求
    wbuf[0] = 0x07;             //数据帧长度:0x07
    wbuf[1] = 0x02;             //命令类型：ISO14443A类型
    wbuf[2] = 0x41;             // 请求A 65
    wbuf[3] = 0x01;             //信息长度：1B
    wbuf[4] = 0x52;             //具体的信息内容
    wbuf[5] = calc_BCC(wbuf);   //校验和
    wbuf[6] = 0x03;             //结束符

    // 3. 将请求的数据帧写入到串口
    write(fd, wbuf, 7);

    // 4.等待下位机的响应 --- io复用来监听--select


    /* Watch fd to see when it has input. */

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /* Wait up to 300 seconds. */

    tv.tv_sec  = 300;
    tv.tv_usec = 0;
    //对我们这个程序来讲，只有读文件描述符，没有写 和 异常
    // select函数的第一个参数，就是三个集合中最大的加1
    retval = select(1 + fd, &rfds, NULL, NULL, &tv);

    switch (retval)
    {

        case -1:
            perror("piccRequest\n");
            break;
        case 0:
            printf("timeout\n");
            break;

        default:
            //表示 从机有数据返回，然后读取串口的数据
            retval = read(fd, rbuf, 8);
            if (retval < 0)
            {
                printf("piccRequest %d\n", retval);
            }

            //如果返回的状态码为0，则表示成功
            if (rbuf[2] == 0x00)
            {
                printf("piccRequest sucess\n");
                return 0;
            }
    }
    FD_CLR(fd, &rfds);

    return 1;
}

//防碰撞
int piccAnticol(int fd)
{

    // 1. 根据数据帧的格式，定义数据格式
    unsigned char  wbuf[128], rbuf[128];
    fd_set         rfds;
    struct timeval tv;
    int            retval;


    //清零数组


    memset(wbuf, 0, 128);
    memset(rbuf, 0, 128);
    // 2.根据具体的要求，给数组赋值
    //主机向从机发起请求
    wbuf[0] = 0x08;   //数据帧长度:0x08
    wbuf[1] = 0x02;   //命令类型：ISO14443A类型
    wbuf[2] = 0x42;   // 防碰撞B 66
    wbuf[3] = 0x02;   //信息长度：2B
    wbuf[4] = 0x93;   //具体的信息内容
    wbuf[5] = 0x00;
    wbuf[6] = calc_BCC(wbuf);   //校验和
    wbuf[7] = 0x03;             //结束符

    // 3. 将请求的数据帧写入到串口
    write(fd, wbuf, 8);

    // 4.等待下位机的响应 --- io复用来监听--select


    /* Watch fd to see when it has input. */

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /* Wait up to 300 seconds. */

    tv.tv_sec  = 300;
    tv.tv_usec = 0;
    //对我们这个程序来讲，只有读文件描述符，没有写 和 异常
    // select函数的第一个参数，就是三个集合中最大的加1
    retval = select(1 + fd, &rfds, NULL, NULL, &tv);

    switch (retval)
    {

        case -1:
            perror("piccAnticol\n");
            break;
        case 0:
            printf("timeout\n");
            break;

        default:
            //表示 从机有数据返回，然后读取串口的数据
            retval = read(fd, rbuf, 10);
            if (retval < 0)
            {
                printf("piccAnticol %d\n", retval);
            }

            //如果返回的状态码为0，则表示成功
            if (rbuf[2] == 0x00)
            {
                printf("piccAnticol sucess\n");
                //将IC卡的ID号读取出来：rbuf[4] rbuf[5] rbuf[6] rbuf[7]
                id = rbuf[7] << 24 | rbuf[6] << 16 | rbuf[5] << 8 | rbuf[4];
                printf("piccAnticol the card id = %x\n", id);
                return 0;
            }
    }
    FD_CLR(fd, &rfds);

    return 1;
}

void readCardId()
{
    // 1.打开串口
    int fd = open(COM_NAME, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    // 2.串口初始化
    init_tty(fd);

    // 3. 根据开发文档中的访问设备的流程：-- 请求(A) --- 防碰撞(B)
    //请求
    if (piccRequest(fd))
    {
        printf("piccRequest falied\n");
        return;
    }
    //防碰撞
    if (piccAnticol(fd))
    {
        printf("piccAnticol falied\n");
        return;
    }
}


#endif
// int main(void)
//{
//	//不断循环读取RFID的卡号
//	while(1)
//	{
//		readCardId();
//		sleep(1);
//	}

//	return 0;
//}
