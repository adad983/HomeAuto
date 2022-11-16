#include "softkey.h"
#include <QKeyEvent>
#include <QApplication>

SoftKey::SoftKey(QWidget *parent):QToolButton(parent)
{
    //在构造函数中，建立clicked相关的槽，在槽中实现对应的事件的功能
  connect(this,SIGNAL(clicked(bool)),this,SLOT(button_clicked(bool)));
}

void SoftKey::button_clicked(bool)
{

    //this->text() ----获得按钮控件上的文本内容，比如，删除  this --- 它就代表着ui->pushButton
    if(this->text() == "<-")
       {
        //定义一个事件，Key_Backspace撤消按钮
           QKeyEvent* key = new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier, this->text());
            //通过postEvent将事件key发送出去给QApplication::focusWidget()，
           //在当前窗口界面中有焦点的控件
           QApplication::postEvent(QApplication::focusWidget(),key);
       }else
       {
           QKeyEvent* key = new QKeyEvent(QEvent::KeyPress,'w',Qt::NoModifier, this->text());
           //通过postEvent将事件key的文本内容发送出去，QApplication::focusWidget()当前窗口中，有光标的
           //控件上
           QApplication::postEvent(QApplication::focusWidget(),key);
       }
}
