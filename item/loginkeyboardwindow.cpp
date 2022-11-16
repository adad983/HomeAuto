#include "loginkeyboardwindow.h"
#include "ui_loginkeyboardwindow.h"

loginKeyboardWindow::loginKeyboardWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::loginKeyboardWindow)
{
    ui->setupUi(this);

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    //    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");

    if (!db.open())
    {
        qDebug() << "fail to open";
    }

    ui->accountEdit->installEventFilter(this);
    ui->passwordEdit->installEventFilter(this);
}

loginKeyboardWindow::~loginKeyboardWindow()
{
    delete ui;
}

bool loginKeyboardWindow::eventFilter(QObject* watched, QEvent* event)
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

void loginKeyboardWindow::on_accountEdit_editingFinished() {}

void loginKeyboardWindow::on_passwordEdit_editingFinished() {}

void loginKeyboardWindow::on_keyBoardLoginButton_clicked()
{
    ui->myWidget->hide();

    QSqlQuery query;
    QString   sqlSelect = "SELECT * FROM userInfo;";
    if (!query.exec(sqlSelect))
    {
        qDebug() << "fail to select db";
    }

    bool loginKeyBoard = false;
    while (query.next())
    {
        QString account  = query.value("account").toString();
        QString password = query.value("password").toString();
        if (account == ui->accountEdit->text() && password == ui->passwordEdit->text())
        {
            loginKeyBoard = true;
            break;
        }
    }

    if (loginKeyBoard)
    {
//        db.close();
        QMessageBox::information(this, "登录提示", "登录成功");

        MainWindow* mw = new MainWindow(this);
        mw->show();
        connect(this, &loginKeyboardWindow::sendAccount, mw, &MainWindow::user_label);
        emit sendAccount(ui->accountEdit->text());

        this->hide();
    }
    else
    {
        QMessageBox::information(this, "登录提示", "登录失败");
    }
}

void loginKeyboardWindow::on_faceLoginButton_clicked()
{
//    db.close();
    loginFaceWindow* lfw = new loginFaceWindow(this);
    connect(this,&loginKeyboardWindow::loginFace,lfw,&loginFaceWindow::LoginOfRegister);
    loginFace(1);
    lfw->show();
    this->hide();
}

void loginKeyboardWindow::on_registerButton_clicked()
{
//    db.close();
    registerKeyboardWindow* rkw = new registerKeyboardWindow(this);
    rkw->show();
    this->hide();
}

void loginKeyboardWindow::on_icLoginButton_clicked()
{
//    db.close();
    loginICWindow* liw = new loginICWindow(this);
    liw->show();
    this->hide();
}
