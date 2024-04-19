#include "mywidget.h"
#include <QPainter>
MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MyWidget::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::red,3,Qt::DashDotDotLine,Qt::RoundCap));
    painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    //绘制椭圆
    painter.drawEllipse(80,80,400,240);
    //绘制矩形
    painter.drawRect(550,80,300,240);
    //修改填充颜色
    painter.setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
    //绘制圆角矩形 5为圆角半径
    painter.drawRoundedRect(1000,80,300,240,20,20);

    //绘制点，修改画笔；
    painter.setPen(QPen(Qt::green,6,Qt::SolidLine,Qt::RoundCap));
    QPointF pointf[13];
    for (int i=0; i<13; ++i)
    {
        pointf[i].setX(100+i*30.0);
        pointf[i].setY(500.0);
    }
    painter.drawPoints(pointf, 13);

    //绘制线段
    QLineF pointL[2];
    pointL[0].setLine(550,450,500,600);
    pointL[1].setLine(700,450,800,600);
    painter.drawLines(pointL, 2);

    //绘制多边形
    QPolygonF polygon;
    polygon << QPointF(1000, 450) << QPointF(1100, 450) << QPointF(1100, 600) << QPointF(1300, 600);
    painter.drawPolygon(polygon, Qt::WindingFill);

    //绘制文字
    painter.setFont(QFont("Arial", 50));
    painter.drawText(80,800, "Hello,Qt!");

    //绘制图片
    QPixmap TestImage1;
    QPixmap TestImage2;
    TestImage1.load("C:/Users/YUANHAITAO/Pictures/RGB.jpg");
    TestImage2.load("C:/Users/YUANHAITAO/Pictures/Qt0.jpg");
    painter.drawPixmap(500,700,TestImage1);
    painter.drawPixmap(1000,700,TestImage2);
}
MyWidget::~MyWidget()
{
}


