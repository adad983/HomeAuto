#ifndef SURVEILLANCEWINDOW_H
#define SURVEILLANCEWINDOW_H

#include "monitorwindow.h"
#include <QMainWindow>

#include <QMessageBox>

#include <QFile>
#include <QFileDialog>
#include <QtDebug>

#include "v4l2api.h"

#include <QImage>
#include <QTimer>
#include <QTime>
#include <QDate>

#include <QMessageBox>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui
{
    class surveillanceWindow;
}

class surveillanceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit surveillanceWindow(QWidget* parent = nullptr);
    ~surveillanceWindow();

private slots:

    void update_time();

    void on_backButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_captureButton_clicked();

    void on_recordButton_clicked();

    void video_play_Ui();

    void countDownTime();
signals:
    void countdownStart();
public:
    Ui::surveillanceWindow* ui;

    V4l2Api* v4l2;
    QTimer*  timer;
    QTimer*  countdownTime;
    QTime    time;
    QDate    ndate;
    QPixmap  map;

    bool isRecord;

    char* buffer;
    char* rgbbuf;
    int   size;

    int second;
};

#endif   // SURVEILLANCEWINDOW_H
