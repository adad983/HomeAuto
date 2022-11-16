#include "videowindow.h"
#include "ui_videowindow.h"

#include <QProcess>
#include <QtDebug>

videoWindow::videoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::videoWindow)
{
    ui->setupUi(this);


    index = 0;

    isVideoPlay = false;
    isListVideo = false;

    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    mProcess = new QProcess;

    connect(this, &videoWindow::send_videoListWidget_row, this,
            &videoWindow::set_selectVideoListWidget_item);

}

videoWindow::~videoWindow()
{
    delete ui;
}


void videoWindow::set_selectVideoListWidget_item(int row)
{
    QListWidgetItem* item = ui->videoListWidget->item(row);

    item->setSelected(true);
}


void videoWindow::on_playButton_clicked()
{
    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    on_stopButton_clicked();
    QString cmd = QString("mplayer -geometry +%1+%2 -x %3 -y %4 -zoom %5")
                      .arg(140)
                      .arg(0)
                      .arg(ui->label->width())
                      .arg(ui->label->height())
                      .arg(videoLists[index]);

    mProcess->start(cmd);
    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
}

void videoWindow::on_stopButton_clicked()
{
    if (mProcess->state() == QProcess::Running)
    {
        mProcess->kill();
        mProcess->waitForFinished();

        ui->stopButton->setEnabled(false);
    }
    ui->label->clear();

    ui->label->setStyleSheet(
        "background-color: rgba(179, 255, 131, 45%);border:1px solid darkkhaki;");
}

void videoWindow::on_addButton_clicked()
{
    on_stopButton_clicked();

    QString pathname = QFileDialog::getOpenFileName(this);

    videoLists.append(pathname);

    QStringList lists = pathname.split('/');

    ui->videoListWidget->addItem(new QListWidgetItem(lists[lists.size() - 1]));
}

void videoWindow::on_prevButton_clicked()
{

    ui->stopButton->setEnabled(true);
    --index;
    if (index < 0)
    {
        index = videoLists.size() - 1;
    }
    //    index = ++index % videoLists.size();

    on_stopButton_clicked();

    QString cmd = QString("mplayer -geometry +%1+%2 -x %3 -y %4 -zoom %5")
                      .arg(140)
                      .arg(0)
                      .arg(ui->label->width())
                      .arg(ui->label->height())
                      .arg(videoLists[index]);

    mProcess->start(cmd);

    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit send_videoListWidget_row(index);
}

void videoWindow::on_nextButton_clicked()
{
    ui->stopButton->setEnabled(true);
    ++index;
    if (index == videoLists.size())
    {
        index = 0;
    }

    on_stopButton_clicked();

    QString cmd = QString("mplayer -geometry +%1+%2 -x %3 -y %4 -zoom %5")
                      .arg(140)
                      .arg(0)
                      .arg(ui->label->width())
                      .arg(ui->label->height())
                      .arg(videoLists[index]);

    mProcess->start(cmd);

    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit send_videoListWidget_row(index);
}

void videoWindow::on_videoListWidget_itemDoubleClicked(QListWidgetItem* item)
{

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    isVideoPlay = true;

    int row = ui->videoListWidget->row(item);
#if 1
    on_stopButton_clicked();

    QString cmd = QString("mplayer -geometry +%1+%2 -x %3 -y %4 -zoom %5")
                      .arg(140)
                      .arg(0)
                      .arg(ui->label->width())
                      .arg(ui->label->height())
                      .arg(videoLists[row]);

    mProcess->start(cmd);
#endif
    index = row;
    qDebug() << videoLists[row];
}

void videoWindow::on_videoListWidget_itemClicked(QListWidgetItem* item)
{
    ui->playButton->setEnabled(true);
    index = ui->videoListWidget->row(item);
}

void videoWindow::on_backButton_clicked()
{
    on_stopButton_clicked();
    monitorWindow *mnw = dynamic_cast<monitorWindow*>(this->parentWidget());
    mnw->show();
    this->close();
}
