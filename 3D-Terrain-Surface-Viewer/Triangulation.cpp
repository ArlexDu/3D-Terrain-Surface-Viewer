#include"Triangulation.h"

/****************��������********************/
//��ȡ�����ε����
double Triangle::getArea(const Point3D& pt0, const Point3D &pt1, const Point3D &pt2)
{
	Point3D vector1, vector2;
	vector1.x = pt1.x - pt0.x;
	vector1.y = pt1.y - pt0.y;
	vector2.x = pt2.x - pt1.x;
	vector2.y = pt2.y - pt1.y;
	return fabs((vector1.x * vector2.y - vector1.y * vector2.x)) / 2.0f;  //����������˼��������ε����
}
#define REAL_EPSILON 1.192092896e-07f  //��С�ĸ�����
//�������������Բ,������������ԲԲ�ĺͰ뾶�Ȳ���
void Triangle::calculateCircumcircle()
{
	double x0 = mVertex[0].x;
	double y0 = mVertex[0].y;

	double x1 = mVertex[1].x;
	double y1 = mVertex[1].y;

	double x2 = mVertex[2].x;
	double y2 = mVertex[2].y;

	double y10 = y1 - y0;
	double y21 = y2 - y1;
	double y20 = y2 - y0;

	bool b21zero = y21 > -REAL_EPSILON && y21 < REAL_EPSILON;
	bool b10zero = y10 > -REAL_EPSILON && y10 < REAL_EPSILON;
	bool b20zero = y20 > -REAL_EPSILON && y20 < REAL_EPSILON;

	if (b20zero)  //mVertex[0]��mVertex[2]��ͬһˮƽ����
	{
		mVertex[0].y -= 0.001;
		mVertex[2].y += 0.001;
	}
	if (b10zero)  //mVertex[0]��mVertex[1]��ͬһˮƽ����
	{
		if (b21zero)	//�������㶼��ͬһˮƽ����
		{
			if (x1 > x0)  { if (x2 > x1) x1 = x2; }
			else		  { if (x2 < x0) x0 = x2; }
			mCenter.x = (x0 + x1) * 0.5F;
			mCenter.y = y0;
		}
		else
		{
			double m1 = -(x2 - x1) / y21;
			double mx1 = (x1 + x2) * 0.5F;
			double my1 = (y1 + y2) * 0.5F;

			mCenter.x = (x0 + x1) * 0.5F;
			mCenter.y = m1 * (mCenter.x - mx1) + my1;
		}
	}
	else if (b21zero)	//mVertex[1]��mVertex[2]��ͬһˮƽ����
	{
		double m0 = -(x1 - x0) / y10;
		double mx0 = (x0 + x1) * .5F;
		double my0 = (y0 + y1) * .5F;

		mCenter.x = (x1 + x2) * .5F;
		mCenter.y = m0 * (mCenter.x - mx0) + my0;
	}
	else
	{
		double m0 = -(x1 - x0) / y10;
		double m1 = -(x2 - x1) / y21;
		double mx0 = (x0 + x1) * 0.5F;
		double my0 = (y0 + y1) * 0.5F;
		double mx1 = (x1 + x2) * 0.5F;
		double my1 = (y1 + y2) * 0.5F;
		mCenter.x = (m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1);  //���������ԲԲ��x����
		mCenter.y = m0 * (mCenter.x - mx0) + my0;                   //���������ԲԲ��y����
	}
	double dx = x0 - mCenter.x;
	double dy = y0 - mCenter.y;
	r2 = dx * dx + dy * dy; 	//���������Բ�뾶��ƽ��d
	radius = (double)sqrt(r2);	//���������Բ�뾶
	r2 *= 1.000001f;
}
//�жϵ�p�Ƿ�����������
bool Triangle::inTriangle(const Point3D &p) const
{
	double S_ABC, S_ADB, S_BDC, S_ADC;
	S_ABC = getArea(mVertex[0], mVertex[1], mVertex[2]);
	S_ADB = getArea(mVertex[0], p, mVertex[2]);
	S_BDC = getArea(p, mVertex[1], mVertex[2]);
	S_ADC = getArea(mVertex[0], mVertex[1], p);
	double S_Sum = S_ADB + S_BDC + S_ADC;
	if (fabs(S_ABC - S_Sum) < 1e-8)  //���S_ADB + S_BDC + S_ADC==S_ABC����D��������ABC��
		return true;
	else
		return false;
}

