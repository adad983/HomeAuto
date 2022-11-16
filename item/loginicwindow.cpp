#include "loginicwindow.h"
#include "ui_loginicwindow.h"

loginICWindow::loginICWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::loginICWindow)
{
    ui->setupUi(this);

    ui->opButton->hide();
    ui->disButton->hide();

    th1 = new MyLoginThread;
    th1->setObjectName("th1");

    connect(th1, &MyLoginThread::sendMsg, this, &loginICWindow::updateUI);

    on_opButton_clicked();
}

loginICWindow::~loginICWindow()
{
    delete ui;
}

void loginICWindow::updateUI(int value)
{
    on_disButton_clicked();

    QSqlQuery query;
    QString   sqlSelect = "SELECT * FROM userInfo;";
    if (!query.exec(sqlSelect))
    {
        qDebug() << "fail to select db";
    }
    uint    ic;
    QString account;
    bool    loginIc = false;
    while (query.next())
    {
        ic      = query.value("icInfo").toUInt();
        account = query.value("account").toString();
        if (ic == value)
        {
            loginIc = true;
            break;
        }
    }

    //    QString sqlAccount = QString("select account from userInfo where icInfo = '%1'").arg(ic);

    //    if (!query.exec(sqlAccount))
    //    {
    //        qDebug() << "fail to select db";
    //    }
    bool _flag = false;
    if (loginIc)
    {
        //        db.close();
        QMessageBox::information(this, "登录提示", "登录成功");

        MainWindow* mw = new MainWindow(this);
        mw->show();
        connect(this, &loginICWindow::sendAccount, mw, &MainWindow::user_label);
        emit sendAccount(account);

        this->hide();
    }
    else
    {

        int ret = QMessageBox::information(this, "登录提示", "登录失败", QMessageBox::Cancel);
        switch (ret)
        {
            case QMessageBox::Cancel:

                _flag = true;
                break;
        }
    }

    if (_flag)
    {
        loginKeyboardWindow* lkw = dynamic_cast<loginKeyboardWindow*>(this->parentWidget());
        lkw->show();
        this->close();
    }
}

void loginICWindow::on_opButton_clicked()
{
    th1->setRun(true);
    th1->start();

    qDebug() << QString::number(id);
}

void loginICWindow::on_disButton_clicked()
{
    th1->setRun(false);
}
