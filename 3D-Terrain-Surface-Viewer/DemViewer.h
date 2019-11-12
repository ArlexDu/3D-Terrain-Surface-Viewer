#ifndef DEMVIEWER
#define DEMVIEWER

#include<set>
#include<QtWidgets\qlabel.h>
#include<QtGui\qpen.h>
#include<QtGui\qimagereader.h>
#include<QtGui\qguiapplication.h>
#include<QtGui\qpainter.h>
#include<QtCore\qdir.h>
#include <QtWidgets\QMessageBox>
#include"Triangulation.h"
using namespace std;

//DEMͼ����ͼ�ؼ�
class DemViewer : public QLabel
{
	Q_OBJECT  //ֻ�м�����Q_OBJECT������ʹ��QT�е�signal��slot����
private:
	QPen pen;  //����
	bool hasData;  //�Ƿ��Ѿ�����DEMͼ��
	
public:	
	double scale;  //���ű�
	QImage demImage;  //DEMͼ��
	set<Point3D> pointSet;  //���������㼯��

public:
	//���캯��
	DemViewer(QLabel *parent = 0);
	//�Ƿ��Ѿ��������DEMͼ��
	bool HasData()
	{
		return hasData;
	}

	//����DEM����
	void loadDemData(const QString &fileName);
	//����
	void zoom();
	//��ȡ����������
	void extractFeatures();
	
	//��ͼ�¼�
	void paintEvent(QPaintEvent *event);
};

#endif