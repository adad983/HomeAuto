#ifndef REGISTERFACEWINDOW_H
#define REGISTERFACEWINDOW_H

#include <QMainWindow>

namespace Ui {
class registerFaceWindow;
}

class registerFaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registerFaceWindow(QWidget *parent = nullptr);
    ~registerFaceWindow();

private:
    Ui::registerFaceWindow *ui;
};

#endif // REGISTERFACEWINDOW_H
