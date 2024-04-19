#ifndef VERTEX_H
#define VERTEX_H
#include<string>
using namespace std;
class Vertex {
public:
    int v;//点的编号
    int subwayNum;//在哪号线，需要用来算最短换乘
    string name;//叫啥
    int x,y;

    Vertex() {
        v = -1;
        subwayNum = 0;
        name = "";
    }
    Vertex(int v, string name,int x,int y, int subwayNum) {
        this->v = v;
        this->name = name;
        this->subwayNum = subwayNum;
        this->x=x;
        this->y=y;
    }
    Vertex(int v) {
        this->v = v;
        subwayNum = 0;
        name = "";
    }
};
#endif // VERTEX_H
