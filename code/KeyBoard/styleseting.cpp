#include "styleseting.h"
#include "ui_styleseting.h"
#include  <QDebug>
StyleSeting::StyleSeting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StyleSeting)
{
    ui->setupUi(this);
}

StyleSeting::~StyleSeting()
{
    delete ui;
}
//退出
void StyleSeting::on_pushButton_4_clicked()
{
    emit QuitSignal();
}
//设置背景颜色
void StyleSeting::on_pushButton_clicked()
{
    emit SetBackColor("SetBackColor");
}
//设置按钮颜色
void StyleSeting::on_pushButton_2_clicked()
{
     emit SetBTColor("SetBTColor");
}
//设置背景图片
void StyleSeting::on_pushButton_3_clicked()
{
       emit SetBackImage();
}

//开启或关闭背景图片
void StyleSeting::on_checkBox_stateChanged(int arg1)
{
   if(arg1 == 0){
       //没选中
       emit SetBackImageState("Close");
   }
   else{
       //选中
       emit SetBackImageState("Open");
   }

}
