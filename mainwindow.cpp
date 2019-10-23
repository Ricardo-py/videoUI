#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchresult.h"
#include <QCoreApplication>
#include <Python.h>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QMovie>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置拉伸边框改变大小的参数
    mWp = unKnown; //设置拉伸变宽初始标志为 unKnow
    m_bPressed = false;  // 设置鼠标按下的状态
    mouseEvent = nullptr;  //设置鼠标事件的初始值

    init();
    display_data();
    //setSearchResult();
    setWindowFlag(Qt::FramelessWindowHint);
    //设置过滤器，8个Label作为边框，过滤的鼠标事件
    ui->lefttopLabel->installEventFilter(this);
    ui->topLabel->installEventFilter(this);
    ui->righttopLabel->installEventFilter(this);
    ui->leftLabel->installEventFilter(this);
    ui->rightLabel->installEventFilter(this);
    ui->leftbottomLabel->installEventFilter(this);
    ui->bottomLabel->installEventFilter(this);
    ui->rightbottomLabel->installEventFilter(this);


    // widget_2 的信号处理

    connect(ui->widget_2,SIGNAL(quitEvent()),this,SLOT(quitEvent()));
    connect(ui->widget_2,SIGNAL(mid_maxEvent()),this,SLOT(mid_maxEvent()));
    connect(ui->widget_2,SIGNAL(minEvent()),this,SLOT(minEvent()));
    connect(ui->widget_2,QOverload<QString>::of(&titlewid::searchEvent),
            [=](QString str)
            {
                qDebug() << "lineEdit的内容为:" << str;
                this->download_data(str);
            });

    connect(this,QOverload<>::of(&MainWindow::finish_download),
            [=]()
            {
                qDebug() << "开始展示";
                this->display_data();
            });

    connect(ui->widget_2,SIGNAL(moveWindow(QPoint&)),this,SLOT(moveWindowEvent(QPoint&)));
}

// 退出按钮处理事件
void MainWindow::quitEvent()
{
    qApp->quit();
}
// 最大化最小化窗口按钮事件
void MainWindow::mid_maxEvent()
{
    if(this->isMaximized())
    {
          showNormal();
         ui->widget_2->setMaxIcon(false);
    }
    else {
        showMaximized();
        ui->widget_2->setMaxIcon(true);
    }
}
// 最小化窗口事件
void MainWindow::minEvent()
{
     showMinimized();
}

// 拖动窗口标题，移动窗口
void MainWindow::moveWindowEvent(QPoint& pos)
{
    if(this->isMaximized()) return;
    QPoint tmp = this->pos() + pos;
    this->move(tmp);
}

// 过滤 8 个 Label 边框的点击、移动、改变窗口大小
 bool MainWindow::eventFilter(QObject *target, QEvent *event)
 {
    // 如果过滤事件是鼠标按下，设置改变窗口大小的标志为鼠标按下状态。
     if(event->type() == QEvent::MouseButtonPress)
     {
         //检测是哪条边框按下
         mWp = testLabelBorder(target->objectName());
        if(mWp == LabelBorderStatus::unKnown)
        {
           m_bPressed = false;
        }
        else {
             m_bPressed = true;
             mouseEvent = nullptr;
             mouseEvent = static_cast<QMouseEvent*>(event);
             if(mouseEvent )
             {
                 // 获取鼠标按下的定位。
                 pLast = mouseEvent->globalPos();
                 event->ignore();
             }
        }
     }
     // 如果是鼠标释放事件，设置拖动标志位初始状态。
     else if(event->type() == QEvent::MouseButtonRelease)
     {
         m_bPressed = false;
         mWp = LabelBorderStatus::unKnown;
     }
     // 如果过滤事件是鼠标移动事件，处理是否拖动边框改变窗口大小
     else if(event->type() == QEvent::MouseMove)
     {
         //检测鼠标是否按下，并且拖动的边框类型
         if(m_bPressed && mWp != LabelBorderStatus::unKnown)
          {
             mouseEvent = nullptr;
             mouseEvent = static_cast<QMouseEvent*>(event);
             if(mouseEvent)
             {
                 //获取鼠标拖动的位置，
                 QPoint ptemp = mouseEvent->globalPos();
                 // 计算鼠标按下并拖动的的位移
                 ptemp = ptemp - pLast ;
                 //保存当前鼠标拖动的位置，用于下一次计算拖动位移
                 pLast = mouseEvent->globalPos();
                 event->ignore();
                 // 拉伸窗口函数
                 moveAndResizeWindow(ptemp);
             }
         }
     }
     return QMainWindow::eventFilter(target,event);
 }