/***************��������******************/
//���캯��
Triangulation::Triangulation()
{
	bTrianglationFinished = false;
	width = 0;
	height = 0;
}
//��ʼ��������
void Triangulation::initialize()
{
	triangleSet.clear();
	tempTvec.clear();
	ebuffer.clear();
}
//������Ҫ�������������ķ�Χ�������˹����������е㼯�ĳ�������
void Triangulation::setDelaunayAreaSize(int w, int h)
{
	width = w;
	height = h;
}
//����������е㼯�ĳ�������
Triangle Triangulation::createSuperTriangle()
{
	Point3D midpt = Point3D(width / 2, 2 * height, 0);
	//���˴�Y��������ֵΪ0ʱ�����ڶ����������ִ��������������Σ������������ӳ��ִ�����������Ϊ-50�������˸ô���
	Point3D vleft = Point3D(-width, -50, 0);
	Point3D vright = Point3D(width * 2, -50, 0);
	return Triangle(vleft, midpt, vright);
}
//���ݵ㼯����������
bool Triangulation::doTriangulation()
{
	superTriangle = createSuperTriangle();  //������������
	triangleSet.insert(superTriangle);      //���볬������
	iterPoint pt = pointSet.begin();
	for (; pt != pointSet.end(); pt++)  //������
	{
		findTriangleWithPoint(pt);  //���������������������Բ����һ��������Σ�����tempTvec
		CreateTriangleWithPt(pt);   //ʹ���ҵ��Ķ���Σ����߼�ebuffer�е����бߣ��뵱ǰ�㹹���µ�������
	}
	deleteSuperTriangle();  //ɾ���볬�����ζ������ӵ�������
	bTrianglationFinished = true;
	return true;
}
//���������������������Բ����һ���������
void Triangulation::findTriangleWithPoint(iterPoint pt)
{
	ebuffer.clear();
	tempTvec.clear();
	iterTriangle trianglePtr = triangleSet.begin();
	for (; trianglePtr != triangleSet.end(); trianglePtr++)
	{
		Point3D cp = trianglePtr->mCenter;
		double dx = pt->x - cp.x, dy = pt->y - cp.y;
		if (dx * dx + dy * dy < trianglePtr->r2)
		{
			tempTvec.push_back(trianglePtr);  //ʹ��vector������ʱ�ҵ��������ε�����(ָ��)
			Edge e[3];
			e[0] = Edge(trianglePtr->mVertex[0], trianglePtr->mVertex[1]);
			e[1] = Edge(trianglePtr->mVertex[1], trianglePtr->mVertex[2]);
			e[2] = Edge(trianglePtr->mVertex[2], trianglePtr->mVertex[0]);
			for (int k = 0; k < 3; k++)
				if (ebuffer.find(e[k]) != ebuffer.end())
					ebuffer.erase(e[k]);
				else
					ebuffer.insert(e[k]);
		}
	}
	int eraseTriNum = tempTvec.size();
	for (int i = 0; i < eraseTriNum; i++)
	{
		iterTriangle eraseTriPtr = tempTvec[i];
		triangleSet.erase(eraseTriPtr);
	}
}
//ʹ���ҵ��Ķ���Σ����߼�ebuffer�е����бߣ��뵱ǰ�㹹���µ�������
void Triangulation::CreateTriangleWithPt(iterPoint iterPt)
{
	for (iterEdge iter = ebuffer.begin(); iter != ebuffer.end(); iter++)
	{
		Triangle nt = Triangle(*iterPt, iter->pBegin, iter->pEnd);
		triangleSet.insert(nt);
	}
}
//ɾ����������
void Triangulation::deleteSuperTriangle()
{
	for (iterTriangle iter = triangleSet.begin(); iter != triangleSet.end();)
		if (pointSet.find(iter->mVertex[0]) == pointSet.end() || pointSet.find(iter->mVertex[1]) == pointSet.end() || pointSet.find(iter->mVertex[2]) == pointSet.end())
			iter = triangleSet.erase(iter);
		else
			iter++;
}