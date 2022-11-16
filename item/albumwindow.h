#ifndef ALBUMWINDOW_H
#define ALBUMWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "monitorwindow.h"

#include <QFileDialog>
#include <QListWidgetItem>

namespace Ui {
class albumWindow;
}

class albumWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit albumWindow(QWidget *parent = nullptr);
    ~albumWindow();

private slots:
    void on_addButton_clicked();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_backButton_clicked();

    void set_selectPhotoListWidget_item(int row);
    void on_photoListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_photoListWidget_itemClicked(QListWidgetItem *item);

    void on_openButton_clicked();

signals:
    void send_photoListWidget_row(int row);

private:

    QList<QString> photoLists;

    int  index;
    bool isPhotoPlay;
    bool isListPhoto;

    Ui::albumWindow *ui;
};

#endif // ALBUMWINDOW_H
