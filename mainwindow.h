#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>

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
    void setSearchResult();
    void moveAndResizeWindow(QPoint& pos);
    void init();
    void display_data();
    void delete_layout_widgets(QBoxLayout* layout);

signals:
    void finish_download();
    void change_title(QString str);
    void change_subtitle(QString str);
    void change_imgpath(QString str);
    void change_type(QString str);
    void change_brief(QString str);

private slots:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void moveWindowEvent(QPoint& pos);
    void download_data(QString s);


protected:
    void paintEvent(QPaintEvent* event);
private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *target, QEvent *event);
    QVBoxLayout *V;
    QHBoxLayout *H;
};

#endif // MAINWINDOW_H
