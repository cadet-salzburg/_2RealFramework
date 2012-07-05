#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent) : m_OutletHandle(outletHandle),  QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	/*if(m_OutletHandle.getTypename().find("img")==0)
	{
		m_ValueWidget = new QLabel(this);
	}
	else
	{
		m_ValueWidget = new QLabel(QString::fromStdString( "" ));
	}
	
	layout->addWidget( m_ValueWidget );*/
	setLayout( layout );
	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

	qRegisterMetaType<QImage>("QImage");
	connect(this, SIGNAL(sendPixmap(const QImage &)), this, SLOT(updatePixmap(const QImage &)), Qt::BlockingQueuedConnection);
}

 void BlockOutletWidget::paintEvent(QPaintEvent *event)
 {
	 if(!m_Pixmap.isNull())
	 {
		 m_Mutex.lock();
		 QPainter painter(this);
	 	painter.drawPixmap(0,0,320,240, m_Pixmap);
		m_Mutex.unlock();
	 }
 }
 
 void BlockOutletWidget::resizeEvent(QResizeEvent *event)
 {
 }


void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{	
		m_Mutex.lock();
		
		unsigned char* ptr = data.getData<ImageT<unsigned char>>().getData();
		static unsigned char* ptr1 = new unsigned char[320*240*4]; 
		memcpy( ptr1, ptr, 320*240*4);
		m_Img = QImage( ptr, 320,240, QImage::Format_RGB32);

	//	delete[] ptr;
		std::cout << "XXXX" << std::endl;
		m_AppData = data;
		m_Mutex.unlock();
		emit sendPixmap( m_Img );
		

	}
	//else if( m_OutletHandle.getTypename() == "double" )
	//{
	//	dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
	//}
}


void BlockOutletWidget::updatePixmap(const QImage& img) 
{
	m_Mutex.lock();
	m_Pixmap = QPixmap::fromImage(img);
	m_Mutex.unlock();
	repaint();
}

