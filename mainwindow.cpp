#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<iostream>
#include <QPainter>
#include<QMouseEvent>
#include<QPaintEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsEllipseItem>
#include<QString>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);//选择列表形式展示
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // 读取景点数据
    jd=new JD[10];
    string jdFile = "D:/subway/Qt/subway/subway/JD.txt";
    ifstream jdInput(jdFile);
    if (jdInput.is_open()) {
        string line;
        int i = 0;
        while (getline(jdInput, line)) {
            stringstream ss(line);
            int v;
            string JD;
            ss >> JD >> v;
            jd[i] = { v,JD };
            i++;
        }
            jdInput.close();
    }
    else {
        cout << "无法打开景点文件 " << jdFile << endl;
    }

    ns=new NS[220];
    a=new int[300];//输出
    string EdgeFile = "D:/subway/Qt/subway/subway/subwaymsg.txt";
    ifstream EdgeInput(EdgeFile);
    if (EdgeInput.is_open()) {
        string line;
        int i = 0;
        while (getline(EdgeInput, line)) {
            stringstream ss(line);
            int Num,x,y;
            string sta;
            ss >> Num >> sta>>x>>y;
            ns[i] = { Num,sta,x,y };
            i++;
        }
        EdgeInput.close();
    }
    else {
        cout << "无法打开边文件 " << EdgeFile << endl;
    }

    g = new Graph(300);
    string edgeFile = "D:/subway/Qt/subway/subway/subwayline.txt";
    ifstream edgeInput(edgeFile);
    if (edgeInput.is_open()) {
        string line;
        while (getline(edgeInput, line)) {
            stringstream ss(line);
            int fromV, toV, weight, Line;
            ss >> fromV >> toV >> weight >> Line;
            g->setEdge(fromV,toV,weight,Line);
        }
        edgeInput.close();
    }
    else {
        cout << "无法打开图文件 " << edgeFile << endl;
    }   
    QWidget transparentLineDrawingWidget;
    transparentLineDrawingWidget.setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout layout(ui->scrollArea);
    layout.addWidget(&transparentLineDrawingWidget);


    scene=new QGraphicsScene(-300,-200,600,200); //创建QGraphicsScene
    ui->View->setScene(scene);              //与view关联
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_short_2_clicked()
{
    ui->listWidget->clear();
    QString s = ui->sstation->text();
    QString e = ui->endstation->text();
    int start=0,end=0;//狗熊岭
    for(int i=0;i<220;i++){
        if(s==QString::fromStdString(ns[i].station)){
            start=ns[i].num;
            break;
        }
    }
    for(int i=0;i<9;i++){
        if(s==QString::fromStdString(jd[i].jdm)){
            start=jd[i].num;
            break;
        }
    }
    for(int i=0;i<220;i++){
        if(e==QString::fromStdString(ns[i].station)){
            end=ns[i].num;
            break;
        }
    }
    for(int i=0;i<9;i++){
        if(e==QString::fromStdString(jd[i].jdm)){
            end=jd[i].num;
            break;
        }
    }
//qDebug()<<"s:"<<start<<"e:"<<end;
    Dist* d = new Dist[300];
    a=g->DijkstraShortest(*g, start, d, end, length);

    update();//draw
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 10));

    int j;
    scene->clear();
    for(j=0;j<length-1;j++){

        double x1=ns[a[j]-1].x-880,y1=ns[a[j]-1].y-550,x2=ns[a[j+1]-1].x-880,y2=ns[a[j+1]-1].y-550;
        QGraphicsLineItem   *item = new QGraphicsLineItem(x1,y1,x2,y2);
//        qDebug()<<"x,y12:"<<x1<<y1<<x2<<y2;
        QPen pen;
        pen.setWidth(5); // 设置粗度为
        pen.setColor(Qt::red); // 设置颜色为红色
        item->setPen(pen); // 将画笔应用到item

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
        QApplication::processEvents();
    }

    int i;
    QString text="路线：";
    for (i = 0; i < 300; i++) {
        if (a[i+1] == -1)
            break;
        else{
            n[i]=ns[a[i]-1].station;
        }
        text += QString::fromStdString(n[i])+"->";
        if ((i + 1) % 3 == 0) {
            text += "\n";
        }
    }

    n[i]=ns[a[i]-1].station;
    text+=QString::fromStdString(n[i]);
    ui->listWidget->addItem(text); // 将QString类型的变量添加到QListWidget中
    ui->listWidget->show();
