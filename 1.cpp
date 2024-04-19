#include <iostream>
#include <stdio.h>
#include <QFile>
#include <string.h>
#include <fstream>
#include <sstream>
#include "mainwindow.h"
#include"Vertex.h"
#include <QTextCodec>
#include <QApplication>
using namespace std;
const int INF = std::numeric_limits<int>::max();

struct NS {
    int num;//站点编号
    string station;//站点名
    int x,y;//坐标
};
struct JD {
    int num;//站点编号
    string jdm;// 景点名
};
class Edge {
public:
    int from, to, weight, line;//起点，终点， 权值（距离），属于几号线
    Edge() {
        from = to = -1;
        weight = 0;
        line = 0;
    }
    Edge(int f, int t, int w, int l) {
        from = f;
        to = t;
        weight = w;
        line = l;
    }
};
class Dist {
public:
    int index;
    int length;
    int pre;
    int preline;
};
class Minheap {
private:
    Dist* heapArray;
    int CurrentSize;
    int MaxSize;
    void swap(int pos_x, int pos_y) {
        int temp;
        temp = pos_x;
        pos_x = pos_y;
        pos_y = temp;
    }
    void BuildHeap() {
        for (int i = CurrentSize / 2 - 1; i >= 0; i--)
            SiftDown(i);
    }
public:
    Minheap(int n) {
        if (n <= 0)
            return;
        CurrentSize = 0;
        MaxSize = n;
        heapArray = new Dist[MaxSize];
        BuildHeap();
    }
    bool isEmpty() {
        if (CurrentSize == 0)
            return true;
        else
            return false;
    }
    bool isLeaf(int pos)  {
        return (pos >= CurrentSize / 2) && (pos < CurrentSize);
    }
    int RightChild(int pos) {
        return 2 * pos + 2;
    }
    int LeftChild(int pos) {
        return 2 * pos + 1;
    }
    int Parent(int pos) {
        return (pos - 1) / 2;
    }
    bool Insert( Dist& newNode) {
        if (CurrentSize == MaxSize)
            return false;
        heapArray[CurrentSize] = newNode;
        SiftUp(CurrentSize);
        CurrentSize++;
        return true;
    }
    Dist& RemoveMin() {
        if (CurrentSize == 0) {
            cout << "Can't Delete" << endl;
            exit(1);
        }
        else {
            swap(0, --CurrentSize);
            if (CurrentSize > 1)
                SiftDown(0);
            return heapArray[CurrentSize];
        }
    }
    void SiftUp(int position) {
        int temppos = position;
        Dist temp = heapArray[temppos];
        while ((temppos > 0) && (heapArray[Parent(temppos)].length > temp.length)) {
            heapArray[temppos] = heapArray[Parent(temppos)];
            temppos = Parent(temppos);
        }
        heapArray[temppos] = temp;
    }
    void SiftDown(int left) {
        int i = left;
        int j = LeftChild(i);
        Dist temp = heapArray[i];
        while (j < CurrentSize) {
            if ((j < CurrentSize - 1) && (heapArray[j].length > heapArray[j + 1].length))
                j++;
            if (temp.length > heapArray[j].length) {
                heapArray[i] = heapArray[j];
                i = j;
                j = LeftChild(j);
            }
            else break;
        }
        heapArray[i] = temp;
    }
};
class Graph {
public:
    int** gra;//存地铁图
    int** Line;// 存线属于几号线
    int numVertex;
    int numEdge;
    int* Mark;
    int* Indegree;
    Graph(int numVert) {
        int i, j;
        gra = (int**)new int* [numVert];
        for (i = 0; i <= numVert; i++)
            gra[i] = new int[numVert];
        for (i = 0; i <= numVert; i++) {
            for (j = 0; j <= numVert; j++)
                gra[i][j] = 0;
        }
        Line = (int**)new int* [numVert];
        for (i = 0; i <= numVert; i++)
            Line[i] = new int[numVert];
        for (i = 0; i <= numVert; i++) {
            for (j = 0; j <= numVert; j++)
                Line[i][j] = 0;
        }
        numVertex = numVert;
        numEdge = 0;
        Indegree = new int[numVertex];
        Mark = new int[numVertex];
        for (int k = 0; k < numVertex; k++) {
            Mark[k] = 0;
            Indegree[k] = 0;
        }
    }
    ~Graph() {
        for (int i = 0; i < numVertex; i++)
            delete[]gra[i];
        delete[]gra;
    }
    Edge FirstEdge(int oneVertex) {
        Edge myEdge;
        myEdge.from = oneVertex;
        for (int i = 0; i < numVertex; i++) {
            if (gra[oneVertex][i] != 0) {
                myEdge.to = i;
                myEdge.weight = gra[oneVertex][i];
                break;
            }
        }
        return myEdge;
    }
    Edge NextEdge(Edge preEdge) {
        Edge myEdge;
        myEdge.from = preEdge.from;
        if (preEdge.to < numVertex) {
            for (int i = preEdge.to + 1; i < numVertex; i++) {
                if (gra[preEdge.from][i] != 0) {
                    myEdge.to = i;
                    myEdge.weight = gra[preEdge.from][i];
                    break;
                }
            }
        }
        return myEdge;
    }
    void setEdge(int from, int to, int weight, int line) {
        if (gra[from][to] <= 0) {
            numEdge++;
            Indegree[to]++;
        }
        gra[from][to] = weight;
        Line[from][to] = line;
        gra[to][from] = weight;
        Line[to][from] = line;
    }
    void delEdge(int from, int to) {
        if (gra[from][to] > 0) {
            numEdge--;
            Indegree[to]--;
        }
        gra[from][to] = 0;
    }

