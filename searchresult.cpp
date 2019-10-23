#include "searchresult.h"
#include "ui_searchresult.h"
#include <mainwindow.h>
#include <QDebug>

searchresult::searchresult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchresult)
{
    ui->setupUi(this);

    /*connect(&m,QOverload<QString>::of(&MainWindow::change_title),
            [=](QString str)
            {
                this->set_title(str);
            });
    connect(&m,QOverload<QString>::of(&MainWindow::change_subtitle),
            [=](QString str)
            {
                this->set_sub_title(str);
            });

    connect(&m,QOverload<QString>::of(&MainWindow::change_type),
            [=](QString str)
            {
                this->set_type(str);
            });

    connect(&m,QOverload<QString>::of(&MainWindow::change_brief),
            [=](QString str)
            {
                this->set_brief_introduction(str);
            });

    connect(&m,QOverload<QString>::of(&MainWindow::change_imgpath),
            [=](QString str)
            {
                this->set_Icon(str);
            });*/
}

void searchresult::set_Icon(QString str)
{
    ui->imgIcon->setPixmap(str);

    ui->imgIcon->setScaledContents(true);
}

void searchresult::set_type(QString str)
{
    ui->type->setText(str);
}

void searchresult::set_movie(QMovie *movie)
{
    ui->imgIcon->setMovie(movie);
    ui->imgIcon->setScaledContents(true);
}

void searchresult::set_title(QString str)
{
    ui->bangumiName->setMinimumWidth(30 * str.length());
    ui->bangumiName->setText(str);
}

void searchresult::set_sub_title(QString str)
{
    ui->sub_title->setText(str);
}

void searchresult::set_brief_introduction(QString str)
{
    ui->brief->setText(str);
}


searchresult::~searchresult()
{
    delete ui;
}

