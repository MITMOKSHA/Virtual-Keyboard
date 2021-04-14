#include "Keyboard.h"
#include "KeyButton.h"
#include <QVBoxLayout>
#include <QApplication>
#include <windows.h>
#include <QClipboard>
#include <QDebug>
#include <QPalette>
#include "styleseting.h"
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    #include <QScroller>
#endif

#include <QRegExp>
#include <QDebug>

using namespace AeaQt;

typedef QList<KeyButton::Mode> Modes;
typedef QList<Modes> ModesList;

// ICON相对路径常量
const QString BACKSPACE_ICON = ":/Image/backspace.png";
const QString ENTER_ICON     = ":/Image/enter.png";
const QString SPACE_ICON     = ":/Image/space.png";
const QString CAPLOCK_ICON   = ":/Image/caplock.png";
const QString SETING_ICON   = ":/Image/seting.png";
const double BUTTON_WIDTH_RATIO   = 0.09;  // 按键宽度比例
const double BUTTON_HEIGHT_RATIO  = 0.2;  // 按键高度比例

const QList<Modes> modeListBar1 = {  // 模式结构体列表
    {{Qt::Key_Q, "q"}, {Qt::Key_Q, "Q"}, {Qt::Key_1, "1"}},
    {{Qt::Key_W, "w"}, {Qt::Key_W, "W"}, {Qt::Key_2, "2"}},
    {{Qt::Key_E, "e"}, {Qt::Key_E, "E"}, {Qt::Key_3, "3"}},
    {{Qt::Key_R, "r"}, {Qt::Key_R, "R"}, {Qt::Key_4, "4"}},
    {{Qt::Key_T, "t"}, {Qt::Key_T, "T"}, {Qt::Key_5, "5"}},
    {{Qt::Key_Y, "y"}, {Qt::Key_Y, "Y"}, {Qt::Key_6, "6"}},
    {{Qt::Key_U, "u"}, {Qt::Key_U, "U"}, {Qt::Key_7, "7"}},
    {{Qt::Key_I, "i"}, {Qt::Key_I, "I"}, {Qt::Key_8, "8"}},
    {{Qt::Key_O, "o"}, {Qt::Key_O, "O"}, {Qt::Key_9, "9"}},
    {{Qt::Key_P, "p"}, {Qt::Key_P, "P"}, {Qt::Key_0, "0"}},
    {{Qt::Key_unknown, "["}, {Qt::Key_unknown, "【"}, {Qt::Key_Less, "{"}},
    {{Qt::Key_unknown, "]"}, {Qt::Key_unknown, "】"}, {Qt::Key_Greater, "}"}}
};

const QList<Modes> modeListBar2 = {
    {{Qt::Key_A, "a"}, {Qt::Key_A, "A"}, {Qt::Key_unknown, "~"}},
    {{Qt::Key_S, "s"}, {Qt::Key_S, "S"}, {Qt::Key_unknown, "!"}},
  {{Qt::Key_D, "d"}, {Qt::Key_D, "D"}, {Qt::Key_At,      "@"}},
  {{Qt::Key_F, "f"}, {Qt::Key_F, "F"}, {Qt::Key_NumberSign, "#"}},
  {{Qt::Key_G, "g"}, {Qt::Key_G, "G"}, {Qt::Key_Percent, "%"}},
  {{Qt::Key_H, "h"}, {Qt::Key_H, "H"}, {Qt::Key_unknown, "^"}},
  {{Qt::Key_J, "j"}, {Qt::Key_J, "J"}, {Qt::Key_unknown, "&", "&&"}},
  {{Qt::Key_K, "k"}, {Qt::Key_K, "K"}, {Qt::Key_unknown, "*"}},
  {{Qt::Key_L, "l"}, {Qt::Key_L, "L"}, {Qt::Key_unknown, "?"}},
  {{Qt::Key_Semicolon, ";"}, {Qt::Key_unknown, "；"}, {Qt::Key_unknown, ":"}},
  {{Qt::Key_unknown, "'"}, {Qt::Key_unknown, "’"}, {Qt::Key_Less, "”"}},
};

