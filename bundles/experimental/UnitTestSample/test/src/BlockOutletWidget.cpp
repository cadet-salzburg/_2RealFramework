#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent) : m_OutletHandle(outletHandle),  QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	if(m_OutletHandle.getTypename().find("img")==0)
	{
		//m_ValueWidget = new QLabel(this);
	}
	else
	{
		m_ValueWidget = new QLabel(QString::fromStdString( "" ));
		layout->addWidget( m_ValueWidget );
		setLayout( layout );
	}
	
	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

	qRegisterMetaType<QImage>("QImage");
	connect(this, SIGNAL(sendPixmap(const QImage &)), this, SLOT(updatePixmap(const QImage &)));
	connect(this, SIGNAL(sendString(const QString &)), this, SLOT(updateString(const QString &)));
}

 void BlockOutletWidget::paintEvent(QPaintEvent *event)
 {
	 if(m_OutletHandle.getTypename().find("img")==0)
	 {
		if(!m_Pixmap.isNull())
		{
			QPainter painter(this);
	 		painter.drawPixmap(0,0,25,100, m_Pixmap);
		}
	 }
 }
 
 void BlockOutletWidget::resizeEvent(QResizeEvent *event)
 {
 }


void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{	
		unsigned char* ptr = data.getData<ImageT<unsigned char>>().getData();
		static unsigned char* ptr1 = new unsigned char[320*240*4]; 
		memcpy( ptr1, ptr, 320*240*4);
		m_Img = QImage( ptr1, 320,240, QImage::Format_RGB32);
	
		m_AppData = data;
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
//	m_Mutex.lock();
	m_Pixmap = QPixmap::fromImage(img);
//	m_Mutex.unlock();
	update();
}

