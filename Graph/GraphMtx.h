//GraphMtx.h
//�ڽӾ���ʽ
//2017-2-15 12:00
#pragma once

#include<iostream>
#include<malloc.h>
using namespace std;

#define DEFAULT_VERLIST_SIZE 10

template<class T, class E>
class GraphMtx
{
public:
    GraphMtx(T x,int sz=DEFAULT_VERLIST_SIZE)
    {
        maxVertices = sz > DEFAULT_VERLIST_SIZE ? sz : DEFAULT_VERLIST_SIZE;
        VerticesList = new T[maxVertices];
        for(int i=0; i<maxVertices; ++i)     //������Ϊ#
        {
            VerticesList[i] = x;
        }
        Edge =new E*[maxVertices];
        for(int i=0; i<maxVertices; ++i)
        {
            Edge[i] = new E[maxVertices];
            for(int j=0; j<maxVertices; ++j)//�ߵ�������Ϊ0
            {
                Edge[i][j] = 0;
            }
        }
        numVertices = numEdges = 0;
    }
    ~GraphMtx()
    {
        delete VerticesList;
        VerticesList = NULL;
        for(int i=0; i<maxVertices; ++i)
        {
            delete Edge[i];
            Edge[i] = NULL;
        }
        delete Edge;
        Edge = NULL;
        numVertices = numEdges = 0;
    }
public:
    bool InsertVertex(const T vertex);      //���붥��vertex
    bool InsertEdge(const T vertex1, const T vertex2);//����һ����(vertex1,vertex2)
    int NumberOfVertex()const;              //��ȡ��������
    int NumberOfEdge()const;                //��ȡ����
    int GetFirstNeighbor(const T vertex)const;//��ȡ����vertex�ĵ�һ���ڽӶ����λ��
    int GetNextNeighbor(const T vertex1, const T vertex2)const;//��ȡ����vertex1��ĳ�ڽӶ���vertex2����һ���ڽӶ����λ��
    bool RemoveVertex(const T vertex);      //ɾ������vertex
    bool RemoveEdge(const T vertex1, const T vertex2);//ɾ����(vertex1, vertex2)
public:
    int GetPosVertex(T vertex)const
    {
        for(int i=0; i<numVertices; ++i)
        {
            if(VerticesList[i] == vertex)
                return i;
        }
        return -1;
    }
    void GraphShow()
    {
        cout<<"  ";
        for(int i=0; i<numVertices; ++i)
        {
            cout<<VerticesList[i]<<' ';
        }
        cout<<endl;
        for(int i=0; i<numVertices; ++i)
        {
            cout<<VerticesList[i]<<' ';
            for(int j=0; j<numVertices; ++j)
                cout<<Edge[i][j]<<' ';
            cout<<endl;
        }
    }
private:
    T *VerticesList;//���������
    E **Edge;       //�ߵĴ洢�ռ�
    int maxVertices;//��������
    int numVertices;//��ǰ�������
    int numEdges;   //��ǰ����
};
template<class T, class E>
bool GraphMtx<T,E>::InsertVertex(const T vertex)
{
    if(numVertices >= maxVertices)
        return false;
    VerticesList[numVertices++] = vertex;
    return true;
}
template<class T, class E>
bool GraphMtx<T,E>::InsertEdge(const T vertex1, const T vertex2)
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return false;
    Edge[v1][v2] = Edge[v2][v1] = 1;
    numEdges++;
}
template<class T, class E>
int GraphMtx<T,E>::NumberOfVertex()const
{return numVertices;}
template<class T, class E>
int GraphMtx<T,E>::NumberOfEdge()const
{return numEdges;}
template<class T, class E>
int GraphMtx<T,E>::GetFirstNeighbor(const T vertex)const
{
    int v = GetPosVertex(vertex);
    if(v == -1)
        return -1;
    for (int i = 0; i < numVertices; ++i)
    {
        if(Edge[v][i] == 1)
            return i;
    }
    return -1;
}
template<class T, class E>
int GraphMtx<T,E>::GetNextNeighbor(const T vertex1, const T vertex2)const
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return -1;
    for (int i = v2+1; i < numVertices; ++i)
    {
        if(Edge[v1][i] == 1)
            return i;
    }
    return -1;
}
template<class T, class E>
bool GraphMtx<T,E>::RemoveVertex(const T vertex)
{
    //���Ŀռ���������ú�һ��/�и���Ҫɾ������/�У��������ǰ�ƶ�;
    //ʱ�临�Ӷȵ͵������������һ��/�и���Ҫɾ���Ķ�����/��,��ı䶥���˳��
    //ѡ�ú�������
    int xnumedge = 0;
    int v = GetPosVertex(vertex);
    if(v == -1)
        return false;
    for (int i = v; i < numVertices; i++)
    {
        if(Edge[v][i] == 1)
            xnumedge++;
    }
    //���㸲��
    VerticesList[v] = VerticesList[numVertices-1];
    //�и���
    for (int i = v; i < numVertices; i++)
        Edge[v][i] = Edge[numVertices-1][i];
    //�и���
    for (int i = v; i < numVertices; i++)
        Edge[i][v] = Edge[i][numVertices-1];
    //������٣��߼���
    numVertices--;
    numEdges -= xnumedge;
}
template<class T, class E>
bool GraphMtx<T,E>::RemoveEdge(const T vertex1, const T vertex2)
{
    int v1 = GetPosVertex(vertex1);
    int v2 = GetPosVertex(vertex2);
    if(v1 == -1 || v2 == -1)
        return false;
    if(Edge[v1][v2] == 0)
        return false;
    Edge[v1][v2] = Edge[v2][v1] = 0;
    numEdges--;
}
