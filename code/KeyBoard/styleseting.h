#ifndef STYLESETING_H
#define STYLESETING_H

#include <QWidget>

namespace Ui {
class StyleSeting;
}

class StyleSeting : public QWidget
{
    Q_OBJECT

public:
    explicit StyleSeting(QWidget *parent = nullptr);
    ~StyleSeting();
signals:
    void QuitSignal();
    void SetBackColor(QString);
    void SetBTColor(QString);
    void SetBackImage();
    void SetBackImageState(QString);
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::StyleSeting *ui;
};

#endif // STYLESETING_H
