#include "mymainwindow.h"
//���캯��
MyMainWindow::MyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	pDemViewer = new DemViewer();
	pScrollAreaDemViewer = new QScrollArea;
	pScrollAreaDemViewer->setBackgroundRole(QPalette::Dark);
	pScrollAreaDemViewer->setWidget(pDemViewer);

	pTriangulationViewer = new TriangulationViewer();
	pScrollAreaTriangulationViewer = new QScrollArea;
	pScrollAreaTriangulationViewer->setBackgroundRole(QPalette::Dark);
	pScrollAreaTriangulationViewer->setWidget(pTriangulationViewer);
	
	pGLViewer = new GLViewer();  //OpenGL Viewer	
	pScrollAreaGLViewer = new QScrollArea;
	pScrollAreaGLViewer->setBackgroundRole(QPalette::Dark);
	pScrollAreaGLViewer->setWidget(pGLViewer);
	pScrollAreaGLViewer->setMinimumSize(100, 100);
	pScrollAreaGLViewer->setWidgetResizable(true);
	pScrollAreaGLViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	pTabWidget = new QTabWidget;
	pTabWidget->addTab(pScrollAreaDemViewer, "DEM Data Viewer");
	pTabWidget->addTab(pScrollAreaTriangulationViewer, "DEM Triangulation Viewer");
	pTabWidget->addTab(pScrollAreaGLViewer, "DEM 3D Viewer");
	setCentralWidget(pTabWidget);
	createActions();
	createMenus();
	createToolBars();
	//resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}