    void reMark() {
        for (int k = 0; k < numVertex; k++) {
            Mark[k] = 0;
        }
    }
    bool isEdge(Edge oneEdge) {
        if (oneEdge.weight > 0 && oneEdge.weight < 1e9 && oneEdge.to >= 0)
            return true;
        else
            return false;
    }
    int ToVertex(Edge oneEdge) {
        return oneEdge.to;
    }
    int Weight(Edge oneEdge) {
        return oneEdge.weight;
    }
    int getline(Edge oneEdge) {
        return oneEdge.line;
    }
    int VerticesNum() {
        return numVertex;
    }

    int* DijkstraShortest(Graph& G, int start, Dist*& D, int end, int& length) {
        int a[300]; // 用于存储最短路径上的顶点
        int* b = new int[300]; // 用于存储最短路径上的顶点的逆序数组
        int flag = 0; // 标记数组a和b的当前位置

        // 初始化D数组，用于存储每个顶点的距离信息
        for (int k1 = 0; k1 < 300; k1++)
            a[k1] = b[k1] = -1;
        for (int i = 0; i < G.VerticesNum(); i++) {
            G.Mark[i] = 0; // 标记顶点是否已被访问过
            D[i].index = i; // 存储顶点的索引
            D[i].length = 1e9; // 初始化距离为无穷大
            D[i].pre = start; // 存储前驱顶点
        }
        D[start].length = 0; // 起点到自己的距离为0

        // 创建一个最小堆H，用于存储待处理的顶点
        Minheap H(G.numEdge);
        H.Insert(D[start]);

        // 当堆不为空时，继续处理顶点
        while (1) {
            bool FOUND = false;
            Dist d;
            // 从堆中取出距离最小的顶点
            while (!H.isEmpty()) {
                d = H.RemoveMin();
                if (G.Mark[d.index] == 0) {
                    FOUND = true;
                    break;
                }
            }
            // 如果堆为空，说明所有顶点都已处理完毕，退出循环
            if (!FOUND)
                break;
            int v = d.index;
            G.Mark[v] = 1; // 标记顶点已被访问过

            // 遍历与顶点v相邻的所有边
            for (Edge e = G.FirstEdge(v); G.isEdge(e); e = G.NextEdge(e)) {
                // 如果通过顶点v到达相邻顶点的距离更短，更新距离信息，并将相邻顶点加入堆中
                if (D[G.ToVertex(e)].length > (D[v].length + G.Weight(e))) {
                    D[G.ToVertex(e)].length = D[v].length + G.Weight(e);
                    D[G.ToVertex(e)].pre = v;
                    H.Insert(D[G.ToVertex(e)]);
                    G.Mark[D[G.ToVertex(e)].index] = 0; // 标记相邻顶点未被访问过
                }
            }
        }

        // 将最短路径上的顶点存入数组a
        a[flag++] = end;
        int med = end;
        while (D[med].pre != start) {
            a[flag++] = D[med].pre;
            med = D[med].pre;
        }
        a[flag++] = start;
        int k2 = 0;
        length = 0;
        // 将数组a中的顶点逆序存入数组b，并计算数组长度
        for (int k1 = flag - 1; k1 >= 0; k1--) {
            b[k2] = a[k1];
            length++;
            k2++;
        }

        cout << "最短路径：";
        return b;
    }

