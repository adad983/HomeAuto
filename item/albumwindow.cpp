#include "albumwindow.h"
#include "ui_albumwindow.h"

albumWindow::albumWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::albumWindow)
{
    ui->setupUi(this);

    index = 0;

    isPhotoPlay = false;
    isListPhoto = false;

    ui->openButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    connect(this, &albumWindow::send_photoListWidget_row, this,
            &albumWindow::set_selectPhotoListWidget_item);
}

albumWindow::~albumWindow()
{
    delete ui;
}

void albumWindow::on_addButton_clicked()
{
    QString pathname = QFileDialog::getOpenFileName(this);

    photoLists.append(pathname);

    QStringList lists = pathname.split('/');

    ui->photoListWidget->addItem(new QListWidgetItem(lists[lists.size() - 1]));
}

void albumWindow::on_prevButton_clicked()
{
    --index;
    if (index < 0)
    {
        index = photoLists.size() - 1;
    }
    //    index = ++index % videoLists.size();


    QString name = QString("background-color: rgb(0, 0, 0);image: url(%1);").arg(photoLists[index]);
    ui->label->setStyleSheet(name);

    ui->openButton->setEnabled(false);

    emit send_photoListWidget_row(index);
}

void albumWindow::on_nextButton_clicked()
{
    ++index;
    if (index == photoLists.size())
    {
        index = 0;
    }
    QString name = QString("background-color: rgb(0, 0, 0);image: url(%1);").arg(photoLists[index]);

    ui->label->setStyleSheet(name);

    ui->openButton->setEnabled(false);

    emit send_photoListWidget_row(index);
}

void albumWindow::on_backButton_clicked()
{
    monitorWindow* mnw = dynamic_cast<monitorWindow*>(this->parentWidget());
    mnw->show();
    this->close();
}

void albumWindow::set_selectPhotoListWidget_item(int row)
{
    QListWidgetItem* item = ui->photoListWidget->item(row);

    item->setSelected(true);

    QString name = QString("background-color: rgb(0, 0, 0);image: url(%1);").arg(photoLists[index]);

    ui->label->setStyleSheet(name);
}

void albumWindow::on_photoListWidget_itemDoubleClicked(QListWidgetItem* item) {

    index = ui->photoListWidget->row(item);
    ui->openButton->setEnabled(true);

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->openButton->setEnabled(false);
    QString name = QString("background-color: rgb(0, 0, 0);image: url(%1);").arg(photoLists[index]);

    ui->label->setStyleSheet(name);

}

void albumWindow::on_photoListWidget_itemClicked(QListWidgetItem* item)
{
    index = ui->photoListWidget->row(item);
    ui->openButton->setEnabled(true);
}

void albumWindow::on_openButton_clicked()
{
    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);

    //    QString name = QString("background-color: rgb(0, 0, 0);image:
    //    url(F:/window4/QT/item/item/image/clearrobot.png);");
    QString name = QString("background-color: rgb(0, 0, 0);image: url(%1);").arg(photoLists[index]);
    ui->label->setStyleSheet(name);

    ui->openButton->setEnabled(false);
}
