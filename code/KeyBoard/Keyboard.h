#ifndef AEA_KEYBOARD_H
#define AEA_KEYBOARD_H

#include "AbstractKeyboard.h"
#include "KeyButton.h"
#include <QLayout>
#include <QListWidget>
#include <QHash>
#include <QPair>
#include <QString>
#include <windows.h>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
namespace AeaQt {

class ChineseWidget : public QListWidget {  // 中文输入法类
    Q_OBJECT
public:
    ChineseWidget(QWidget *parent = nullptr);
    void setText(const QString &text);  // 中文输入法框显示文本
    void SetInputBoxColor(QString Color);
signals:
    void pressedChanged(const int &code, const QString &text);

private slots:
    void onItemClicked(QListWidgetItem *item);


private:
    void addOneItem(const QString &text);  // 对输入法框添加单一控件
    void loadData();  // 载入数据到模式列表m_modes中
    void loadData2();

private:
    QMap<QString, QList<QPair<QString, QString> > > m_data;  // 存储筛选出的词库数据集
};

class Keyboard : public AbstractKeyboard
{
    Q_OBJECT
public:
    Keyboard(QWidget *parent = nullptr);
    QWidget *SetingWin;
    QWidget window;
    QPushButton* hideBtn = new QPushButton(&window);
    QPushButton* closeBtn = new QPushButton(&window);
protected:
    void resizeEvent(QResizeEvent *);  // QWidget中改变大小的事件
signals:
    void signals_SetBtColor(QString);
private slots:
    void switchCapsLock();  // 大小写切换槽函数
    void switchSpecialChar();  // 字符键盘切换槽函数
    void switchEnOrCh();  // 中英文切换槽函数
    void onButtonPressed(const int &code, const QString &text);
    void clearBufferText();  // 清除文本缓存
    void SetingButton();
    void SetingBtClick();
    void SetingWinQuit();
    void OpenQColorDialogWin(QString ClickState);
    void slot_getColor(QColor cor);//获取调色板rgb值
    void setBackImageColor();//设置背景图片
    void BackImageColorState(QString State);//背景图片状态
    void backspace(){
        keybd_event(8,0,0,0);
        keybd_event(8,0,KEYEVENTF_KEYUP, 0);
    }
    void enter(){
        keybd_event(13,0,0,0);
        keybd_event(13,0,KEYEVENTF_KEYUP, 0);
    }
    void space(){
        keybd_event(32,0,0,0);
        keybd_event(32,0,KEYEVENTF_KEYUP, 0);
    }
private:
    KeyButton *createButton(QList<KeyButton::Mode> modes);
    QWidget *chineseBar();  // 中文输入法框


    QHBoxLayout *h1();  // 键布局
    QHBoxLayout *h2();
    QHBoxLayout *h3();
    QHBoxLayout *h4();
    QWidget *candidateList();
    void resizeButton();  // 更新按键大小
    QPushButton *Seting; 
public:
    void sendmessage(QString s);
    void setBackColor();//设置背景色
    void setBtColor();//设置按钮颜色
    void CreateSystemTrayIcon();
private:
    bool m_isChinese;  // 是否是中文输入法
    ChineseWidget *m_chineseWidget;  // 聚合类
    QString m_bufferText;  // 缓存文本
    QColorDialog *m_pColorDialog;
    QString ClickState;
    int m_red;//颜色的三色，红绿蓝
    int m_green;
    int m_blue;
    QList<KeyButton *> bt;
    QString BtColor;
    QString SetingBtColor;
    QString InputBoxColor;
    QString BackColor="QWidget#window{background-color:gray;}";
    QString GetImageFile_name;
    QString SetImageFile_name;
};

}
#endif // AEA_KEYBOARD_H
