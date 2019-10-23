#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QWidget>
#include <mainwindow.h>
#include <QMovie>

namespace Ui {
class searchresult;
}

class searchresult : public QWidget
{
    Q_OBJECT

public:
    explicit searchresult(QWidget *parent = nullptr);
    ~searchresult();
    void set_Icon(QString str);
    void set_title(QString str);
    void set_sub_title(QString str);
    void set_brief_introduction(QString str);
    void set_type(QString str);
    void set_movie(QMovie* movie);

private:
    Ui::searchresult *ui;
};

#endif // SEARCHRESULT_H
