#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent) 
	: m_OutletHandle(imageHandle),
	  QGroupBox(parent)
{
	
	m_Layout = new QHBoxLayout();
	m_Layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	if(m_OutletHandle.getTypename().find("img")==0)
	{
		//m_ValueWidget = new QLabel(this);
	}
	else
	{
		m_ValueWidget = new QLabel(QString::fromStdString( "" ));
		m_Layout->addWidget( m_ValueWidget );
		setLayout( m_Layout );
	}

	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

	qRegisterMetaType<QImage>("QImage");
	connect(this, SIGNAL(sendPixmap(const QImage &)), this, SLOT(updatePixmap(const QImage &)), Qt::DirectConnection );
	connect(this, SIGNAL(sendString(const QString &)), this, SLOT(updateString(const QString &)));

}

void BlockOutletWidget::paintEvent(QPaintEvent *event)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0  )
	{
		m_QPixmapAccess.lock();
		if(!m_Pixmap.isNull())
		{
			QPainter painter(this);
			painter.drawPixmap(0,0,m_Pixmap.width(),m_Pixmap.height(), m_Pixmap);
		}
		m_QPixmapAccess.unlock();
	}
}

void BlockOutletWidget::resizeEvent(QResizeEvent *event)
{

}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{	

		m_QPixmapAccess.lock();
		m_AppData = data;

		m_Width  = m_AppData.getData<ImageT<unsigned char>>().getWidth();
		m_Height = m_AppData.getData<ImageT<unsigned char>>().getHeight();
		unsigned char* ptr = m_AppData.getData<ImageT<unsigned char>>().getData();
		m_Img = QImage( ptr, m_Width,m_Height, QImage::Format_RGB888);
		emit sendPixmap( m_Img );	
	}
	else if( m_OutletHandle.getTypename() == "double" )
	{
		emit sendString(QString::fromStdString(data.getDataAsString()));
	}
}

void BlockOutletWidget::updateString(const QString& str) 
{
	dynamic_cast<QLabel*>(m_ValueWidget)->setText(str);
	update();
}

void BlockOutletWidget::updatePixmap(const QImage& img) 
{
	m_Pixmap = QPixmap::fromImage(img);
	update();
	m_QPixmapAccess.unlock();
}
