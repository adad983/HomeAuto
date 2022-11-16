#include "loginfacewindow.h"
#include "ui_loginfacewindow.h"

#include <QImage>

#include <iostream>

#include "aip-cpp-sdk-0.8.1/face.h"

using namespace std;

loginFaceWindow::loginFaceWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::loginFaceWindow)
{
    ui->setupUi(this);

    isbreak = false;

    ui->accountLabel->hide();

    this->buffer = new char[640 * 480 * 3];
    this->rgbbuf = new char[640 * 480 * 3];

    faceInfo = "";

    size      = 0;
    v4l2      = nullptr;
    timer     = new QTimer;
    downtimer = new QTimer;

    connect(timer, &QTimer::timeout, this, &loginFaceWindow::video_play_Ui);
    connect(downtimer, &QTimer::timeout, this, &loginFaceWindow::video_label_Ui);

    timer->setInterval(50);
    downtimer->setInterval(1000);

    ui->startButton->hide();
    ui->stopButton->hide();
    ui->captureButton->hide();
    ui->recognition->hide();
    ui->checkButton->hide();
    ui->registerButton->hide();


    on_stopButton_clicked();
    on_startButton_clicked();

    countdown = 4;
    downtimer->start();
}

loginFaceWindow::~loginFaceWindow()
{
    delete ui;
}

void loginFaceWindow::judge()
{
    QSqlQuery query;
    QString   sqlSelect = QString("SELECT * FROM userInfo;");

    if (!query.exec(sqlSelect))
    {
        qDebug() << "fail to faceInfo";
    }

    while (query.next())
    {
        faceInfo = query.value("faceInfo").toString();
        //        judge();
        on_recognition_clicked();
        if (isbreak)
        {
            break;
        }
    }
    if (!isbreak)
    {
        on_stopButton_clicked();
        QMessageBox::information(this, "提示", "识别失败!");
        loginKeyboardWindow* lkw = dynamic_cast<loginKeyboardWindow*>(this->parentWidget());
        lkw->show();
        this->close();
    }
}

void loginFaceWindow::on_startButton_clicked()
{
    if (v4l2 == nullptr)
    {
        v4l2 = new V4l2Api("/dev/video7");

        v4l2->video_start();
    }

    timer->start();
}

void loginFaceWindow::on_stopButton_clicked()
{
    timer->stop();
    downtimer->stop();
    if (v4l2 != nullptr)
    {
        v4l2->video_stop();
        v4l2->video_destory();
        v4l2 = nullptr;
    }
}

void loginFaceWindow::video_play_Ui()
{
    //数据采集
    v4l2->get_getframe(buffer, &size);

    v4l2->yuv422torgb24((unsigned char*)buffer, (unsigned char*)rgbbuf, 640, 480);

    QImage image = QImage((unsigned char*)rgbbuf, 640, 480, QImage::Format_RGB888);

    map = QPixmap::fromImage(image);

    map = map.scaled(ui->label->width(), ui->label->height());

    ui->label->setPixmap(map);
}

void loginFaceWindow::on_captureButton_clicked()
{

    map.save("23.jpg");
}

void loginFaceWindow::on_checkButton_clicked()
{
    std::string app_id     = "28369532";
    std::string api_key    = "cbObFmOtW6HS2Z3HGQ0wFHQA";
    std::string secret_key = "cYKdp2EGFRRnWpPNIeWIeWZvLcNYUXaF";

    //创建一个请求的对象
    aip::Face client(app_id, api_key, secret_key);

    Json::Value result;
    //添加图片，并将图片的数据读取出来，然后转换成传入BASE64字符串

    //    QString filepath = QFileDialog::getOpenFileName(this);

    faceInfo = QString("%1.jpg").arg(ui->accountLabel->text());

    map.save(faceInfo);

    QString filepath = QString("./%1").arg(faceInfo);

    QFile file(filepath);

    if (!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "打开文件失败";
    }

    QByteArray array = file.readAll();

    //在这个类中QByteArray，提供一个接口可以直接转BASE64

    // QString data  =  array.toBase64();
    std::string image      = array.toBase64().toStdString();
    std::string image_type = "BASE64";

    // 调用人脸检测
    result = client.detect(image, image_type, aip::null);

    // 如果有可选参数
    std::map<std::string, std::string> options;
    options["face_field"]       = "age";
    options["max_face_num"]     = "2";
    options["face_type"]        = "LIVE";
    options["liveness_control"] = "LOW";

    // 带参数调用人脸检测
    result = client.detect(image, image_type, options);


    //输出结果
    cout << result.toStyledString();

    QString msg;
    msg = msg.fromStdString(result.toStyledString());

    QJsonDocument doc    = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject   object = doc.object();

    QString suc = object.value("error_msg").toString();
    if (suc == "SUCCESS")
    {
        on_registerButton_clicked();
    }
    else
    {
        on_stopButton_clicked();
        QMessageBox::information(this, "提示", "人脸识别失败!");
        loginKeyboardWindow* lkw = new loginKeyboardWindow;
        lkw->show();
        this->close();
    }

    //    输出结果
    //    QString msg;
    //    msg = msg.fromStdString(result.toStyledString());
    //    qDebug() << msg;
}


