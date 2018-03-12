#ifndef TRIANGULATIONVIEWER_H
#define TRIANGULATIONVIEWER_H

#include<QtWidgets\qlabel.h>
#include<QtGui\qpen.h>
#include<QtGui\qimagereader.h>
#include<QtGui\qguiapplication.h>
#include<QtGui\qpainter.h>
#include<QtCore\qdir.h>
#include <QMessageBox>
#include"Triangulation.h"

//��������ͼ�ؼ�
class TriangulationViewer : public QLabel
{
	Q_OBJECT
private:
	QPen pen;  //����
	bool hasConstructed;   //�������Ƿ񹹽����

public:
	Triangulation *pTriangulation;  //������

	//���캯��
	TriangulationViewer(QLabel *parent = 0);
	//����������
	void doTriangulation();
	//����������(Ŀ���Ǵ����Ѿ�ѡ���������������)
	void setTriangulation(Triangulation* p) { pTriangulation = p; }

public:
	//��ͼ�¼�
	void paintEvent(QPaintEvent *event);

};

#endif