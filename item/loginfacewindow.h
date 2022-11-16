#ifndef LOGINFACEWINDOW_H
#define LOGINFACEWINDOW_H

#include "loginkeyboardwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QtDebug>

#include "v4l2api.h"

#include <QImage>
#include <QTimer>

#include <QMessageBox>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui
{
    class loginFaceWindow;
}

class loginFaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginFaceWindow(QWidget* parent = nullptr);
    ~loginFaceWindow();

private:
    void judge();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();


    void video_play_Ui();
    void video_label_Ui();
    void on_captureButton_clicked();

    void on_checkButton_clicked();

    void on_recognition_clicked();

    void on_registerButton_clicked();

public slots:
    void user_label(const QString& account);
    void LoginOfRegister(int num);

public:
signals:
    void sendAccount(const QString& account);

private:
    Ui::loginFaceWindow* ui;

    V4l2Api* v4l2;
    QTimer*  timer;
    QTimer*  downtimer;

    QPixmap map;
    QString faceInfo;

    char* buffer;
    char* rgbbuf;
    int   size;
    int   flag;
    int   countdown;


    bool isbreak;
    //创建一个请求的对象
};

#endif   // LOGINFACEWINDOW_H
