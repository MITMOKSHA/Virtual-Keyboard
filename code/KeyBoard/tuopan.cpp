#include "KeyBoard.h"
#include "ui_KeyBoard.h"
#include <QDebug>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
tuopan::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* hideBtn = new QPushButton(QStringLiteral("隐藏窗口"),this);
        connect(hideBtn, &QPushButton::clicked, [&]()
        {
            this->hide();
        });

        setWindowFlags(Qt::Tool);
        CreateSystemTrayIcon();    //创建托盘

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::CreateSystemTrayIcon()
{
    //初始化两个项目
    QAction* showAction = new QAction(QStringLiteral("显示"));//项1
    QAction* exitAction = new QAction(QStringLiteral("退出"));//项2
    //项1的点击槽函数
    connect(showAction, &QAction::triggered, this, [=]()
    {
        this->show();
    });
    //项2的点击槽函数
    connect(exitAction , &QAction::triggered, this, [=]()
    {
        qDebug()<<"exit";
        QApplication::exit(0);
    });

    //初始化菜单并添加项
    QMenu* trayMenu = new QMenu(this);//菜单
    trayMenu->addAction(showAction);
    trayMenu->addAction(exitAction );

    //创建一个系统托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/tubiao.jpg"));    //设置托盘图标
    trayIcon->setContextMenu(trayMenu);                                     //设置菜单
    trayIcon->show();
}