    int* DijkstraGetLeast(Graph& G, int start, Dist*& D,int end) {
        int a[300]; // 定义一个长度为300的整型数组a
        int *b = new int[300]; // 动态分配一个长度为300的整型数组b
        int flag=0; // 初始化flag为0
        int w; // 定义一个整型变量w
        for(int k1=0;k1<300;k1++) // 遍历数组a和b，将它们的元素都初始化为-1
            a[k1]=b[k1]=-1;
        for (int i = 0; i < G.numVertex; i++) { // 遍历图G的所有顶点
            G.Mark[i] = 0; // 将图G的标记数组的第i个元素设置为0
            D[i].index = i; // 将距离数组D的第i个元素的index属性设置为i
            D[i].length = 1e9; // 将距离数组D的第i个元素的length属性设置为1e9
            D[i].pre = start; // 将距离数组D的第i个元素的pre属性设置为起始点start
            D[i].preline=-1; // 将距离数组D的第i个元素的preline属性设置为-1
        }
        D[start].length = 0; // 将距离数组D的起始点start的length属性设置为0
        Minheap H(G.numEdge); // 创建一个最小堆H，其容量为图G的边数
        H.Insert(D[start]); // 将距离数组D的起始点start插入到最小堆H中
        while(1) { // 循环执行以下操作，直到找到从起始点到终点的最短路径
            bool FOUND = false; // 初始化FOUND为false
            Dist d; // 定义一个距离对象d
            while (!H.isEmpty()) { // 当最小堆H不为空时，执行以下操作
                d = H.RemoveMin(); // 从最小堆H中移除最小的元素，并将其赋值给距离对象d
                if (G.Mark[d.index] == 0) { // 如果距离对象d的index属性对应的顶点在图G中未被访问过
                    FOUND = true; // 将FOUND设置为true
                    break; // 跳出循环
                }
            }
            if (!FOUND) // 如果FOUND为false，说明已经找到了从起始点到终点的最短路径，跳出循环
                break;
            int v = d.index; // 将距离对象d的index属性赋值给变量v
            G.Mark[v] = 1; // 将图G的标记数组的第v个元素设置为1，表示顶点v已被访问过
            for (Edge e = G.FirstEdge(v); G.isEdge(e); e = G.NextEdge(e)) { // 遍历与顶点v相连的所有边
                w=G.Weight(e); // 获取边e的权重，并赋值给变量w
                if(G.Line[e.from][e.to]!=D[v].preline) // 如果边e的线段编号不等于距离对象d的preline属性
                    w=w+100000; // 将w的值加100000
                if (D[G.ToVertex(e)].length > (D[v].length + w)) { // 如果从起始点到边e的目标顶点的距离大于从起始点到顶点v的距离加上边的权重w
                    D[G.ToVertex(e)].length = D[v].length + w; // 更新从起始点到边e的目标顶点的距离
                    D[G.ToVertex(e)].pre = v; // 更新距离对象d的pre属性为顶点v
                    D[G.ToVertex(e)].preline = G.Line[e.from][e.to]; // 更新距离对象d的preline属性为边e的线段编号
                    H.Insert(D[G.ToVertex(e)]); // 将距离对象d插入到最小堆H中
                    G.Mark[D[G.ToVertex(e)].index]=0; // 将图G的标记数组的第目标顶点的索引位置的元素设置为0，表示该顶点未被访问过
                }
            }
        }
        a[flag++]=end; // 将终点添加到数组a中
        int med=end; // 将终点赋值给变量med
        while(D[med].pre!=start){ // 当距离对象d的pre属性不等于起始点时，执行以下操作
            a[flag++]=D[med].pre; // 将距离对象d的pre属性添加到数组a中
            med=D[med].pre; // 将距离对象d的pre属性赋值给变量med
        }
        a[flag++]=start; // 将起始点添加到数组a中
        int k2 = 0; // 初始化变量k2为0
        for(int k1=flag-1;k1>=0;k1--){ // 从数组a的最后一个元素开始，向前遍历数组a
            b[k2] = a[k1]; // 将数组a中的元素复制到数组b中
            k2++; // 将变量k2的值加1
        }
        return b; // 返回数组b
    }

