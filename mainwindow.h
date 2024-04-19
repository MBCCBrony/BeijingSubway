#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include"1.cpp"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //void mousePressEvent(QMouseEvent* event);

    //QList<QString> lessTransPath(const QString,const QString);
    //QList<QString> getSameLineABPath(const QString&,const QString&)const;


    //void paintEvent(QPaintEvent *event);
    //virtual void paintEvent(QPaintEvent *);

private slots:
    void on_short_2_clicked();

    void on_change_clicked();
/**/
private:
    Ui::MainWindow *ui;
    NS* ns;
    JD* jd;
    Graph* g;
    QGraphicsScene *scene;
    int*a;
    int p;
    int length;
    string n[300];

    map<string, int> getVNum;
    map<int, string> getVName;
//    map<int,Vertex> getVertex;
    map<string, string> mp;
//    map<string,Vertex> getTransferVertex;
    map<int,int> getX,getY;
};

#endif // MAINWINDOW_H
