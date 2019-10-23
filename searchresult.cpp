#include "searchresult.h"
#include "ui_searchresult.h"
#include <mainwindow.h>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

searchresult::searchresult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchresult)
{
    ui->setupUi(this);
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



void searchresult::on_bangumiName_clicked()
{
    QDesktopServices::openUrl(QUrl(Qurl));
}


void searchresult::set_url(QString str)
{
    this->Qurl = str;
}

searchresult::~searchresult()
{
    delete ui;
}