const QList<Modes> modeListBar3 = {
  {{Qt::Key_CapsLock, "", ""}},  // 大小写切换
  {{Qt::Key_Z, "z"}, {Qt::Key_Z, "Z"}, {Qt::Key_ParenLeft, "("}},
  {{Qt::Key_X, "x"}, {Qt::Key_X, "X"}, {Qt::Key_ParenRight, ")"}},
  {{Qt::Key_C, "c"}, {Qt::Key_C, "C"}, {Qt::Key_Minus,     "-"}},
  {{Qt::Key_V, "v"}, {Qt::Key_V, "V"}, {Qt::Key_unknown,   "_"}},
  {{Qt::Key_B, "b"}, {Qt::Key_B, "B"}, {Qt::Key_unknown,   "="}},
  {{Qt::Key_N, "n"}, {Qt::Key_N, "N"}, {Qt::Key_unknown, "+"}},
  {{Qt::Key_M, "m"}, {Qt::Key_M, "M"}, {Qt::Key_Slash,     "/"}},
  {{Qt::Key_unknown, ","}, {Qt::Key_unknown, "，"}, {Qt::Key_Less, "<"}},
  {{Qt::Key_unknown, "."}, {Qt::Key_unknown, "。"}, {Qt::Key_Greater, ">"}},
  {{Qt::Key_Backspace, "", ""}}  // 退格
};

const QList<Modes> modeListBar4 = {
    {{Qt::Key_Mode_switch, "",  "?123"}},
    {{Qt::Key_Context1,    "",  "En"},    {Qt::Key_Context1, "", "中"}},
    {{Qt::Key_Space,       " ", ""}},  // 空格
    {{Qt::Key_Enter,       "",  ""}}  // 换行
};

Keyboard::Keyboard(QWidget *parent) :  // keyboard继承自AbstractKeyboard
    AbstractKeyboard(parent),  // 构造函数初始化表
    m_isChinese(false)  // m_isChinese初始化为false
{
    this->window.setWindowIcon(QIcon(":/Image/windowIcon.jpg"));
    m_chineseWidget = new ChineseWidget(this);
    // 两个参数分别是水平和竖直扩展性
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);  // 默认大小会被忽略，部件将会得到尽可能多的空间
    resizeButton();  // 设置按钮大小并添加相应功能

    QVBoxLayout *layout = new QVBoxLayout(this);  // 总体垂直布局
    layout->setSizeConstraint(QLayout::SetNoConstraint);  // 大小约束
    layout->setSpacing(0);  // 设置间距
    layout->setMargin(10);  // 设置边缘距离

    layout->addWidget(chineseBar(), 10);  // 将中文输入文本框控件添加到this(Keyboard)中
    // 添加键盘按键的局部(每一行)布局
    layout->addLayout(h1(), 15);  // 第二个参数为stretch
    layout->addLayout(h2(), 15);
    layout->addLayout(h3(), 15);
    layout->addLayout(h4(), 15);

    setLayout(layout);  // 配置键盘布局
    // 对中文输入法框的信号与槽
    connect(m_chineseWidget, SIGNAL(pressedChanged(const int &, const QString &)),  // 选择汉字后窗体显示文本
            this, SLOT(onKeyPressed(const int &, const QString &)));
    connect(hideBtn,&QPushButton::clicked,[=]{
        window.hide();
    });
    connect(closeBtn,&QPushButton::clicked,[=]{
        window.close();
        this->close();
    });
    connect(m_chineseWidget, SIGNAL(pressedChanged(const int &, const QString &)), this, SLOT(clearBufferText()));  // 选择汉字后清空缓存文本
    window.setStyleSheet(BackColor);
}

void Keyboard::resizeEvent(QResizeEvent *e)
{
    resizeButton();
}

void Keyboard::switchCapsLock()
{
    QList<KeyButton *> buttons = findChildren<KeyButton *>();  // 返回this的子类的列表即，QList<KeyButton *>
    foreach(KeyButton *button, buttons)  // qt的for循环(类似于for范围)
        button->switchCapsLock();  // 每个按键调用这个函数
}

