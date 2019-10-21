#include "mainwindow.h"
#include <QApplication>
#include "searchresult.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //searchresult se;
    w.show();
   // se.show();

    return a.exec();
}
