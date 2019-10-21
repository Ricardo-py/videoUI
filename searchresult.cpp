#include "searchresult.h"
#include "ui_searchresult.h"

searchresult::searchresult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchresult)
{
    ui->setupUi(this);
}

searchresult::~searchresult()
{
    delete ui;
}