void Keyboard::switchSpecialChar()
{
    QList<KeyButton *> buttons = findChildren<KeyButton *>();
    foreach(KeyButton *button, buttons)
        button->switchSpecialChar();
}

void Keyboard::switchEnOrCh()
{
    m_isChinese = !m_isChinese;  // 改变中英文的状态(是中文为true)
    QList<KeyButton *> buttons = findChildren<KeyButton *>();
    foreach(KeyButton *button, buttons) {
        if (button->mode().key == Qt::Key_Context1) {  // 若当前模式的按键为中/英按键
            button->switching();
        }
    }
}

void Keyboard::onButtonPressed(const int &code, const QString &text)
{

    if (!m_isChinese) {  // 若不是中文输入
        sendmessage(text);  // AbstractKeyboard的成员函数(其子类Keyboard调用)，显示输出文本
        m_bufferText.clear();  // 清除m_bufferText字符串的内容，使其变为null
        return;
    }

    QRegExp rx("[a-zA-Z]");
    if (!rx.exactMatch(text) && m_bufferText.isEmpty()) {  // 若是输入是字母
        onKeyPressed(code, text);  // 显示输出文本
        return;
    }

    if (code == Qt::Key_Backspace)  // 若是退格
    {
        m_bufferText.chop(1);  // 从字符串尾部开始移除1个字符
        keybd_event(8,0,0,0);
        keybd_event(8,0,KEYEVENTF_KEYUP,0);
    }
    else
        m_bufferText.append(text);  // 否则就继续增加文本
    m_chineseWidget->setText(m_bufferText);
}

void Keyboard::clearBufferText()
{
    m_bufferText.clear();
}

void Keyboard::SetingButton()
{
    Seting = new QPushButton("设置",this);
    connect(Seting,&QPushButton::clicked,this,&Keyboard::SetingBtClick);
    Seting->setFixedSize(100,100);
    Seting->setStyleSheet("QPushButton{background: #003300; border: 2px solid white; border-radius: 5px; margin: 5px;font-size: 25px; color: white;}"
                          "QPushButton:hover{background: #C0C0C0;}"
                          "QPushButton:pressed{background: #808080;}");
}

void Keyboard::SetingBtClick()
{
    SetingWin = new QWidget(this);
    SetingWin->resize(200,200);
    SetingWin->move(60,170);
    SetingWin->setStyleSheet("QWidget{border: 2px solid #000000; border-radius: 5px;background-color:white;}");
    StyleSeting *StyleSet = new StyleSeting(SetingWin);
    connect(StyleSet, SIGNAL(QuitSignal()), this, SLOT(SetingWinQuit()));
    connect(StyleSet, SIGNAL(SetBackColor(QString)), this, SLOT(OpenQColorDialogWin(QString)));
    connect(StyleSet, SIGNAL(SetBTColor(QString)), this, SLOT(OpenQColorDialogWin(QString)));
    connect(StyleSet, SIGNAL(SetBackImage()), this, SLOT(setBackImageColor()));
    connect(StyleSet, SIGNAL(SetBackImageState(QString)), this, SLOT(BackImageColorState(QString)));
    StyleSet->move(0,0);
    SetingWin->show();
}

