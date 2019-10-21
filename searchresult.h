#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QWidget>

namespace Ui {
class searchresult;
}

class searchresult : public QWidget
{
    Q_OBJECT

public:
    explicit searchresult(QWidget *parent = nullptr);
    ~searchresult();

private:
    Ui::searchresult *ui;
};

#endif // SEARCHRESULT_H