    int DijkstraPrice(Graph& G, int start, Dist*& D, int end, int& length) {
        int a[300]; // 用于存储最短路径上的顶点
        int* b = new int[300]; // 用于存储最短路径上的顶点的逆序数组
        int flag = 0; // 标记数组a和b的当前位置

        // 初始化D数组，用于存储每个顶点的距离信息
        for (int k1 = 0; k1 < 300; k1++)
            a[k1] = b[k1] = -1;
        for (int i = 0; i < G.VerticesNum(); i++) {
            G.Mark[i] = 0; // 标记顶点是否已被访问过
            D[i].index = i; // 存储顶点的索引
            D[i].length = 1e9; // 初始化距离为无穷大
            D[i].pre = start; // 存储前驱顶点
        }
        D[start].length = 0; // 起点到自己的距离为0

        // 创建一个最小堆H，用于存储待处理的顶点
        Minheap H(G.numEdge);
        H.Insert(D[start]);

        // 当堆不为空时，继续处理顶点
        while (1) {
            bool FOUND = false;
            Dist d;
            // 从堆中取出距离最小的顶点
            while (!H.isEmpty()) {
                d = H.RemoveMin();
                if (G.Mark[d.index] == 0) {
                    FOUND = true;
                    break;
                }
            }
            // 如果堆为空，说明所有顶点都已处理完毕，退出循环
            if (!FOUND)
                break;
            int v = d.index;
            G.Mark[v] = 1; // 标记顶点已被访问过

            // 遍历与顶点v相邻的所有边
            for (Edge e = G.FirstEdge(v); G.isEdge(e); e = G.NextEdge(e)) {
                // 如果通过顶点v到达相邻顶点的距离更短，更新距离信息，并将相邻顶点加入堆中
                if (D[G.ToVertex(e)].length > (D[v].length + G.Weight(e))) {
                    D[G.ToVertex(e)].length = D[v].length + G.Weight(e);
                    D[G.ToVertex(e)].pre = v;
                    H.Insert(D[G.ToVertex(e)]);
                    G.Mark[D[G.ToVertex(e)].index] = 0; // 标记相邻顶点未被访问过
                }
            }
        }

        // 将最短路径上的顶点存入数组a
        a[flag++] = end;
        int med = end;
        while (D[med].pre != start) {
            a[flag++] = D[med].pre;
            med = D[med].pre;
        }
        a[flag++] = start;
        int k2 = 0;
        length = 0;
        // 将数组a中的顶点逆序存入数组b，并计算数组长度
        for (int k1 = flag - 1; k1 >= 0; k1--) {
            b[k2] = a[k1];
            length++;
            k2++;
        }
        int weight = 0;
        for (int i = 0; i < length; i++) {
            weight = weight + gra[i][i + 1];
        }
        int p = 0;
        if (weight <= 6000) {
            p = 3;
        }
        else if (6000 < weight && weight <= 12000) {
            p = 4;
        }
        else if (12000 < weight && weight <= 22000) {
            p = 5;
        }
        else if (22000 < weight && weight <= 32000) {
            p = 6;
        }
        else if (32000 < weight && weight <= 52000){
            p = 7;
        }
        else if (52000 < weight && weight <= 72000) {
            p = 8;
        }
        else if (72000 < weight && weight <= 92000) {
            p = 9;
        }
        else {
            p = 10;
        }
        return p;
    }
};


