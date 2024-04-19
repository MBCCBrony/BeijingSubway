#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>
using namespace std;
#include <iostream>
#include <stdio.h>
#include <QFile>
#include <string.h>
#include <fstream>
#include <sstream>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("地铁乘车引导系统"));
    //w.resize(300,3000);
    w.show();
    return a.exec();


}