void Keyboard::SetingWinQuit()
{
    SetingWin->close();
}
//打开调色板
void Keyboard::OpenQColorDialogWin(QString ClickState)
{
    this->ClickState=ClickState;
    m_pColorDialog = new QColorDialog(this);
    connect(m_pColorDialog,SIGNAL(colorSelected(QColor)),this,SLOT(slot_getColor(QColor)));
    m_pColorDialog->exec();
}
//获取调色板颜色
void Keyboard::slot_getColor(QColor cor)
{
    m_red=cor.red();
    m_green=cor.green();
    m_blue=cor.blue();
    if(ClickState=="SetBackColor")
    setBackColor();
    else if(ClickState=="SetBTColor")
    setBtColor();
}
//设置窗口背景颜色
void Keyboard::setBackColor()
{
    BackColor=QString("QWidget#window{background-color:rgb(%1,%2,%3);}").arg(m_red).arg(m_green).arg(m_blue);
    window.setStyleSheet(BackColor);
}
//设置按钮颜色
void Keyboard::setBtColor()
{
    BtColor = QString("AeaQt--KeyButton { background:rgb(%1,%2,%3) ; border: 2px solid white;g border-radius: 5px;" \
                      "margin: 5px;" \
                      "font-size: 25px; color: white;}" \
                      "AeaQt--KeyButton:hover{background:#C0C0C0}"
                      "AeaQt--KeyButton:pressed { background:#808080}").arg(m_red).arg(m_green).arg(m_blue);
    SetingBtColor = QString("QPushButton{background: rgb(%1,%2,%3); border: 2px solid white; border-radius: 5px; margin: 5px;font-size: 25px; color: white;}"
                            "QPushButton:hover{background: #C0C0C0;}"
                            "QPushButton:pressed{background: #808080;}").arg(m_red).arg(m_green).arg(m_blue);
    InputBoxColor = QString("                                                                           \
                                    QListWidget { outline: none; border: 2px solid white; border-radius: 5px; color: yellow; background-color:rgb(%1,%2,%3) ;}    \
                                    QListWidget::Item { width: 50px; height: 50px; }                            \
                                    QListWidget::Item:hover { background: black; color: white; }              \
                                    QListWidget::item:selected { background: black; color: black; }           \
                                    QListWidget::item:selected:!active { background: #00000000; color: black; } \
                                    ").arg(m_red).arg(m_green).arg(m_blue);
    for(int i=0;i<38;i++){
          bt.at(i)->SetBtColor(BtColor);
    }
    Seting->setStyleSheet(SetingBtColor);
    m_chineseWidget->SetInputBoxColor(InputBoxColor);

}

void Keyboard::setBackImageColor()
{
   GetImageFile_name = QFileDialog::getOpenFileName(this,
           tr("Open File"),
           "",
           "",
           0);
       if (!GetImageFile_name.isNull())
       {
           qDebug()<<GetImageFile_name;
           SetImageFile_name = QString("QWidget#window{background-image: url(%1);}").arg(GetImageFile_name);
           window.setStyleSheet(SetImageFile_name);
       }
       else{
           //点的是取消
       }
}

void Keyboard::BackImageColorState(QString State )
{
    if(State=="Open"){
        SetImageFile_name = QString("QWidget#window{background-image: url(%1);}").arg(GetImageFile_name);
        window.setStyleSheet(SetImageFile_name);
    }
    else if(State=="Close"){
        window.setStyleSheet("QWidget#window{}");
        window.setStyleSheet(BackColor);
    }
}


KeyButton *Keyboard::createButton(QList<KeyButton::Mode> modes)
{
    KeyButton *button = new KeyButton(modes, this);
    button->onReponse(this, SLOT(onButtonPressed(const int&, const QString&)));  // 信号与槽
    button->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    bt.append(button);
    return button;
}

QWidget *Keyboard::chineseBar()  // 中文输入法框
{
    m_chineseWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    return m_chineseWidget;
}

QHBoxLayout *Keyboard::h1()
{
    QHBoxLayout *h = new QHBoxLayout;
    h->addSpacing(20);
    h->setSizeConstraint(QLayout::SetNoConstraint);
    for (int i = 0; i < modeListBar1.count(); i++) {
        KeyButton *button = createButton(modeListBar1.at(i));
        h->addWidget(button, 100);  // 向h1行中添加按钮控件
    }
    h->addSpacing(20);
    return h;
}

QHBoxLayout *Keyboard::h2()
{
    QHBoxLayout *h = new QHBoxLayout;
    h->addSpacing(45);
    for (int i = 0; i < modeListBar2.count(); i++) {
        KeyButton *button = createButton(modeListBar2.at(i));
        h->addWidget(button);
    }
    h->addSpacing(45);  // 设置距边缘的水平距离
    return h;
}

