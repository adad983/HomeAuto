#include "registerfacewindow.h"
#include "ui_registerfacewindow.h"

registerFaceWindow::registerFaceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registerFaceWindow)
{
    ui->setupUi(this);
}

registerFaceWindow::~registerFaceWindow()
{
    delete ui;
}