// 移动并改变窗口大小
 // 移动并改变窗口大小
void MainWindow::moveAndResizeWindow(QPoint& pos)
{
  if(this->isMaximized()) return;
  int x=0,y=0,w=0,h=0;
 // 根据拖动的那一条边框，确定拉伸还是缩小窗口。
  switch (mWp) {
  // 左边框被拉伸
  case LabelBorderStatus::toLeft:
      x = this->pos().x() + pos.x();
      y = this->pos().y();
      w = this->size().width() - pos.x();
      qDebug() << "size:" << this->size().width();
      h = this->size().height();
      qDebug() << "x:" << x <<" y:" << y << " w:" << w << " h:" << h;
     break;
   // 右边框被拉伸
  case LabelBorderStatus::toRight:
      x = this->pos().x();
      y = this->pos().y();
      w = this->size().width() + pos.x();

      h = this->size().height();

      break;
      // 上边框被拉伸
  case LabelBorderStatus::toTop:
      x = this->pos().x();
      y = this->pos().y() + pos.y();
      w = this->size().width() ;
      h = this->size().height() - pos.y();
      break;
      // 下边框被拉伸
  case LabelBorderStatus::toBottom:
      x = this->pos().x();
      y = this->pos().y();
      w = this->size().width() ;
      h = this->size().height() + pos.y();
      break;
      //右上角被拉伸
  case LabelBorderStatus::toRightTop:
      x = this->pos().x();
      y = this->pos().y() + pos.y();
      w = this->size().width() + pos.x() ;
      h = this->size().height() - pos.y();
      break;
      //左上角被拉伸
  case LabelBorderStatus::toLeftTop:
      x = this->pos().x() + pos.x();
      y = this->pos().y() + pos.y();
      w = this->size().width() - pos.x() ;
      h = this->size().height() - pos.y();
      break;
      // 右下角被拉伸
  case LabelBorderStatus::toRightBottom:
      x = this->pos().x();
      y = this->pos().y();
      w = this->size().width() + pos.x() ;
      h = this->size().height() + pos.y();
      break;
      // 左下角被拉伸
  case LabelBorderStatus::toLeftBottom:
      x = this->pos().x() + pos.x();
      y = this->pos().y();
      w = this->size().width() - pos.x() ;
      h = this->size().height() + pos.y();
      break;
  default:
      return;
  }
 // 改变窗口的大小和位置。
  if (w >= this->minimumWidth() && h >= this->minimumHeight())
    this->setGeometry(x,y,w,h);
}

// 根据边框Label的objectName， 确定拉伸的是哪个边框
 MainWindow::LabelBorderStatus MainWindow::testLabelBorder(QString _objectName)
 {
     if (_objectName == "lefttopLabel") return LabelBorderStatus::toLeftTop;
     if (_objectName == "topLabel") return LabelBorderStatus::toTop;
     if (_objectName == "righttopLabel") return LabelBorderStatus::toRightTop;
     if (_objectName == "leftLabel") return LabelBorderStatus::toLeft;
     if (_objectName == "rightLabel") return LabelBorderStatus::toRight;
     if (_objectName == "leftbottomLabel") return LabelBorderStatus::toLeftBottom;
     if (_objectName == "bottomLabel") return LabelBorderStatus::toBottom;
     if (_objectName == "rightbottomLabel") return LabelBorderStatus::toRightBottom;
     return LabelBorderStatus::unKnown;
 }

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPalette palette = this->palette();

   // qDebug() << "palette color: " << palette.color(QPalette::Window);
    QColor color(48,48,56);

    palette.setBrush(QPalette::Window,QBrush(color));
    this->setPalette(palette);

    QPainter painter(this);

    QRect rect = this->rect();
    QPen pen;
    pen.setWidth(1);

    pen.setColor(color);

    painter.setPen(pen);
    painter.drawRect(rect);

}

