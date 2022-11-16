# HomeAuto

# 智能家居系统
## 开发环境：Ubuntu16.04    三星S5P6818开发板    Qt Creator(5.14.2)    secureCRT    
## 项目描述：为方便用户对智能家居的操作，实现的基于面向对象思想的系统。用户通过发生对开发板触摸屏的触发事件而发生对应的家居操作。
## 技术要点：【C++、多线程、Json数据解析、Sqlite数据库、RFID射频读写卡、v4l2模块、人脸识别模块、mplayer、ffmpeg】</b>  
用户的登录和注册：用户的数据在数据库有用户名，密码，IC卡，人脸图片数据，用户可以通过自定义的软键盘类，通过光标的确定，开发板按钮的点击，将数据进行输入与数据库用户数据比对，也可以使用RFID射频技术实现的刷卡、使用v4l2调用的百度智能云人脸识别模块的刷脸实行登录操作。注册操作则是将未注册过的用户数据实现数据在数据库的比对、存储；
天气数据显示：通过访问网络上开放http的天气API，对返回的Json数据进行解析，打印至开发板，可以选择特定城市的天气进行显示打印；</b>  
Led灯操作：安装开发板对应的驱动程序，通过write函数给Led设备写入不同的电平，实现开关灯。</b>  
监控摄像头：通过v4l2模块，使用外接的摄像头调用ffmpeg命令得以对摄像头数据的录制和抓拍图像；</b>  
音乐播放器和视频播放器：使用Qt文件类，对音乐和视频进行添加，双击列表控件通过使用mplayer和ffmpeg实现音乐和视频的播放、暂停、上一个、下一个的转换；</b>  
相册：使用Qt文件类，对系统文件进行图片添加，实现获取按钮事件对上，下图片进行切换，展示；</b>  