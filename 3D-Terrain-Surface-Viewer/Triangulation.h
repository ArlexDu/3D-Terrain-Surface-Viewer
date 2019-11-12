#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include<set>
#include<vector>
#include<stack>
using namespace std;

//���и߳���Ϣ����ά�ռ��
struct Point3D
{
	float x, y, z;

	Point3D()
	{
		x = 0; y = 0; z = 0;
	};
	Point3D(float a, float b, float c)
	{
		x = a; y = b; z = c;
	}

	//����<�����
	bool operator<(const Point3D &p) const
	{
		if (x == p.x)
			return y < p.y;
		else
			return  x < p.x;
	}
	//����==�����
	bool operator == (const Point3D &p) const
	{
		return ((x == p.x) && (y == p.y));
	}
};

//�����εı�
class Edge
{
private:
	Point3D pBegin;
	Point3D  pEnd;

public:
	Edge(){}
	Edge(Point3D v1, Point3D v2)
	{
		if (v1 < v2)
		{
			pBegin = v1; pEnd = v2;
		}
		else
		{
			pBegin = v2;  pEnd = v1;
		}
	}
	//����<�����
	bool operator<(const Edge &e) const
	{
		if (pBegin == e.pBegin)
			return pEnd < e.pEnd;
		else
			return pBegin < e.pBegin;
	}
	//����==�����
	bool operator==(const Edge &e) const
	{
		return ((pBegin == e.pBegin) && (pEnd == e.pEnd));
	}
	friend class Triangulation;
};

//������
class Triangle
{
private:
	Point3D mCenter;     //���������ԲԲ��
	double radius;       //���������Բ�뾶
	double r2;           //���������Բ�뾶��ƽ��

public:
	Point3D mVertex[3];  //�����ε�3������
	Triangle(){}
	Triangle(Point3D v1, Point3D v2, Point3D v3)
	{
		mVertex[0] = v1;
		mVertex[1] = v2;
		mVertex[2] = v3;
		calculateCircumcircle();
	}

	//��ȡ�����ε����
	static double getArea(const Point3D &pt0, const Point3D &pt1, const Point3D &pt2);

	//�������������Բ
	void calculateCircumcircle();
	//�жϵ�p�Ƿ�����������
	bool inTriangle(const Point3D &p) const;

public:
	//����<�����
	bool operator <(const Triangle &t) const
	{
		return mCenter < t.mCenter;
	}
	//����==�����
	bool operator == (const Triangle &t) const
	{
		return mCenter == t.mCenter;
	}
	friend class Triangulation;
};

typedef set<Point3D>::iterator iterPoint;      //����ָ��㼯�ĵ���������
typedef set<Triangle>::iterator iterTriangle;  //����ָ�������μ��ĵ���������
typedef set<Edge>::iterator iterEdge;          //����ָ��߼��ĵ���������

//������
class Triangulation
{
private:
	int width, height;

public:
	//���캯��
	Triangulation();

public:
	int getWidth()
	{
		return width;
	}
	int getHeight()
	{
		return height;
	}
	//��ʼ��
	void initialize();
	//���ݵ㼯����������
	bool doTriangulation();
	//������Ҫ�������������ķ�Χ�������˹����������е㼯�ĳ�������
	void setDelaunayAreaSize(int width, int height);
	//����������е㼯�ĳ�������
	Triangle createSuperTriangle();
	//�ú�������һ���㣬�ڵ�ǰ�������������������Բ�����õ��������
	void findTriangleWithPoint(iterPoint iterPt);
	//ʹ���ҵ��Ķ���Σ����߼�ebuffer�е����бߣ��뵱ǰ�㹹���µ�������
	void CreateTriangleWithPt(iterPoint iterPt);
	//ɾ����������
	void deleteSuperTriangle();

public:
	Triangle superTriangle;  //�������Σ������������㷨��
	bool bTrianglationFinished;  //�������Ƿ񹹽����
	set<Point3D> pointSet;      //���ڴ����㼯��ʹ��set������Ե�������򣬼ӿ칹�����ٶ�
	set<Triangle> triangleSet;  //���ڹ���������ɵ�������
	vector<iterTriangle> tempTvec;    //���ڴ�������һ����ҵ������������ε�ָ�룬������ָ����������ɾ��������ʵ�壬Ӱ������������������	
	set<Edge> ebuffer;          //���ڹ����������������Σ��γɵĶ����
};

#endif