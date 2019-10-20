#ifndef TITLEWID_H
#define TITLEWID_H

#include <QWidget>

namespace Ui {
class titlewid;
}

class titlewid : public QWidget
{
    Q_OBJECT

public:
    explicit titlewid(QWidget *parent = nullptr);
    ~titlewid();

    void setMaxIcon(bool arg);
    void setWindowTitle(QString _title);

    bool m_bPressed;
    QPoint pLast;

private slots:
    void on_pb_close_clicked();
    void on_pb_mid_max_clicked();
    void on_pb_min_clicked();

signals:
    void quitEvent();
    void mid_maxEvent();
    void minEvent();
    void moveWindow(QPoint& pos);


private:
    Ui::titlewid *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void  mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // TITLEWID_H
