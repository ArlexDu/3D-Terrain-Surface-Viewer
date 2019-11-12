#ifndef GLVIEWER_H
#define GLVIEWER_H

//#include <QtGui>
//#include <QtCore>
#include <gl\glaux.h>
#include <QtOpenGL\QtOpenGL>
//#include <QWidget>
#include"Triangulation.h"

class GLViewer : public QGLWidget
{
	Q_OBJECT

private:
	//�Ը����ķ�ʽ������ά����
	void DrawGLGridScene();
	//���������ķ�ʽ������ά����
	void DrawGLTriangulationScene();

private slots:
	//��ת
	void alwaysRotate();
	//���ݸ̷ֲ߳���ɫ��Ⱦ
	void GLViewer::setColor(float height);

signals:
	void zRotationChanged(int angle);

public:
	explicit GLViewer(QGLWidget *parent = 0);
	~GLViewer();

	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	//���̰����¼�
	void keyPressEvent(QKeyEvent *e);
	//���̵����¼�
	void keyReleaseEvent(QKeyEvent *e);
	void showEvent(QShowEvent *e);

	QImage* demImage = nullptr;  //����DEM
	Triangulation* pTriangulation = nullptr;  //������
	QOpenGLTexture *texture = nullptr;  //����

private:
	float rtri = 0;  //��ת��

	bool myKey[255];

	bool bwireframe = false;  //bGridΪtrueʱ���ߣ�Ϊfalseʱ����
	bool blight;  //����
	bool stopRotate = true;  //stopRotateΪtrueʱ����ת
	bool bGrid = true;  //bGridΪtrueʱ��������Ϊfalseʱ��������

	float moveX = 0, moveY = 0, moveZ = 0;
	float direction = 0.15f;

	Point3D *tnormal;

};

#endif