#include "monitorwindow.h"
#include "ui_monitorwindow.h"

monitorWindow::monitorWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::monitorWindow)
{
    ui->setupUi(this);
}

monitorWindow::~monitorWindow()
{
    delete ui;
}

void monitorWindow::on_backMainButton_clicked()
{
    MainWindow* mw = dynamic_cast<MainWindow*>(this->parentWidget());

    mw->show();

    this->close();
}

void monitorWindow::on_videoButton_clicked()
{
    videoWindow* vw = new videoWindow(this);
    vw->show();
    this->hide();
}

void monitorWindow::on_albumButton_clicked()
{
    albumWindow* aw = new albumWindow(this);
    aw->show();
    this->hide();
}

void monitorWindow::on_surButton_clicked()
{
    surveillanceWindow *sw = new surveillanceWindow(this);
    sw->show();
    this->hide();
}
