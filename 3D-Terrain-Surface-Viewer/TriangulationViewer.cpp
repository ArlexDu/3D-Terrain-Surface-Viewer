#include"TriangulationViewer.h"
//���캯��
TriangulationViewer::TriangulationViewer(QLabel *parent) : QLabel(parent)
{
	setBackgroundRole(QPalette::Base);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	setScaledContents(true);
	hasConstructed = false;
	pTriangulation = nullptr;
	resize(1024, 768);
}
//����������
void TriangulationViewer::doTriangulation()
{
	if (pTriangulation != nullptr)
		hasConstructed = pTriangulation->doTriangulation();
}
//��ͼ�¼�
void TriangulationViewer::paintEvent(QPaintEvent *  /*event*/)
{
	QPainter painter(this);
	painter.setPen(QColor(Qt::blue));
	if (pTriangulation == nullptr)
		return;
	//��������λ�������
	for (iterTriangle pset = pTriangulation->triangleSet.begin(); pset != pTriangulation->triangleSet.end(); pset++)
	{
		QPoint p1 = QPoint(pset->mVertex[0].x, pset->mVertex[0].y);
		QPoint p2 = QPoint(pset->mVertex[1].x, pset->mVertex[1].y);
		QPoint p3 = QPoint(pset->mVertex[2].x, pset->mVertex[2].y);
		painter.drawLine(p1, p2);
		painter.drawLine(p2, p3);
		painter.drawLine(p3, p1);
	}
}