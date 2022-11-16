#include "registericwindow.h"
#include "ui_registericwindow.h"

registerICWindow::registerICWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::registerICWindow)
{
    ui->setupUi(this);

    //    db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName("user.db");

    //    if (!db.open())
    //    {
    //        qDebug() << "fail to open";
    //    }

    ui->disButton->hide();
    ui->opButton->hide();
    //    qDebug() << "hello";

    th1 = new MyThread;
    th1->setObjectName("th1");

    connect(th1, &MyThread::sendMsg, this, &registerICWindow::updateUI);
    on_opButton_clicked();
}

registerICWindow::~registerICWindow()
{
    delete ui;
}

void registerICWindow::on_backButton_clicked()
{
    //    db.close();
    registerKeyboardWindow* rkw = dynamic_cast<registerKeyboardWindow*>(this->parentWidget());

    rkw->show();

    this->close();
}

void registerICWindow::on_loginButton_clicked()
{
    //    db.close();
    loginKeyboardWindow* rkw = new loginKeyboardWindow;
    rkw->show();
    this->hide();
}

void registerICWindow::receiveAccount(const QString& account)
{
    ui->accountLabel->setText(account);
}

void registerICWindow::updateUI(int value)
{
    on_disButton_clicked();

    ui->icLabel->setText(QString::number(value));
    ui->label->clear();
    ui->label->setText("读卡成功!");

    QSqlQuery query;
    //    QString   sqlSelect = "SELECT * FROM userInfo;";

    //    if (!query.exec(sqlSelect))
    //    {
    //        qDebug() << "fail to select db";
    //    }

    QString sqlUpdate = QString("update userInfo set icInfo = '%1' where account = '%2';")
                            .arg(ui->icLabel->text())
                            .arg(ui->accountLabel->text());
    //    bool regiIc = false;

    if (!query.exec(sqlUpdate))
    {
        qDebug() << "not";
        //        regiIc = true;
    }

    bool jump = false;
    int  ret  = QMessageBox::information(this, "提示", "是否继续完成人脸注册?",
                                       QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
    switch (ret)
    {
        case QMessageBox::Cancel:
            break;
        case QMessageBox::Ok:
            jump = true;
            break;
    }

    if (jump)
    {
        loginFaceWindow* lfw = new loginFaceWindow;
        connect(this, &registerICWindow::sendAccount, lfw, &loginFaceWindow::user_label);
        sendAccount(ui->accountLabel->text());
        lfw->show();
        this->close();
    }
}

void registerICWindow::on_opButton_clicked()
{
    th1->setRun(true);
    th1->start();

    qDebug() << "on_opButton_clicked()";
    qDebug() << QString::number(id);
}

void registerICWindow::on_disButton_clicked()
{
    th1->setRun(false);
}