void MainWindow::init()
{
    QWidget *mainWidget = new QWidget();
    QWidget *leftWidget = new QWidget();
    leftWidget->setMinimumSize(50,0);
    QWidget *rightWidget = new QWidget();
    rightWidget->setMinimumSize(50,0);

    V = new QVBoxLayout(mainWidget);
    H = new QHBoxLayout(ui->scrollAreaWidgetContents);
    //ui->scrollAreaWidgetContents->setStyle();
   // this->setStyleSheet("background-color:rgb(90,90,90)");
    H->addWidget(leftWidget);
    H->addWidget(mainWidget);
    H->addWidget(rightWidget);
}

void MainWindow::setSearchResult()
{

    searchresult *wt[10];

    for (int i = 0; i < 10; i ++)
    {
        wt[i] = new searchresult();
        wt[i] = new searchresult();
        V->addWidget(wt[i]);
    }
}

void MainWindow::display_data()
{
    //删除原来的所有控件
    delete_layout_widgets(V);
    searchresult *wt[300];
    int i = 0;
    QFile file("./xmlfiles/informations.xml");
    if (!file.open(QFile::ReadOnly))
    {
        wt[i] = new searchresult();
        qDebug() << "找不到xml文件";
        QMovie *movie = new QMovie(":/images/404.gif");
        movie->start();
        wt[i]->set_movie(movie);
        wt[i]->set_title( QString::fromStdString("404未找到相关内容!"));
        V->addWidget(wt[i]);
        //添加一个容器，使得结果更加美观
        QWidget *ww = new QWidget();
        V->addWidget(ww);
        return;
    }

    QDomDocument doc;
    //设置test.xml到文档
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    //返回根节点
    QDomElement root=doc.documentElement();
    //获得第一个子节点
    QDomNode node=root.firstChild();

    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            //转换为元素
            QDomElement e=node.toElement();
            QDomNodeList list=e.childNodes();
            QString title = list.at(0).toElement().text();
            QString sub_title = list.at(1).toElement().text();
            QString type = list.at(2).toElement().text();
            QString url = list.at(3).toElement().text();
            QString image_path = "./title_images/" + list.at(4).toElement().text();

            wt[i] = new searchresult();
            QString brief_introduction = list.at(5).toElement().text();
            wt[i]->set_title(title);
            emit change_title(title);
            wt[i]->set_sub_title(sub_title);
            emit change_subtitle(sub_title);
            wt[i]->set_Icon(image_path);
            qDebug() << image_path;
            wt[i]->set_brief_introduction(brief_introduction);
            emit change_brief(brief_introduction);

            wt[i]->set_type(type);
            wt[i]->set_url(url);
            emit change_type(type);

            emit change_imgpath(image_path);
            V->addWidget(wt[i]);

            i = i + 1;
        }
        node=node.nextSibling();
    }
    //添加一个容器，使得结果更加美观
    QWidget *ww = new QWidget();
    V->addWidget(ww);
}


void MainWindow::download_data(QString str)
{
    if (str == "")
    {
        qDebug() << "请输入要搜索的内容";
        return;
    }
    std::string strr = str.toStdString();

    const char* ch = strr.c_str();
    //cout << ch << endl;
    Py_Initialize();
     if ( !Py_IsInitialized() )
     {
         qDebug() << "初始化错误";
         return;
     }
     PyObject* pModule = PyImport_ImportModule("py_spider_yinghua");  // 这里的test_py就是创建的python文件
     if (!pModule) {
             qDebug() << "Cant open python file!\n" << endl;
             return;
         }

    PyObject* pFunhello= PyObject_GetAttrString(pModule,"spider");  // 这里的spider就是python文件定义的函数
    PyObject* args = Py_BuildValue("(s)",ch);
     if(!pFunhello){
         qDebug()<<"Get function hello failed"<<endl;
         return;
     }
     qDebug() << "开始下载数据";
     PyObject_CallObject(pFunhello,args);
     qDebug() << "数据下载完毕";
     emit finish_download();
     //Py_Finalize();
}


void MainWindow::delete_layout_widgets(QBoxLayout *layout)
{
    QLayoutItem *child;
     while ((child = layout->takeAt(0)) != 0)
     {
            //setParent为NULL，防止删除之后界面不消失
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }

            delete child;
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}
