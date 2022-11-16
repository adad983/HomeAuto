#include "loginicwindow.h"
#include "loginkeyboardwindow.h"
#include "mainwindow.h"
#include "registericwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
        loginKeyboardWindow w;
//        registerICWindow w;
//    loginICWindow w;
    w.show();
    return a.exec();
}
