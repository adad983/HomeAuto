#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QMainWindow>
#include "monitorwindow.h"


#include <QProcess>

#include <QFileDialog>
#include <QListWidgetItem>


#ifdef __cplusplus
extern "C"
{
#endif


#include <stdlib.h>

#ifdef __cplusplus
}
#endif

namespace Ui {
class videoWindow;
}

class videoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit videoWindow(QWidget *parent = nullptr);
    ~videoWindow();
private slots:
    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_addButton_clicked();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_videoListWidget_itemDoubleClicked(QListWidgetItem* item);

    void on_videoListWidget_itemClicked(QListWidgetItem* item);

    void set_selectVideoListWidget_item(int row);
    void on_backButton_clicked();

signals:
    void send_videoListWidget_row(int row);
public:
    QList<QString> videoLists;

    int  index;
    bool isVideoPlay;
    bool isListVideo;
private:
    Ui::videoWindow *ui;
    QProcess*       mProcess;
};

#endif // VIDEOWINDOW_H
