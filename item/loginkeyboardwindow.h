#ifndef LOGINKEYBOARDWINDOW_H
#define LOGINKEYBOARDWINDOW_H

#include "loginfacewindow.h"
#include "registerkeyboardwindow.h"
#include "mainwindow.h"
#include "loginicwindow.h"
#include <QMainWindow>


#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMessageBox>
#include <QtDebug>
namespace Ui
{
    class loginKeyboardWindow;
}

class loginKeyboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginKeyboardWindow(QWidget* parent = nullptr);
    ~loginKeyboardWindow();

    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void on_accountEdit_editingFinished();

    void on_passwordEdit_editingFinished();

    void on_keyBoardLoginButton_clicked();

    void on_faceLoginButton_clicked();

    void on_registerButton_clicked();

    void on_icLoginButton_clicked();

public:
signals:
    void sendAccount(const QString& account);

    void loginFace(int num);

private:
    Ui::loginKeyboardWindow* ui;
    QSqlDatabase db;
};

#endif   // LOGINKEYBOARDWINDOW_H
