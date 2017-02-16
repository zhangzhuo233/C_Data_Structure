/*GraphLink.h
**2016.2.16
*/
#pragma once

#include<iostream>
#include<malloc.h>
using namespace std;

#define DEFAULT_VERTEX_SIZE 10
template<class Type> class GraphLink;
template<class Type>
class Edge
{
    friend class GraphLink<Type>;
public:
    Edge(int num=0):dest(num),link(NULL)
    {}
    ~Edge()
    {}
private:
    int dest;
    Edge *link;
};
template<class Type>
class Vertex
{
    friend class GraphLink<Type>;
public:
    Vertex():data(),adj(NULL)
    {}
    ~Vertex()
    {}
private:
    Type data;
    Edge<Type> *adj;
};
template<class Type>
class GraphLink
{
public:
    GraphLink(int sz = DEFAULT_VERTEX_SIZE)
    {
        maxVertices = sz > DEFAULT_VERTEX_SIZE ? sz : DEFAULT_VERTEX_SIZE;
        numEdges = numVertices = 0;
        NodeTable = new Vertex<Type>[maxVertices];
    }
    ~GraphLink()
    {
        delete []NodeTable;
    }
public:
    bool InsertVertex(const Type &v);               //���붥��v
    bool InsertEdge(const Type &vertex1, const Type &vertex2);//����vertex1-->vertex2��
    int NumberOfVertice()const;                     //��ȡ��������
    int NumberOfEdge()const;                        //��ȡ������
    int GetFirstNeighbor(const Type &vertex)const;  //��ȡvertex�ĵ�һ���ڽӶ���
    int GetNextNeighbor(const Type &vertex1, const Type &vertex2)const;//��ȡvertex1���ڽӶ���vertex2����һ���ڽӶ���
    bool RemoveVertex(const Type &vertex);          //ɾ������vertex
    bool RemoveEdge(const Type &vertex1, const Type &vertex2);//ɾ��vertex1��vertex2���ɵı�
public:
    void ShowGraph()const
    {
        for (int i = 0; i < numVertices; ++i)
        {
            cout<<NodeTable[i].data<<"-->";
            Edge<Type> *e = NodeTable[i].adj;
            while(e != NULL)
            {
                cout<<e->dest<<"-->";
                e = e->link;
            }
            cout<<"Nul"<<endl;
        }
    }
private:
    int GetPosVertex(const Type v)const
    {
        for (int i = 0; i < numVertices; ++i)
        {
            if(NodeTable[i].data == v)
                return i;
        }
        return -1;
    }

    Vertex<Type> *NodeTable;//�����
    int maxVertices;
    int numVertices;
    int numEdges;
};
template<class Type>
bool GraphLink<Type>::InsertVertex(const Type &v)
{
    if(numVertices > maxVertices)
        return false;
    NodeTable[numVertices++].data = v;
    return true;
}
template<class Type>
bool GraphLink<Type>::InsertEdge(const Type &vertex1, const Type &vertex2)
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return false;
    //���õ������ͷ�巽ʽ
    //v1-->v2
    Edge<Type> *e = new Edge<Type>(v2);
    e->link = NodeTable[v1].adj;
    NodeTable[v1].adj = e;
    //v2-->v1
    e = new Edge<Type>(v1);
    e->link = NodeTable[v2].adj;
    NodeTable[v2].adj = e;
    numEdges++;
}
template<class Type>
int GraphLink<Type>::NumberOfVertice()const
{return numVertices;}
template<class Type>
int GraphLink<Type>::NumberOfEdge()const
{return numEdges;}
template<class Type>
int GraphLink<Type>::GetFirstNeighbor(const Type &vertex)const
{
    int v = GetPosVertex(vertex);
    if(v == -1)
        return -1;
    if(NodeTable[v].adj != NULL)
        return NodeTable[v].adj->dest;
    return -1;
}
template<class Type>
int GraphLink<Type>::GetNextNeighbor(const Type &vertex1, const Type &vertex2)const
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return -1;
    Edge<Type> *p = NodeTable[v1].adj;
    while(p != NULL && p->dest != v2)
        p = p->link;
    if(NULL == p)
        return -1;
    if(p->link != NULL)
        return p->link->dest;
    return -1;
}
template<class Type>
bool GraphLink<Type>::RemoveEdge(const Type &vertex1, const Type &vertex2)
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return false;
    //ɾ��v1-->v2
    Edge<Type> *p = NodeTable[v1].adj;
    Edge<Type> *q = NULL;
    while(p != NULL && p->dest != v2)
    {
        q = p;
        p = p->link;
    }
    if(NULL == p)
        return false;
    if(q == NULL)//˵��ɾ������ͷ���
        NodeTable[v1].adj = p->link;
    else
        q->link = p->link;
    free(p);
    p == NULL;
    //ɾ��v2-->v1
    p = NodeTable[v2].adj;
    q = NULL;
    while(p != NULL && p->dest != v1)
    {
        q = p;
        p = p->link;
    }
    if(NULL == p)
        return false;
    if(q == NULL)//˵��ɾ������ͷ���
        NodeTable[v2].adj = p->link;
    else
        q->link = p->link;
    free(p);
    p == NULL;
    numEdges--;
}
template<class Type>
bool GraphLink<Type>::RemoveVertex(const Type &vertex)
{
/*˼·��
**  1.�޳�����vertex�������������������еĹ���vertex�Ľ��(���Խ���RemoveEdage())
**  2.���һ�и�����ɾ����
        ��1�����㸲�Ƕ���
                  1��p�������һ�еĵ�����ɾ���еĶ���ָ�����һ�еı�����tmp����numVertices-1
                  2�����Ƕ���
                  3�������һ���������Ľ�㣬�����������������һ�������±���ĳ���ɾ�����±�
        (2)�������Ǳ�����ָ��tmp��
**  3.���ٶ�����
*/
    int v = GetPosVertex(vertex);
    if(v == -1)
        return false;
    //1.
    Edge<Type> *p = NodeTable[v].adj;
    while(p != NULL)
    {
        RemoveEdge(vertex, NodeTable[p->dest].data);
        p = NodeTable[v].adj;
    }
    //2.
        //1)
    p = NodeTable[numVertices-1].adj;
    NodeTable[v].adj = NodeTable[numVertices-1].adj;
    int tmp = numVertices-1;
        //2)
    NodeTable[v].data = NodeTable[numVertices-1].data;
        //3)
    Edge<Type> *q = NULL;
    while(p != NULL)
    {
        q = NodeTable[p->dest].adj;
        while(q != NULL && q->dest != tmp)
            q = q->link;
        q->dest = v;
        p = p->link;
    }
    //3.
    numVertices--;
}
