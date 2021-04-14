#ifndef AEA_KEY_BUTTON_H
#define AEA_KEY_BUTTON_H

#include <QPushButton>
namespace AeaQt {

class KeyButton : public QPushButton
{
    Q_OBJECT
public:
    enum Type { Auto = 0, LowerCase, UpperCase, SpecialChar };  // 小写键盘，大写键盘，符号键盘

    struct Mode {  // 模式结构体
        Mode() = default;  // 默认构造函数
        Mode(int _key, QString _value, QString _display = QString(), Type _type = Auto)
          : key(_key), value(_value), display(_display), type(_type) { }  // 构造函数初始化表
        int key;          /* Qt::Key */
        QString value;    // 文本
        QString display;  // 按键上显示文本
        Type type;        // 枚举类型默认为auto
    };

    KeyButton(const QList<Mode> modes = QList<Mode>(), QWidget *parent = nullptr);
    Mode mode();
    void onReponse(const QObject* receiverObj, const QString &receiver);
    void switchCapsLock();  // 大小写切换
    void switchSpecialChar();  // 特殊字符切换
    void switching(); // Cycle switch
    void sendmessage();
    void SetBtColor(QString color);
    QString BtColor;
signals:
    void pressed(int key, QString value);

private slots:
    void onPressed();


private:
    Type find(const QString &value);  // 寻找键盘的类型
    Mode find(Type type);  // 寻找类型
    Mode findNext();  // 找到对应按钮上的文本
    void setText(const QString &text);  // 设置按钮上的文本
private:
    Mode        m_preMode;  // 前一个模式
    Mode        m_mode;  // 当前模式
    QList<Mode> m_modes;  // 模式列表
};

}
#endif // AEA_KEY_BUTTON_H
