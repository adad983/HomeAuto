#include "registerkeyboardwindow.h"

#include "ui_registerkeyboardwindow.h"
#include <QMessageBox>

registerKeyboardWindow::registerKeyboardWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::registerKeyboardWindow)
{
    ui->setupUi(this);

    //    db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName("user.db");

    //    if (!db.open())
    //    {
    //        qDebug() << "fail to open";
    //    }


    ui->accountEdit->clear();
    ui->passwordEdit->clear();

    //    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName("user.db");

    //    if (!db.open())
    //    {
    //        qDebug() << "fail to open";
    //    }
    QSqlQuery query;
    QString   sqlTable = "create table if not exists userInfo("
                       "id integer PRIMARY KEY AUTOINCREMENT,"
                       "account varchar(20),"
                       "password varchar(20),"
                       "icInfo text,"
                       "faceInfo text"
                       ")";
    // exec to go to sql
    if (!query.exec(sqlTable))
    {
        qDebug() << "fail to create db";
    }

    ui->accountEdit->installEventFilter(this);
    ui->passwordEdit->installEventFilter(this);
}

registerKeyboardWindow::~registerKeyboardWindow()
{
    delete ui;
}

bool registerKeyboardWindow::eventFilter(QObject* watched, QEvent* event)
{
    if ((watched == ui->accountEdit) && (event->type() == QEvent::MouseButtonPress))
    {
        //        callKeyBoard();
        ui->myWidget->show();
        ui->accountEdit->setFocus();
    }
    else if ((watched == ui->passwordEdit) && (event->type() == QEvent::MouseButtonPress))
    {
        //        callKeyBoard();
        ui->myWidget->show();
        ui->passwordEdit->setFocus();
    }
    //    else
    //    {
    //        ui->myWidget->hide();
    //    }

    return QMainWindow::eventFilter(watched, event);
}

void registerKeyboardWindow::on_registerButton_clicked()
{

    ui->myWidget->hide();

    QSqlQuery query;
    QString   sqlSelect = "SELECT account FROM userInfo;";
    if (!query.exec(sqlSelect))
    {
        qDebug() << "fail to select db";
    }

    bool registerUser = true;
    while (query.next())
    {
        QString account = query.value("account").toString();
        //        QString password = query.value("password").toString();
        if (account == ui->accountEdit->text())
        {
            registerUser = false;
            break;
        }
    }

    bool    gotoNew = false;
    QString sqlInsert;
    if (registerUser)
    {
        int ret   = QMessageBox::information(this, "注册提示",
                                           "注册成功!是否进一步完成人脸识别绑定和IC卡绑定?",
                                           QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
        sqlInsert = QString("insert into userInfo(account,password) values ('%1','%2');")
                        .arg(ui->accountEdit->text())
                        .arg(ui->passwordEdit->text());
        if (!query.exec(sqlInsert))
        {
            qDebug() << "注册失败!";
        }
        switch (ret)
        {
            case QMessageBox::Cancel:
                on_backButton_clicked();
                break;
            case QMessageBox::Ok:
                gotoNew = true;
                break;
        }
    }
    else
    {
        QMessageBox::information(this, "注册提示", "注册失败,该账号可能已经存在!",
                                 QMessageBox::Close);
    }

    if (gotoNew)
    {
        //        db.close();
        registerICWindow* riw = new registerICWindow(this);

        connect(this, &registerKeyboardWindow::sendAccount, riw, &registerICWindow::receiveAccount);
        emit sendAccount(ui->accountEdit->text());

        riw->show();
        this->hide();
    }
}

void registerKeyboardWindow::on_backButton_clicked()
{
    //    db.close();
    loginKeyboardWindow* lkw = dynamic_cast<loginKeyboardWindow*>(this->parentWidget());
    lkw->show();
    this->hide();
}

void registerKeyboardWindow::on_accountEdit_editingFinished() {}

void registerKeyboardWindow::on_passwordEdit_editingFinished() {}