//    qDebug()<<text<<endl;

    g->reMark();/**/

    p = g->DijkstraPrice(*g, start, d, end, length);

    QString text1="票价：";
    text1+=QString::number(p);
    ui->listWidget->addItem(text1);
    ui->listWidget->show();

//    qDebug()<<p;

    g->reMark();

}

void MainWindow::on_change_clicked()
{
    ui->listWidget->clear();
    QString s = ui->sstation->text();
    QString e = ui->endstation->text();
    int start=0,end=0;
    for(int i=0;i<220;i++){
        if(s==QString::fromStdString(ns[i].station)){
            start=ns[i].num;
            break;
        }
    }
    for(int i=0;i<9;i++){
        if(s==QString::fromStdString(jd[i].jdm)){
            start=jd[i].num;
            break;
        }
    }
    for(int i=0;i<220;i++){
        if(e==QString::fromStdString(ns[i].station)){
            end=ns[i].num;
            break;
        }
    }
    for(int i=0;i<9;i++){
        if(e==QString::fromStdString(jd[i].jdm)){
            end=jd[i].num;
            break;
        }
    }

    Dist* d = new Dist[300];
    int* minPath = new int[300];
    minPath=g->DijkstraGetLeast(*g,start,d,end);
    update();//draw
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 10));
    scene->clear();
    int j;
    for (j = 0; j <300; j++) {

        if (minPath[j+1] == -1){

//            qDebug()<<"break";
            break;
        }

            double x1=ns[minPath[j]-1].x-880,y1=ns[minPath[j]-1].y-550,x2=ns[minPath[j+1]-1].x-880,y2=ns[minPath[j+1]-1].y-550;
            QGraphicsLineItem   *item = new QGraphicsLineItem(x1,y1,x2,y2);
            qDebug()<<"x,y12:"<<x1<<y1<<x2<<y2;
            QPen pen;
            pen.setWidth(5); // 设置粗度为
            pen.setColor(Qt::red); // 设置颜色为红色
            item->setPen(pen); // 将画笔应用到item
//            qDebug()<<"--"<<j;
            scene->addItem(item);
//            scene->clearSelection();
            item->setSelected(true);
//            qDebug()<<"center"<<scene->width()<<scene->height();
//            qDebug(<<"center mainWindow"<<width()<<height();
            QApplication::processEvents();
    }
//    qDebug()<<"ok";
    int i;
    QString text="路线：";

    for (i = 0; i <300; i++) {
//    for (i = 48; i >=0; i--) {
//        qDebug()<<minPath[i];
        if ((minPath[i+1] == -1)||(minPath[i+1] == 0))
//            continue;
          break;
        else{
            n[i]=ns[minPath[i]-1].station;
        }
//        qDebug()<<minPath[i]<<"->";

        text += QString::fromStdString(n[i])+"->";
        if ((i + 1) % 3 == 0) {
            text += "\n";
        }
    }
//    qDebug()<<"out_end";
//    qDebug()<<i;
    n[i]=ns[minPath[i]-1].station;
    text+=QString::fromStdString(n[i]);
    ui->listWidget->addItem(text); // 将QString类型的变量添加到QListWidget中
    ui->listWidget->show();
    qDebug()<<text<<endl;
/**/
    g->reMark();
    Dist* d1  = new Dist[300];
    p = g->DijkstraPrice(*g, start, d1, end, length);

    QString text1="票价：";
    text1+=QString::number(p);
    ui->listWidget->addItem(text1);
    ui->listWidget->show();

//    qDebug()<<p;

    g->reMark();

}
/*
void MainWindow::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        QPoint pos=event->pos();
        qDebug()<<pos.x()<<pos.y();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)   {
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 15));
    for(int i=0;i<length-2;i++){
        painter.drawLine(ns[a[i]].x, 0, height(), width());
    }
    //QDebug<<pos.height()<<width();
}*/


