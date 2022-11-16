#ifndef V4L2API_H
#define V4L2API_H

#ifdef __cplusplus
extern "C"
{
#endif
    #include <stdio.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <linux/videodev2.h>
    #include <string.h>
    #include <sys/mman.h>
    #include <unistd.h>

#ifdef __cplusplus
}
#endif

/******************************************************
 * 功能：用来完成对arm linux下摄像头处理
 * 具体流程如下：
 * 第一步：打开摄像头
 * 第二步：配置摄像头
 * 第三步：申请内存缓冲区队列
 * 第四步：进行内存映射
 *    以上要放一起 ---- 类似于摄像头头初始化工作
 * 第五步：摄像头数据开启采集
 * 第六步：采集数据 ---- yuyv
 * 第七步：将数据进行格式转换 --- yuyv --->rgb
 * 第八步：停止采集
 * 第九步：如果摄像头不需要停止，把硬件资源要进行释放
 *
 * **************************************************/

class V4l2Api //该类就相当于一个摄像头，所以在创建对象时，要指定摄像头设备的名字
{
public:
    V4l2Api(const char* device = "/dev/video7");
    void video_init();
    void video_start();

    void video_stop();
    void video_destory();

    int get_getframe( char *buffer,int* size);

    bool yuv422torgb24(unsigned char *buffer, unsigned char *rgbdata, int w, int h);
private:

    int fd;
    char device_name[128];
    unsigned char* mptr[4];
    unsigned int size[4];
};

#endif // V4L2API_H
