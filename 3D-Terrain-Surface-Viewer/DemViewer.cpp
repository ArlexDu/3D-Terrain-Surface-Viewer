#include"DemViewer.h"

//���캯��
DemViewer::DemViewer(QLabel *parent) :QLabel(parent)
{
	setBackgroundRole(QPalette::Base);
	setScaledContents(true);
	hasData = false;
	scale = 1.0;
}
//����DEM����
void DemViewer::loadDemData(const QString &fileName)
{
	QImageReader reader(fileName);
	reader.setAutoTransform(true);
	demImage = reader.read();
	if (demImage.isNull())  //����DEM����ʧ��
	{
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
		setWindowFilePath(QString());
		setPixmap(QPixmap());
		adjustSize();
		return;
	}
	setPixmap(QPixmap::fromImage(demImage));
	hasData = true;
}
//����
void DemViewer::zoom()
{
	resize(scale * pixmap()->size());
}
//��ȡ����������
void DemViewer::extractFeatures()
{
	if (!hasData)
		return;
	pointSet.clear();  //��յ㼯
	int width = demImage.width();  //ͼ��Ŀ��/����
	int height = demImage.height();  //ͼ��ĸ߶�/����

	int threshold = 7;  //��ֵ,��ֵԽ��������Խ��
	int wsize = 21;  //���ڴ�С
	float wdata[21];
	//��������ȡ�㷨
	for (int j = 3; j < height; j += 3)
	{
		for (int i = 3; i < width - wsize; i += 3)
		{
			int cve = qGray(demImage.pixel(i + wsize - 1, j));
			int cvb = qGray(demImage.pixel(i, j));
			float k = (cve - cvb) / wsize;
			float A = k;
			float B = -1.0;
			int yi = qGray(demImage.pixel(i, j));

			int x, y;
			int n = 0;

			for (int a = i + 1; a < i + wsize; a++)
			{
				int av = qGray(demImage.pixel(a, j));
				int xa = a - i;
				int ya = av - yi;

				float dis = fabs(A*xa + B*ya);
				wdata[n++] = dis;
			}

			int xm = i + wsize / 2;
			int ym = qGray(demImage.pixel(i + wsize / 2, j)) - yi;

			float mdis = fabs(A*xm + B*ym);

			float max = -1;
			for (int d = 0; d < n; d++)
				if (max < wdata[d])
					max = wdata[d];

			if (mdis == max && max > threshold)
			{
				x = i + wsize / 2;
				y = j;
				float iv = qGray(demImage.pixel(x, y));
				float ix = x + rand() % 100 * 0.00001F;  //��Ҫ���������x��y�ϼ�һ��С���Ŷ�������ֹ������������������
				float iy = y + rand() % 100 * 0.00001F;
				Point3D p = Point3D(ix, iy, iv);
				pointSet.insert(p);
			}
		}
	}
	this->update();
}
//��ͼ�¼�
void DemViewer::paintEvent(QPaintEvent *  /*event*/)
{
	if (!hasData)
		return;
	//��DEMͼ��
	QPainter painter(this);
	painter.setPen(QColor(Qt::yellow));
	if (pixmap())
	{
		const QPixmap* pix = pixmap();
		QRectF target = QRectF(QPointF(0, 0), scale* pix->size());  //Ŀ�����
		QRectF source = QRectF(QPointF(0, 0), pix->size());  //����Դ
		painter.drawPixmap(target, *pix, source);
	}
	//������������
	int n = pointSet.size();
	if (n > 1)
	{
		auto iter = pointSet.begin();
		for (; iter != pointSet.end(); iter++)
		{
			Point3D point = *iter;
			QRect rect = QRect(scale * point.x, scale * point.y, 3, 3);
			rect.translate(-1, -1);
			painter.drawRect(rect);
		}
	}
}