#ifndef REGISTERICWINDOW_H
#define REGISTERICWINDOW_H

#include "readRfid.h"

#include "registerfacewindow.h"
#include "registerkeyboardwindow.h"
#include "loginfacewindow.h"

#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMessageBox>
#include <QtDebug>

#include <QLabel>
#include <QThread>
#include <QtDebug>

extern unsigned int id;

namespace Ui
{
    class registerICWindow;
}

class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread(QObject* parent = nullptr)
        : QThread(parent)
    {
        isRun = true;
    };
    void run() override
    {
        id = 0;
        while (isRun)
        {
#if 1
            readCardId();
#endif
            qDebug() << QString::number(id);
            if (id != 0)
            {
                emit sendMsg(id);
            }
            sleep(1);
        }
    }
    void setRun(bool value)
    {
        this->isRun = value;
    }
    bool Run() const
    {
        return isRun;
    }

signals:
    void sendMsg(int id);

private:
    bool isRun;
};

class registerICWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registerICWindow(QWidget* parent = nullptr);
    ~registerICWindow();

private slots:
    void on_backButton_clicked();

    void on_loginButton_clicked();


    void on_opButton_clicked();

    void on_disButton_clicked();

public slots:

    void updateUI(int value);
    void receiveAccount(const QString& account);
signals:
    void sendAccount(const QString &account);
private:
    Ui::registerICWindow* ui;
    MyThread*             th1;
//    QSqlDatabase db;
};

#endif   // REGISTERICWINDOW_H
