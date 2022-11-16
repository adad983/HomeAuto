#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "monitorwindow.h"
#include "loginkeyboardwindow.h"

#include <QFileDialog>
#include <QListWidgetItem>

#include <QtDebug>

#include <QGraphicsOpacityEffect>
#include <QPainter>

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QTimer>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


#ifdef __cplusplus
extern "C"
{
#endif

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void weather_data(QNetworkReply* reply);

    void update_time();

    void on_addMusicButton_clicked();

    void on_prevButton_clicked();

    void on_playButton_clicked();

    void on_nextButton_clicked();

    void on_musicListButton_clicked();

    void on_musicListWidget_itemClicked(QListWidgetItem* item);

    void on_musicListWidget_itemDoubleClicked(QListWidgetItem* item);

    void set_selectMusicListWidget_item(int row);

    void on_cameraControlButton_clicked();

    void on_parlorLightButton_clicked();

    void on_parlorLightButton_2_clicked();

    void on_bedroomLightButton_clicked();

    void on_balconyLightButton_clicked();

    void on_washLightButton_2_clicked();

    void on_washLightButton_clicked();

    void on_balconyDoorButton_clicked();

    void on_balconyDoorButton_2_clicked();

    void on_parlorCurtainButton_clicked();

    void on_parlorCurtainButton_2_clicked();

    void on_parlorAircButton_clicked();

    void on_parlorAircButton_2_clicked();

    void on_backButton_clicked();

public slots:
    void user_label(const QString& account);
signals:
    void send_musicListWidget_row(int row);

private:
    Ui::MainWindow* ui;

    int  index;
    bool isMusicPlay;
    bool isListMusic;

    bool parlorLight;
    bool parlorLight_2;
    bool bedroomLight;
    bool balconyLight;
    bool washLight;
    bool washLight_2;
    bool balconyDoor;
    bool balconyDoor_2;
    bool parlorCurtain;
    bool parlorCurtain_2;
    bool parlorAirc;
    bool parlorAirc_2;

    int  fd;
    char buf[2];
    bool isLed;
    bool isLed_2;
    bool isLed_3;
    bool isLed_4;

    QTimer timer;
    QTime  time;
    QDate  ndate;

    QList<QString> musicLists;

    QNetworkAccessManager* manager;
};
#endif   // MAINWINDOW_H