//��������
MyMainWindow::~MyMainWindow()
{
	delete openAct;         //���ļ�����
	delete exitAct;         //�˳�����
	delete zoomInAct;       //�Ŵ���
	delete zoomOutAct;      //��С����
	delete normalSizeAct;   //���ű�Ϊ1
	delete aboutAct;            //���ڶ���
	delete doTriangulationAct;  //��������������
	delete extractFeaturesAct;  //��ȡ���������㶯��

	delete fileMenu;  //�ļ��˵�
	delete viewMenu;  //��ͼ�˵�
	delete helpMenu;  //�����˵�

	delete fileToolBar;  //�ļ�������
	delete editToolBar;  //�༭������
	delete viewToolBar;  //��ͼ������

	delete pDemViewer;             //��ʾDEMͼ��Ŀؼ�
	delete pTriangulationViewer;   //��ʾ�������Ŀؼ�
	delete pScrollAreaDemViewer;            //����demDataViewer��ScrollArea
	delete pScrollAreaTriangulationViewer;  //����TriangulationViewer��ScrollArea
	delete pTabWidget;                      //tab control�ؼ�
}
//��ָ���ļ�·������DEM����
bool MyMainWindow::loadFile(const QString &fileName)
{
	pDemViewer->loadDemData(fileName);
	if (pDemViewer->HasData())  //����Ѿ�����DEMͼ��
	{
		extractFeaturesAct->setEnabled(true);  //������ȡ����������
		show3DViewerAct->setEnabled(true);     //������ʾ3ά����
	}
	scaleFactor = 1.0;
	pDemViewer->adjustSize();
	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	normalSizeAct->setEnabled(true);
	setWindowFilePath(fileName);
	int width = pDemViewer->demImage.width();
	int height = pDemViewer->demImage.height();
	pTriangulationViewer->resize(width + 20, height + 20);  //������������ͼ�ĸߺͿ�ʹ��ʾ������
	pGLViewer->demImage = &pDemViewer->demImage;
	return true;
}
//���ļ�
void MyMainWindow::open()
{
	QStringList mimeTypeFilters;
	foreach(const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
		mimeTypeFilters.append(mimeTypeName);
	mimeTypeFilters.sort();
	const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

	//ѡ���ļ��Ի���
	QFileDialog dialog(this, tr("Open File"), picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setMimeTypeFilters(mimeTypeFilters);
	dialog.selectMimeTypeFilter("image/jpeg");
	while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first()))
		;
	pTabWidget->setCurrentIndex(0);
}
//�Ŵ�
void MyMainWindow::zoomIn()
{
	scaleImage(1.25);
}
//��С
void MyMainWindow::zoomOut()
{
	scaleImage(0.8);
}
void MyMainWindow::normalSize()
{
	scaleFactor = 1.0;
	pDemViewer->scale = scaleFactor;
	pDemViewer->adjustSize();
}
void MyMainWindow::about()
{
	QMessageBox::about(this, tr("About This Program"),
		tr("<p>By LiuZhengyuan</p><p>Student ID: 1300012430 </p><p>Institude of RS & GIS</p><p>School of Earth and Space Sciences</p>"));
}
//��ȡ����������
void MyMainWindow::extractDemFeatures()
{
	pDemViewer->extractFeatures();
	doTriangulationAct->setEnabled(true);
}
//������ȡ�ĵ��������㹹��������
void MyMainWindow::doTriangulation()
{
	int width = pDemViewer->demImage.width();
	int height = pDemViewer->demImage.height();
	triangulation.initialize();
	triangulation.setDelaunayAreaSize(width, height);
	triangulation.pointSet = pDemViewer->pointSet;
	pTriangulationViewer->setTriangulation(&triangulation);
	pTriangulationViewer->doTriangulation();
	pTabWidget->setCurrentIndex(1);
	pGLViewer->pTriangulation = &triangulation;
}
//��ʾ3D DEM��ͼ
void MyMainWindow::show3DViewer()
{
	pGLViewer->initializeGL();
	pGLViewer->resizeGL(640, 480);
	pTabWidget->setCurrentIndex(2);
}
//�������
void MyMainWindow::createActions()
{
	//���ļ�����
	openAct = new QAction(QIcon("icons/open16.png"), tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	//�˳�����
	exitAct = new QAction(QIcon("icons/GenericDeleteRed16.png"), tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	//�Ŵ����
	zoomInAct = new QAction(QIcon("icons/ZoomInTool16.png"), tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
	//��С����
	zoomOutAct = new QAction(QIcon("icons/ZoomOutTool16.png"), tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
	//�ָ�ԭʼ��С
	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));
	//��ȡ����������
	extractFeaturesAct = new QAction(QIcon("icons/EditingStopEditing16.png"), tr("&Extract Topographic Features"), this);
	extractFeaturesAct->setShortcut(tr("Ctrl+E"));
	extractFeaturesAct->setEnabled(false);
	connect(extractFeaturesAct, SIGNAL(triggered()), this, SLOT(extractDemFeatures()));
	//����������
	doTriangulationAct = new QAction(QIcon("icons/EditingEditVertices16.png"), tr("Create &Triangulation"), this);
	doTriangulationAct->setShortcut(tr("Ctrl+T"));
	doTriangulationAct->setEnabled(false);
	connect(doTriangulationAct, SIGNAL(triggered()), this, SLOT(doTriangulation()));
	//��ʾ��ά���ζ���
	show3DViewerAct = new QAction(QIcon("icons/DEM.png"), tr("&Show 3D DEM"), this);
	show3DViewerAct->setShortcut(tr("Ctrl+K"));
	show3DViewerAct->setEnabled(false);
	connect(show3DViewerAct, SIGNAL(triggered()), this, SLOT(show3DViewer()));
	//����
	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}
//�����˵���
void MyMainWindow::createMenus()
{
	//�����ļ��˵�
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addSeparator();
	fileMenu->addAction(extractFeaturesAct);
	fileMenu->addAction(exitAct);
	//������ͼ�˵�
	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	//���������˵�
	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(helpMenu);
}
//����������
void MyMainWindow::createToolBars()
{
	//�򿪹�����
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(exitAct);
	//�༭������
	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(extractFeaturesAct);
	editToolBar->addAction(doTriangulationAct);
	editToolBar->addAction(show3DViewerAct);
	//��ͼ������
	viewToolBar = addToolBar(tr("Edit"));
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
}
//�����������ӽ�������
void MyMainWindow::scaleImage(double factor)
{
	Q_ASSERT(pDemViewer->pixmap());
	scaleFactor *= factor;
	pDemViewer->scale = scaleFactor;
	pDemViewer->zoom();

	adjustScrollBar(pScrollAreaDemViewer->horizontalScrollBar(), factor);
	adjustScrollBar(pScrollAreaDemViewer->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 5.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//����������
void MyMainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()	+ ((factor - 1) * scrollBar->pageStep() / 2)));
}
