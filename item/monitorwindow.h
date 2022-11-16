#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "videowindow.h"
#include "albumwindow.h"
#include "surveillancewindow.h"
namespace Ui {
class monitorWindow;
}

class monitorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit monitorWindow(QWidget *parent = nullptr);
    ~monitorWindow();

private slots:
    void on_backMainButton_clicked();

    void on_videoButton_clicked();

    void on_albumButton_clicked();

    void on_surButton_clicked();

private:
    Ui::monitorWindow *ui;
};

#endif // MONITORWINDOW_H
