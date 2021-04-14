#include "Keyboard.h"
#include"mainwindow.h"
#include <QApplication>
#include <QLineEdit>
#include <windows.h>
#include <QDesktopWidget>
using namespace AeaQt;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Keyboard keyboard;
    keyboard.window.raise();

    if( keyboard.window.objectName().isEmpty())
         keyboard.window.setObjectName("window");
    keyboard.window.setWindowTitle("Virtual Keyboard");  // 窗口标题
    keyboard.window.setFixedSize(900, 470);
    keyboard.window.setWindowFlags(Qt::Tool);
    keyboard.CreateSystemTrayIcon();    //创建托盘

    keyboard.show();
    QVBoxLayout *v = new QVBoxLayout;  // 垂直分布
    v->addWidget(&keyboard, 5);  // 垂直分布添加键盘对象, 第二个参数为high
    v->setMargin(20);
    keyboard.window.setLayout(v);  // 当前窗口应用垂直布局
    keyboard.window.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Dialog|Qt::FramelessWindowHint);
    HWND win = (HWND) keyboard.window.winId();
    SetWindowLongPtr(win, GWL_EXSTYLE, WS_EX_NOACTIVATE);
    QRect deskRect = QApplication::desktop()->availableGeometry();
    keyboard.window.move(deskRect.right()/2- keyboard.window.width()/2,deskRect.bottom() - keyboard.window.height());
    keyboard.window.show();
    return a.exec();
}
