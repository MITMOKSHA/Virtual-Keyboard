#ifndef AEA_ABSTRACT_KEYBOARD_H
#define AEA_ABSTRACT_KEYBOARD_H

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

namespace AeaQt {

class AbstractKeyboard : public QWidget
{
    Q_OBJECT
public:
    AbstractKeyboard(QWidget *parent = 0) : QWidget(parent) {

    }
    ~AbstractKeyboard() { }

    const QString name() { return m_name; }
    void setName(const QString &name) { m_name = name; }

public slots:
    virtual void update(const QString &text) { Q_UNUSED(text); }

    void onKeyPressed(int key, QString value)
    {
        qDebug() << "key: " << key << "Value: " << value;
        QWidget *receiver = QApplication::focusWidget();  // 返回键盘事件输入的窗体，也就是当前鼠标点击的窗口
        if (!receiver)
            return;

        QKeyEvent keyPress(QEvent::KeyPress,     key, Qt::NoModifier, value);  // 模拟按下key键，创建QKeyEvent类的keyPress对象
        QKeyEvent keyRelease(QEvent::KeyRelease, key, Qt::NoModifier, value);  // 模拟释放key键

        QApplication::sendEvent(receiver, &keyPress);  // 将事件发送给接收者(即键盘事件输入的窗体QApplication)
        QApplication::sendEvent(receiver, &keyRelease);
    }

signals:
    void keyPressed(int key, QString value);

private:
    QString m_name;
};

}
#endif // AEA_ABSTRACT_KEYBOARD_H
