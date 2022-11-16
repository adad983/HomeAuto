#ifndef LOGINICWINDOW_H
#define LOGINICWINDOW_H

#include <QMainWindow>
#include "readRfid.h"

#include "registerfacewindow.h"
#include "registerkeyboardwindow.h"
#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMessageBox>
#include <QtDebug>

#include <QLabel>
#include <QThread>
#include <QtDebug>

extern unsigned int id;

namespace Ui {
class loginICWindow;
}

class MyLoginThread : public QThread
{
    Q_OBJECT

public:
    MyLoginThread(QObject* parent = nullptr)
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


class loginICWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginICWindow(QWidget *parent = nullptr);
    ~loginICWindow();
private slots:
    void on_opButton_clicked();

    void on_disButton_clicked();
public slots:
    void updateUI(int value);
public:
signals:
    void sendAccount(QString&account);
private:
    Ui::loginICWindow *ui;
    MyLoginThread*             th1;
};

#endif // LOGINICWINDOW_H
