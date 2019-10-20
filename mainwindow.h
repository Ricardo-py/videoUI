#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum LabelBorderStatus {
       unKnown = 0,
       toTop = 1,
       toBottom = 2,
       toLeft = 3,
       toRight = 4,
       toLeftTop = 5,
       toRightTop = 6,
       toLeftBottom = 7,
       toRightBottom = 8
    };

    LabelBorderStatus mWp;
    QMouseEvent* mouseEvent;
    bool m_bPressed;
    QPoint pLast;

    LabelBorderStatus testLabelBorder(QString _objectName);
    void moveAndResizeWindow(QPoint& pos);

private slots:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void moveWindowEvent(QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event);
private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // MAINWINDOW_H