QHBoxLayout *Keyboard::h3()
{
    QHBoxLayout *h = new QHBoxLayout;
    h->setSpacing(0);
    for (int i = 0; i < modeListBar3.count(); i++) {
        KeyButton *button = createButton(modeListBar3.at(i));
        if (i == 0 || i == (modeListBar3.count() - 1))  // 若为第一个或者最后一个按键，改变其大小
            h->addWidget(button, 80);  // 第二个参数为扩展度
        else
            h->addWidget(button, 69);
    }
    return h;
}

QHBoxLayout *Keyboard::h4()
{
    QHBoxLayout *h = new QHBoxLayout;

    SetingButton();
    h->setSpacing(0);
    h->addWidget(Seting,12);
    for (int i = 0; i < modeListBar4.count(); i++) {
        KeyButton *button = createButton(modeListBar4.at(i));
        if (i == 0)
            h->addWidget(button, 12);
        if (i == 1)
            h->addWidget(button, 10);
        if (i == 2)
            h->addWidget(button, 56);
        if (i == 3)
            h->addWidget(button, 22);
    }
    return h;
}

QWidget *Keyboard::candidateList()
{
    return m_chineseWidget;
}

void Keyboard::resizeButton()
{
    foreach (KeyButton *button, findChildren<KeyButton *>()) {  // findChildren为模板函数，T为KeyButton *，其返回this所有子类的Qlist<T>
        int fixedWidth = width()*BUTTON_WIDTH_RATIO;
        int fixedHeight = height()*BUTTON_HEIGHT_RATIO;
        button->setIconSize(QSize(2*fixedWidth/3, 2*fixedHeight/3));  // 设置按钮上图标的大小

        // 匹配键值并设置相应的图案
        switch (button->mode().key) {
        case Qt::Key_Backspace:  // 回退按键
            button->setIcon(QIcon(BACKSPACE_ICON));
            connect(button, SIGNAL(pressed()), this, SLOT(backspace()), Qt::UniqueConnection);
            break;
        case Qt::Key_CapsLock:  // 大小写按键
            button->setIcon(QIcon(CAPLOCK_ICON));
            connect(button, SIGNAL(pressed()), this, SLOT(switchCapsLock()), Qt::UniqueConnection);  // 点击按钮触发槽函数
            break;
        case Qt::Key_Mode_switch:  // 特殊字符按键
            connect(button, SIGNAL(pressed()), this, SLOT(switchSpecialChar()), Qt::UniqueConnection);  // UniqueConnection避免重复链接
            break;
        case Qt::Key_Context1:  // 中英文按键
            connect(button, SIGNAL(pressed()), this, SLOT(switchEnOrCh()), Qt::UniqueConnection);
            break;
        case Qt::Key_Enter:  // 回车按键
            button->setIcon(QIcon(ENTER_ICON));
            connect(button, SIGNAL(pressed()), this, SLOT(enter()), Qt::UniqueConnection);
            break;
        case Qt::Key_Space:  // 空格按键
            connect(button, SIGNAL(pressed()), this, SLOT(space()), Qt::UniqueConnection);
            button->setIcon(QIcon(SPACE_ICON));
            break;
        default:
            break;
        }
    }
}

ChineseWidget::ChineseWidget(QWidget *parent) :  // 中文输入法框控件
    QListWidget(parent)
{
    // 将数据载入m_data中
    loadData();
    loadData2();

    setFocusPolicy(Qt::NoFocus);  // 不可通过TabFocus和ClickFocus获得焦点
    setViewMode(QListView::ListMode);  // 设置为列表显示模式(不能够拖动以及上下浮动)
    setFlow(QListView::LeftToRight);  // 从左往右排列
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 屏蔽水平滑动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 屏蔽垂直滑动条
    setHorizontalScrollMode(QListWidget::ScrollPerPixel);  // 设置为像素滚动

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))  // qt版本检查必须是5.0.0以上
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);  // 设置鼠标左键拖动
#endif

    // 设置输入法框样式
    setStyleSheet("                                                                           \
                  QListWidget { outline: none; border: 2px solid white;color: yellow; background-color: #003300;}    \
                  QListWidget::Item { width: 50px; height: 50px; }                            \
                  QListWidget::Item:hover { background: black; color: white; }              \
                  QListWidget::item:selected { background: black; color: black; }           \
                  QListWidget::item:selected:!active { background: #00000000; color: black; } \
                  ");

    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onItemClicked(QListWidgetItem *)));  // 当前键按下传递对应的中文
}

