#ifndef REGISTERKEYBOARDWINDOW_H
#define REGISTERKEYBOARDWINDOW_H

#include "loginkeyboardwindow.h"
#include "registericwindow.h"
#include <QMainWindow>

#include <QMessageBox>

#include <QtDebug>
namespace Ui
{
    class registerKeyboardWindow;
}

class registerKeyboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registerKeyboardWindow(QWidget* parent = nullptr);
    ~registerKeyboardWindow();

    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void on_registerButton_clicked();

    void on_backButton_clicked();

    void on_accountEdit_editingFinished();

    void on_passwordEdit_editingFinished();

public:
signals:
    void sendAccount(const QString& account);

private:
    Ui::registerKeyboardWindow* ui;
//    QSqlDatabase db;
};

#endif   // REGISTERKEYBOARDWINDOW_H
