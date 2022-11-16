#include "surveillancewindow.h"
#include "ui_surveillancewindow.h"

surveillanceWindow::surveillanceWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::surveillanceWindow)
{
    ui->setupUi(this);

    this->buffer = new char[640 * 480 * 3];
    this->rgbbuf = new char[640 * 480 * 3];

    second = 0;

    size     = 0;
    v4l2     = nullptr;
    timer    = new QTimer;
    isRecord = false;

    countdownTime = new QTimer;

    connect(timer, &QTimer::timeout, this, &surveillanceWindow::video_play_Ui);
    //    connect(time, &QTimer::timeout, this, &surveillanceWindow::update_time);
    timer->setInterval(50);

    time  = QTime::currentTime();
    ndate = QDate::currentDate();

    ui->recordButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->captureButton->setEnabled(false);
}

surveillanceWindow::~surveillanceWindow()
{
    delete ui;
}

void surveillanceWindow::update_time()
{
    time = time.addMSecs(1000);
}

void surveillanceWindow::on_startButton_clicked()
{
    if (v4l2 == nullptr)
    {
        v4l2 = new V4l2Api("/dev/video7");

        v4l2->video_start();
    }

    timer->start();
    ui->recordButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->captureButton->setEnabled(true);
}

void surveillanceWindow::on_stopButton_clicked()
{
    timer->stop();
    if (v4l2 != nullptr)
    {
        v4l2->video_stop();
        v4l2->video_destory();
        v4l2 = nullptr;
    }

    ui->recordButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    ui->captureButton->setEnabled(false);
}

void surveillanceWindow::on_captureButton_clicked()
{

    QString photoName =
        QString("%1_%2.jpg").arg(ndate.toString("dddd")).arg(time.toString("hh_mm_ss"));
    map.save(photoName);
}

void surveillanceWindow::on_recordButton_clicked()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->captureButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->backButton->setEnabled(false);

    // ffmpeg  -f v4l2 -s 640x480   -t 10 -i /dev/video7 out.avi
    on_stopButton_clicked();
    QString cmd = QString("ffmpeg  -f v4l2 -s 640x480   -t 5 -i /dev/video7 %1_%2.avi &")
                      .arg(ndate.toString("dddd"))
                      .arg(time.toString("hh_mm_ss"));
    system(cmd.toStdString().c_str());
    //    on_startButton_clicked();
    connect(countdownTime, &QTimer::timeout, this, &surveillanceWindow::countDownTime);
    countdownTime->start(1000);
}

void surveillanceWindow::video_play_Ui()
{
    //数据采集
    v4l2->get_getframe(buffer, &size);

    v4l2->yuv422torgb24((unsigned char*)buffer, (unsigned char*)rgbbuf, 640, 480);

    QImage image = QImage((unsigned char*)rgbbuf, 640, 480, QImage::Format_RGB888);

    map = QPixmap::fromImage(image);

    map = map.scaled(ui->label->width(), ui->label->height());

    ui->label->setPixmap(map);
}

void surveillanceWindow::countDownTime()
{

    ++second;
    if (second == 6)
    {
        int ret = QMessageBox::information(this, "提示", "录制成功!", QMessageBox::Ok);
        switch (ret)
        {
            case QMessageBox::Ok:
                second = 0;
                countdownTime->stop();
                on_stopButton_clicked();
                ui->startButton->setEnabled(true);
                ui->backButton->setEnabled(true);
                break;
        }
    }
}

void surveillanceWindow::on_backButton_clicked()
{
    on_stopButton_clicked();
    monitorWindow* mnw = dynamic_cast<monitorWindow*>(this->parentWidget());
    mnw->show();
    this->close();
}
