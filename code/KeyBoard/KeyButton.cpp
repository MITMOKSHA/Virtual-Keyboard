#include "KeyButton.h"
#include <QDebug>

using namespace AeaQt;


KeyButton::KeyButton(const QList<KeyButton::Mode> modes, QWidget *parent) :
    QPushButton(parent)
{
    BtColor="AeaQt--KeyButton { background: #003300; border: 2px solid white; border-radius: 5px;" \
            "margin: 5px;" \
            "font-size: 25px; color: white;}"
            "AeaQt--KeyButton:hover{background:#C0C0C0}"
            "AeaQt--KeyButton:pressed { background:#808080}";


    this->setStyleSheet(BtColor);  // 设置按钮样式
    Q_ASSERT(!modes.isEmpty());  // 若modes不为空，断言
    this->setFocusPolicy(Qt::NoFocus);  // 不接受焦点


    foreach (Mode mode, modes) {  // modes结构体列表
        if (mode.type == Auto) {  // 若模式为自动(默认)，则使用find函数进行模式搜索，并返回相应键盘类型
            mode.type = find(mode.value);  // 赋值给模式的类型成员type
        }

        if (mode.display.isNull())
            mode.display = mode.value;  // 赋值给显示文本

        m_modes.append(mode);  // 处理之后将该模式加入到模式结构体列表成员m_modes中
    }

    if (!modes.isEmpty()) {  // 若modes不为空
        m_preMode = m_mode = m_modes.first();  // 将队列首结构体元素赋值给m_preMode以及m_mode
        setText(m_mode.display);  // 设置文本框显示文本，传入m_mode对象display成员
    }
    connect(this, SIGNAL(pressed()), this, SLOT(onPressed()));  // 按钮被按下发射信号，槽函数接收调用pressed()的重载函数
}

KeyButton::Type KeyButton::find(const QString &value)  // 匹配相应的字符，返回键盘枚举类型
{
    QRegExp rx("[a-z,.;'\\[\\]]");  // 正则表达式对象, 匹配小写字母a-z
    if (rx.exactMatch(value))  // 若匹配到,exactMatch()成员函数返回true
        return KeyButton::LowerCase;  // 返回小写模式


    rx = QRegExp("[A-Z，。；’【】]");
    if (rx.exactMatch(value))
        return KeyButton::UpperCase;  // 返回大写模式

    return KeyButton::SpecialChar;  // 返回特殊字符模式
}

KeyButton::Mode KeyButton::find(KeyButton::Type type)  // 返回模式对象
{
    foreach (KeyButton::Mode mode, m_modes) {  // 遍历模式对象列表
        if (mode.type == type)  // 若模式类别为传递过来的模式
            return mode;  // 返回当前模式对象
    }

    return m_modes.first();  // 若不匹配，返回模式列表中的第一个模式对象
}

KeyButton::Mode KeyButton::findNext()  // 找到对应按钮上的文本
{
    for(int i = 0; i < m_modes.count(); i++) {
        KeyButton::Mode mode = m_modes[i];  // 遍历m_modes列表中的模式
        if (mode.display == m_mode.display) {  // 若遍历的模式与当前模式相等
            if (i+1 < m_modes.count())
                return m_modes.at(i+1);
            else
                return m_modes.first();
        }
    }

    return m_modes.first();
}

void KeyButton::setText(const QString &text)
{
    // toStdString函数将Qstring转string, QString::data()一个存在于字符串中的字符char
    QPushButton::setText(QString::fromUtf8(text.toStdString().data()));  // fromUtf8获取字符串的UTF8编码值，该参数类型为const char*
}

KeyButton::Mode KeyButton::mode()  // 返回m_mode
{
    return m_mode;
}

void KeyButton::onReponse(const QObject *receiverObj, const QString &receiver)
{
    connect(this, SIGNAL(pressed(int,QString)), receiverObj, receiver.toStdString().c_str());  // 将receiver内容转换为std样式再转换为c样式
}

void KeyButton::switchCapsLock()
{
    if (m_mode.type == SpecialChar)  // 若模式的类别为符号键盘(枚举类型), 函数结束
        return;

    m_preMode = m_mode;  // 将当前模式类别存放在前一个模式m_preMode中
    m_mode = find(m_mode.type == LowerCase ? UpperCase : LowerCase);  // 传递一个模式类别，并返回模式对象(相反大小写)
    setText(m_mode.display);  // 设置转换后按键显示的文本
}

void KeyButton::switchSpecialChar()
{
    if (m_mode.type == SpecialChar) {  // 若为特殊字符类型，则将之前的模式传递给当前模式
        m_mode = m_preMode;
    }
    else {
        m_preMode = m_mode;  // 否则将当前模式存储到m_preMode中
        m_mode = find(SpecialChar);  //  查找特殊字符模式
    }
    setText(m_mode.display);  // 设置转换后按键显示的文本
}

void KeyButton::switching()
{
    m_mode = findNext();  // 切换按键上中/En显示
    setText(m_mode.display);  // 设置转换后按键显示的文本
}

void KeyButton::SetBtColor(QString color)
{
    this->setStyleSheet(color);
}

void KeyButton::onPressed()
{
    emit pressed(m_mode.key, m_mode.value);
}
