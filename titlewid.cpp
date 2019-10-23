#include "titlewid.h"
#include "ui_titlewid.h"
#include <QMouseEvent>


titlewid::titlewid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::titlewid)
{
    ui->setupUi(this);
}

titlewid::~titlewid()
{
    delete ui;
}

void titlewid::on_pb_close_clicked()
{
    emit quitEvent();
}

void titlewid::on_pb_mid_max_clicked()
{
    emit mid_maxEvent();
}

void titlewid::on_pb_min_clicked()
{
     emit minEvent();
}

void titlewid::setMaxIcon(bool arg)
{
    QIcon icon;
    if(arg)
    {
        icon.addFile(":/images/mid.png");
    }
    else {
       icon.addFile(":/images/max.png");
    }
    ui->pb_mid_max->setIcon(icon);
}

void titlewid::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    if(Qt::LeftButton == event->button())
    {
        QPoint pos = event->pos();
       if(pos.x() > ui->icon->pos().x() && pos.x() < ui->tools->pos().x())
        {
            if(pos.y() > ui->title->pos().y() && pos.y() < ui->title->pos().y() + ui->title->rect().height())
            {
                pLast=event->globalPos();
               // event->ignore();
                m_bPressed = true;
            }
        }
    }
}

void titlewid::mouseMoveEvent(QMouseEvent* event)
{
    if( m_bPressed)
    {
        QPoint ptemp = event->globalPos();
        ptemp = ptemp - pLast ;
        pLast = event->globalPos();
        emit moveWindow(ptemp);
    }
}

 void titlewid::mouseReleaseEvent(QMouseEvent * event)
 {
    m_bPressed = false;
 }

 void titlewid::mouseDoubleClickEvent(QMouseEvent *event)
 {
     if(Qt::LeftButton == event->button())
     {
         QPoint pos = event->pos();
        if(pos.x() > ui->icon->pos().x() && pos.x() < ui->widget->pos().x())
         {
             if(pos.y() > ui->title->pos().y() && pos.y() < ui->title->pos().y() + ui->title->rect().height())
             {
                  emit mid_maxEvent();
             }
         }
     }
 }

void titlewid::on_search_clicked()
{
    QString str = ui->lineEdit->text();
    emit searchEvent(str);
}