void ChineseWidget::setText(const QString &text)
{
    for (int i = 0; i < count(); i++) {
        QListWidgetItem *item = takeItem(i);  // 移除行i处的项目，并返回列表控件
        delete item;  // 删除掉项目控件
        item = nullptr;  // 并置为空
    }
    clear();  // 清除控件内容
    addOneItem(text);  // 添加到候选栏
    if (!m_data.contains(text.left(1))) {  // 若m_data数据集中不包含该文本的首字符，结束函数
        return;
    }

    // 通过获取首字母索引词库内容，用于加快匹配词(组)
    const QList<QPair<QString, QString> > &tmp = m_data[text.left(1)];
    for (int i = 0; i < tmp.count(); i++) {
        const QPair<QString, QString> &each = tmp.at(i);
        // 模糊匹配
        if (each.first.left(text.count()) != text)  // 使用首字母加快筛选
            continue;

        // 添加到候选栏(若首字母相同)
        addOneItem(each.second);  // 将词组添加到候选栏
    }
}

void ChineseWidget::SetInputBoxColor(QString Color)
{
    setStyleSheet(Color);
}

void ChineseWidget::onItemClicked(QListWidgetItem *item)
{
    emit pressedChanged(-1, item->text());  // 当前键按下传递对应的中文
    Keyboard().sendmessage(item->text());
    setText("");  // 设置输入法框的文本
}

void ChineseWidget::addOneItem(const QString &text)
{
    QListWidgetItem *item = new QListWidgetItem(text, this);  // 创建一个控件
    // 给输入法框中的控件(即每一个词组)设置样式
    QFont font;
    font.setPointSize(18);
    font.setBold(true);
    font.setWeight(50);
    item->setFont(font);  // 将文字样式应用到控件中

    // 设置文字居中
    item->setTextAlignment(Qt::AlignCenter);
    bool isChinese = QRegExp("^[\u4E00-\u9FA5]+").indexIn(text.left(1)) != -1;  // 匹配当前字符是否是中文

    // 根据是否是中文来设置控件宽度
    int width = font.pointSize();
    if (isChinese)
        width += text.count()*font.pointSize()*1.5;
    else
        width += text.count()*font.pointSize()*2/3;

    item->setSizeHint(QSize(width, 50));  // 最后设置控件大小
    addItem(item);  // 将item添加输入法列表控件中
}

void ChineseWidget::loadData()  // 提取拼音首字母以及匹配的中文
{
    QFile pinyin(":/ChineseLib/pinyin.txt");
    if (! pinyin.open(QIODevice::ReadOnly)) {  // 以只读方式打开文件
        qDebug() << "Open pinyin file failed!";  // 若打不开文件返回false，并输出一段提示文本
        return;
    }

    while (! pinyin.atEnd()) {  // 若文件未被读取完
        QString buf = QString::fromUtf8(pinyin.readLine()).trimmed();  // readLine返回文本的一行(包含换行符),trimmed函数去除string首尾的空字符
        QRegExp regExp("^[\u4E00-\u9FA5]+");  // 创建正则表达式对象

        int index = regExp.indexIn(buf);  // 搜索找到匹配的字符串，返回匹配的起始位置
        if (index == -1)  // 若未匹配，则continue继续下一行文本
            continue;

        // QString.right函数类似于substr, 其从该行字符串右边开始, 提取长度为第一个参数
        QString first = buf.right(buf.size() - regExp.matchedLength());  //   提取拼音首字符，matchedLengh函数返回最新匹配的字符的长度
        QString second = buf.mid(index, regExp.matchedLength());  // 抽取匹配的文本，mid函数等于substr的操作, index为匹配文本的索引

        QList<QPair<QString, QString> > &tmp = m_data[first.left(1)];  // 匹配m_data中的首字母映射的列表，是个引用
        tmp.append(qMakePair(first, second));  // 构造一个pair对象添加到m_data映射的列表中(拼音首字符，对应文本)
    }
}

