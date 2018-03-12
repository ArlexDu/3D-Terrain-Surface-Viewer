#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mymainwindow.h"

#include<QtCore\qstandardpaths.h>
#include<QtWidgets\qscrollbar.h>
#include<QtWidgets\qscrollarea.h>
#include<QtWidgets\qfiledialog.h>
#include"DemViewer.h"
#include"TriangulationViewer.h"
#include"GLViewer.h"

//�����������
class MyMainWindow : public QMainWindow
{
	Q_OBJECT
		
public:
	MyMainWindow(QWidget *parent = 0);
	~MyMainWindow();

private:
	Ui::MyMainWindowClass ui;

//slots
private slots:
	//��DEMͼ���ļ�
	void open();
	//�Ŵ�
	void zoomIn();
	//��С
	void zoomOut();
	//��ʼ��С
	void normalSize();
	//��ȡ����������
	void extractDemFeatures();
	//������ȡ�ĵ����������������������
	void doTriangulation();
	//��ʾ��ά����
	void show3DViewer();
	//����
	void about();

//�ؼ�
private:
	Triangulation triangulation;  //������
	double scaleFactor;  //����ϵ��
	DemViewer* pDemViewer;                        //��ʾDEMͼ��Ŀؼ�
	TriangulationViewer* pTriangulationViewer;    //��ʾ�������Ŀؼ�
	GLViewer *pGLViewer;                          //��ʾ3D���α���Ŀؼ�
	QScrollArea* pScrollAreaDemViewer;            //����demDataViewer��ScrollArea
	QScrollArea* pScrollAreaTriangulationViewer;  //����TriangulationViewer��ScrollArea
	QScrollArea *pScrollAreaGLViewer;             //����GLViewer��ScrollArea
	QTabWidget* pTabWidget;                       //tab control�ؼ�

private:
	//��ָ���ļ�·������DEM����
	bool loadFile(const QString &);
	//�������
	void createActions();
	//����˵�
	void createMenus();
	//���칤����
	void createToolBars();

	//����ͼƬ
	void scaleImage(double factor);
	//����������
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

	QAction *openAct;         //���ļ�����
	QAction *exitAct;         //�˳�����
	QAction *zoomInAct;       //�Ŵ���
	QAction *zoomOutAct;      //��С����
	QAction *normalSizeAct;   //�ָ�1:1���Ŷ���
	QAction *aboutAct;            //���ڶ���
	QAction *doTriangulationAct;  //��������������
	QAction *extractFeaturesAct;  //��ȡ���������㶯��
	QAction *show3DViewerAct;     //��ʾ��ά���ζ���

	QMenu *fileMenu;  //�ļ��˵�
	QMenu *viewMenu;  //��ͼ�˵�
	QMenu *helpMenu;  //�����˵�

	QToolBar *fileToolBar;  //�ļ�������
	QToolBar *editToolBar;  //�༭������
	QToolBar *viewToolBar;  //��ͼ������
};

#endif // MYMAINWINDOW_H