void loginFaceWindow::on_registerButton_clicked()
{
    QSqlQuery query;

    QString sqlUpdate = QString("update userInfo set faceInfo = '%1' where account == '%2';")
                            .arg(faceInfo)
                            .arg(ui->accountLabel->text());
    bool enterFace = false;
    //    bool gotoNew   = false;
    if (query.exec(sqlUpdate))
    {
        //        qDebug()<<"fail to face";
        on_stopButton_clicked();
        QMessageBox::information(this, "提示", "录入成功!");
        enterFace = true;
    }
    else
    {
        on_stopButton_clicked();
        int ret = QMessageBox::information(this, "提示", "录入失败!是否重新录入!",
                                           QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
        switch (ret)
        {
            case QMessageBox::Cancel:
                enterFace = true;
                break;
            case QMessageBox::Ok:
                break;
        }
    }

    if (enterFace)
    {
        loginKeyboardWindow* lkw = new loginKeyboardWindow;
        lkw->show();
        this->close();
    }
    else
    {
        loginFaceWindow* lfw = new loginFaceWindow;
        connect(this, &loginFaceWindow::sendAccount, lfw, &loginFaceWindow::user_label);
        emit sendAccount(ui->accountLabel->text());

        this->close();
    }
}

void loginFaceWindow::on_recognition_clicked()
{

    map.save("_last.jpg");

    //    QString jpgDate =
    std::string app_id     = "28369532";
    std::string api_key    = "cbObFmOtW6HS2Z3HGQ0wFHQA";
    std::string secret_key = "cYKdp2EGFRRnWpPNIeWIeWZvLcNYUXaF";

    //创建一个请求的对象
    aip::Face client(app_id, api_key, secret_key);

    std::string file_content1;
    std::string file_content2;

    Json::Value data;
    Json::Value image_desp_1;
    Json::Value image_desp_2;

    QString faceJpg = QString("./%1").arg(faceInfo);

    qDebug() << faceJpg;

    aip::get_file_content(faceJpg.toStdString().c_str(), &file_content1);

    image_desp_1["image"] = aip::base64_encode(file_content1.c_str(), (int)file_content1.size());
    image_desp_1["image_type"] = "BASE64";

    aip::get_file_content("./_last.jpg", &file_content2);
    image_desp_2["image"] = aip::base64_encode(file_content2.c_str(), (int)file_content2.size());
    image_desp_2["image_type"] = "BASE64";

    data[0] = image_desp_1;
    data[1] = image_desp_2;

    Json::Value result = client.match(data);

    cout << result.toStyledString();

    QString msg;
    msg = msg.fromStdString(result.toStyledString());

    QJsonDocument doc    = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject   object = doc.object();

    QJsonObject resu  = object.value("result").toObject();
    double      score = resu.value("score").toDouble();

    if (score > 80.0)
    {
        on_stopButton_clicked();
        isbreak = true;

        QString   selectAll = QString("select account from userInfo where faceInfo == '%1';").arg(faceInfo);
        QSqlQuery query;

        if (!query.exec(selectAll))
        {
            qDebug() << "fail";
        }
        QString account;
        while (query.next())
        {
             account = query.value("account").toString();
        }


        QMessageBox::information(this, "提示", "识别成功!");
        MainWindow* mw = new MainWindow;
        connect(this, &loginFaceWindow::sendAccount, mw, &MainWindow::user_label);
        emit sendAccount(account);
        mw->show();
        this->close();
    }
    //    else
    //    {
    //        on_stopButton_clicked();
    //        QMessageBox::information(this, "提示", "识别失败!");
    //        loginKeyboardWindow* lkw = dynamic_cast<loginKeyboardWindow*>(this->parentWidget());
    //        lkw->show();
    //        this->close();
    //    }
}

void loginFaceWindow::user_label(const QString& account)
{
    ui->accountLabel->setText(account);
}

void loginFaceWindow::LoginOfRegister(int num = 0)
{
    flag = num;
}

void loginFaceWindow::video_label_Ui()
{
    ui->label_2->setText(QString::number(--countdown));
    if (0 == countdown)
    {
        if (flag == 1)
        {
            judge();
        }
        else
        {
            on_checkButton_clicked();
        }
    }
}