void ChineseWidget::loadData2()
{
    /* 加载词组字库内容 */
    QFile pinyin(":/ChineseLib/pinyin_phrase.txt");
    if (! pinyin.open(QIODevice::ReadOnly)) {  // 打开文件错误处理
        qDebug() << "Open pinyin file failed!";
        return;
    }

    /* 按行读取内容 */
    while (! pinyin.atEnd()) {
        QString buf = QString::fromUtf8(pinyin.readLine()).trimmed();  // 读取一行并去除首位空字符
        if (buf.isEmpty())
            continue;

        /* 去除#号后的注释内容 */
        if (buf.left(1) == "#")  // QString.left从左边开始提取长度为1的字符
            continue;

        // 正则匹配词组内容并通过组捕获获取'词组'和'拼音'
        QRegExp regExp("(\\S+): ([\\S ]+)");  // cap(0)表示匹配的整个文本的值，\S匹配非空格字符，+匹配一个或多个字符
        int pos = 0;
        while ((pos = regExp.indexIn(buf, pos)) != -1) {  // 若匹配的起始位置不等于-1
            pos += regExp.matchedLength();  // 索引到匹配字符串的后一个字符
            QString second = regExp.cap(1);  // cap(1)为第一个圆括号的匹配值，即词组
            QString first = regExp.cap(2);  // cap(2)为第二个圆括号的匹配值，即拼音

            // QString.split()第一个参数sep出现的位置将字符串拆分为子字符串，并返回这些字符串的列表；若sep未匹配，则返回此字符串的单元素列表
            QStringList strList = first.split(" ");  // QStringList 继承于QList<QSting>
            QString abb;  // 首字母
            for (int i = 0; i < strList.count(); i++) {
                // 获得拼音词组的首字母(用于缩写匹配)
                abb += strList.at(i).left(1);  // at访问数组元素越界时发出异常
            }
            QList<QPair<QString, QString> > &tmp = m_data[first.left(1)];
            // 将'拼音缩写(首字母)'和'词组'写入匹配容器(m_data)
            tmp.append(qMakePair(abb, second));
            // 将'拼音(全拼)'和'词组'写入匹配容器
            tmp.append(qMakePair(first.remove(" "), second));  // remove()将字符删除，默认为大小写敏感(区分大小写)
        }
    }
}

void Keyboard::sendmessage(QString s){
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    QString originalText = clipboard->text();	     //获取剪贴板上文本信息
    qDebug() << originalText;
    clipboard->setText("");
    clipboard->setText(s);
    qDebug() << clipboard->text();

    keybd_event(17,0,0,0);
    keybd_event(86,0,0,0);
    keybd_event(86,0,KEYEVENTF_KEYUP, 0);
    keybd_event(17,0,KEYEVENTF_KEYUP, 0);
}


void Keyboard::CreateSystemTrayIcon()
{
    //初始化两个项目
    QAction* showAction = new QAction(QStringLiteral("显示"));//项1
    QAction* exitAction = new QAction(QStringLiteral("退出"));//项2
    //项1的点击槽函数
    connect(showAction, &QAction::triggered, &window, [=]()
    {
        window.show();
    });
    //项2的点击槽函数
    connect(exitAction , &QAction::triggered, &window, [=]()
    {
        qDebug()<<"exit";
        QApplication::exit(0);
    });
    //初始化菜单并添加项
    QMenu* trayMenu = new QMenu(&window);//菜单
    trayMenu->addAction(showAction);
    trayMenu->addAction(exitAction );
    //创建一个系统托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(&window);
    trayIcon->setIcon(QIcon(":/Image/windowIcon.jpg"));    //设置托盘图标
    trayIcon->setContextMenu(trayMenu);                                     //设置菜单
    trayIcon->show();

    hideBtn->setIcon(QIcon(":/Image/hide.png"));
    hideBtn->move(10, window.height() - hideBtn->height());
    closeBtn->setIcon(QIcon(":/Image/close.png"));
    closeBtn->move(900 - closeBtn->width() - 10, window.height() - closeBtn->height());
}
