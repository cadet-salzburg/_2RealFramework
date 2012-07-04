#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent) : m_OutletHandle(outletHandle),  QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	if(m_OutletHandle.getTypename().find("img")==0)
	{
		m_ValueWidget = new QLabel("");
	}
	else
	{
		m_ValueWidget = new QLabel(QString::fromStdString( "" ));
	}

	QObject::connect( this, SIGNAL(sendImage(unsigned char*)), this, SLOT(setImage(unsigned char *)));
	
	layout->addWidget( m_ValueWidget );
	setLayout( layout );
	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );
}
void BlockOutletWidget::setImage( unsigned char *img )
{
	//m_Mutex.lock();
	QImage imageReceived = QImage( img, 640, 480, QImage::Format_RGB888 );
	imageReceived.mirrored( true );
	dynamic_cast<QLabel*>(m_ValueWidget)->setPixmap(QPixmap::fromImage( imageReceived ));
	//m_Mutex.unlock();
}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{
		//unsigned char* ptr = data.getData<ImageT<unsigned char>>().getData();
		//layout()->removeWidget(m_ValueWidget);
		//m_ValueWidget->setParent(NULL);
		//delete m_ValueWidget;
		//QImage m_ValueWidget = QImage(ptr, 640, 480 , QImage::Format_RGB32 );
		//layout()->addWidget( m_ValueWidget );
		m_Mutex.lock();
		m_Data = data;
		unsigned char* ptr = m_Data.getData<ImageT<unsigned char>>().getData();
		//QImage img = QImage(ptr, 640, 480 , QImage::Format_RGB888);
		emit sendImage( ptr );
		m_Mutex.unlock();
	}
	else if( m_OutletHandle.getTypename() == "double" )
	{
		dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
	}
}
