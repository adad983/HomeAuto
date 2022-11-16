#include "v4l2api.h"



V4l2Api::V4l2Api(const char* device)
{
    strcpy(device_name, device);
    video_init();
}

void V4l2Api::video_init()
{
    int ret;
    // 1 打开摄像头
    fd = open(device_name, O_RDWR);
    if (fd < 0)
    {
        perror("打开设备失败");
        return;
    }
    // 2 配置摄像头
    struct v4l2_format vfmt;
    vfmt.type          = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vfmt.fmt.pix.width = 640;   //摄像头画面的宽，要跟底层系统保持一致，不能随意修改
    vfmt.fmt.pix.height      = 480;
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;   //#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y',
                                                    //'U', 'Y', 'V') /* 16  YUV 4:2:2     */

    ret = ioctl(fd, VIDIOC_S_FMT, &vfmt);
    if (ret < 0)
    {
        perror("设置格式失败");
        return;
    }
    // 3 验证是否配置成功
    memset(&vfmt, 0, sizeof(vfmt));
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret       = ioctl(fd, VIDIOC_G_FMT, &vfmt);   //重新从系统中获取设置的信息
    if (ret < 0)
    {
        perror("获取格式失败");
        return;
    }
    //判断设置的信息是否正确
    if (vfmt.fmt.pix.width == 640 && vfmt.fmt.pix.height == 480 &&
        vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)
    {
        printf("设置成功\n");
    }
    else
    {
        printf("设置失败\n");
    }
    // 4 完成内存映射
    struct v4l2_requestbuffers reqbuffers;
    reqbuffers.count  = 4;   //申请4个缓冲区
    reqbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuffers.memory = V4L2_MEMORY_MMAP;
    ret               = ioctl(fd, VIDIOC_REQBUFS, &reqbuffers);
    if (ret < 0)
    {
        perror("申请队列空间失败");
    }
    // 6 把内核缓冲区队列映射到用户空间
    // VIDIOC_QUERYBUF：把 VIDIOC_REQBUFS 中分配的数据缓存转换成物理地址
    //#define VIDIOC_QUERYBUF		_IOWR('V',  9, struct v4l2_buffer)

    // unsigned char* mptr[4];
    // unsigned int size[4];
    struct v4l2_buffer mapbuffer;

    mapbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    for (int i = 0; i < 4; i++)
    {


        mapbuffer.index = i;
        ret             = ioctl(fd, VIDIOC_QUERYBUF, &mapbuffer);
        if (ret < 0)
        {
            perror("查询内核空间队列失败");
            return;
        }

        //真正实现内存映射，mmap接口实现
        /*
        void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);

        */
        mptr[i] = (unsigned char*)mmap(NULL, mapbuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                                       fd, mapbuffer.m.offset);

        size[i] = mapbuffer.length;

        //通知使用完成，再把缓冲区放回给内核
        // VIDIOC_QBUF：把数据放回缓存队列
        //#define VIDIOC_QBUF		_IOWR('V', 15, struct v4l2_buffer)
        ret = ioctl(fd, VIDIOC_QBUF, &mapbuffer);
        if (ret < 0)
        {
            perror("放回失败");
            return;
        }
    }
}

void V4l2Api::video_start()
{
    // 5 打开摄像头数据开关
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret  = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        perror("开启失败\n");
        return;
    }
}

void V4l2Api::video_stop()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret  = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0)
    {
        perror("停止失败\n");
        return;
    }
}

void V4l2Api::video_destory()
{

    close(fd);
    //释放内存 int munmap(void *addr, size_t length);

    for (int i = 0; i < 4; i++)
    {

        munmap(mptr[i], size[i]);
    }
}

//该接口，如果上层应用想要显示实时画面，可以利用定时器或者子线程 - 20ms
int V4l2Api::get_getframe(char* buffer, int* size)
{
    //采集数据
    struct v4l2_buffer readbuffer;
    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret         = ioctl(fd, VIDIOC_DQBUF, &readbuffer);
    if (ret < 0)
    {
        perror("提取数据失败\n");
        return -1;
    }

    //使用内存拷贝
    // void *memcpy(void *dest, const void *src, size_t n);
    memcpy(buffer, mptr[readbuffer.index], readbuffer.length);
    *size = readbuffer.length;

    ret = ioctl(fd, VIDIOC_QBUF, &readbuffer);
    if (ret < 0)
    {
        perror("放回队列失败\n");
        return -1;
    }
    return 0;
}
// rgbdata ---->QImage ----> QPixmap接口 --->QPixmap对象--->  更新到UI控件上--QLabel
bool V4l2Api::yuv422torgb24(unsigned char* buffer, unsigned char* rgbdata, int w, int h)
{
    int r1, g1, b1;
    int r2, g2, b2;

    for (int i = 0; i < w * h / 2; i++)
    {
        char data[4];
        memcpy(data, buffer + i * 4, 4);
        // Y0U0Y1V1  -->[Y0 U0 V1] [Y1 U0 V1]
        unsigned char Y0 = data[0];
        unsigned char U0 = data[1];
        unsigned char Y1 = data[2];
        unsigned char V1 = data[3];

        r1 = Y0 + 1.4075 * (V1 - 128);
        if (r1 > 255)
            r1 = 255;
        if (r1 < 0)
            r1 = 0;
        g1 = Y0 - 0.3455 * (U0 - 128) - 0.7169 * (V1 - 128);
        if (g1 > 255)
            g1 = 255;
        if (g1 < 0)
            g1 = 0;
        b1 = Y0 + 1.779 * (U0 - 128);
        if (b1 > 255)
            b1 = 255;
        if (b1 < 0)
            b1 = 0;
        r2 = Y1 + 1.4075 * (V1 - 128);
        if (r2 > 255)
            r2 = 255;
        if (r2 < 0)
            r2 = 0;
        g2 = Y1 - 0.3455 * (U0 - 128) - 0.7169 * (V1 - 128);
        if (g2 > 255)
            g2 = 255;
        if (g2 < 0)
            g2 = 0;
        b2 = Y1 + 1.779 * (U0 - 128);
        if (b2 > 255)
            b2 = 255;
        if (b2 < 0)
            b2 = 0;

        rgbdata[i * 6 + 0] = r1;
        rgbdata[i * 6 + 1] = g1;
        rgbdata[i * 6 + 2] = b1;
        rgbdata[i * 6 + 3] = r2;
        rgbdata[i * 6 + 4] = g2;
        rgbdata[i * 6 + 5] = b2;
    }

    return true;
}
