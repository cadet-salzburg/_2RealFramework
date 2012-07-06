#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent) 
	: m_ImageHandle(imageHandle),
	  QGroupBox(parent)
{
	
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( m_ImageHandle.getName() )) );
	m_ValueWidget = new QLabel(QString::fromStdString( "" ));
	QObject::connect( this, SIGNAL(sendImage(unsigned char*)), this, SLOT(setImage(unsigned char *)));
	//QObject::connect( this, SIGNAL(sendResolution(_2Real::StringEnumeration const&)), this, SLOT(setResolution(_2Real::StringEnumeration const&)));
	//qRegisterMetaType<_2Real::StringEnumeration>("_2Real::StringEnumeration");
	layout->addWidget( m_ValueWidget );
	setLayout( layout );
	m_ImageHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );
//	m_ResolutionHandle.registerToNewData( *this, &BlockOutletWidget::receiveResolution );
}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_ImageHandle.getTypename().find("img_uchar")==0)
	{
		m_ImageData = data;
		m_Width =  m_ImageData.getData<ImageT<unsigned char>>().getWidth();
		m_Height =  m_ImageData.getData<ImageT<unsigned char>>().getHeight();
		unsigned char* ptr = m_ImageData.getData<ImageT<unsigned char>>().getData();
		emit sendImage( ptr );
	}
	else if( m_ImageHandle.getTypename() == "double" )
	{
		dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
	}
}

//void BlockOutletWidget::receiveResolution(_2Real::app::AppData const& data)
//{
//	m_ResolutionData = data;
//	emit sendResolution(  m_ResolutionData.getData<_2Real::StringEnumeration>() );
//}


void BlockOutletWidget::setImage( unsigned char *img )
{
	//QImage imageReceived = QImage( img, im, m_Height, QImage::Format_RGB888 );
	QImage imageReceived = QImage( img, m_Width, m_Height, QImage::Format_RGB888 );
	imageReceived.mirrored( true );
	dynamic_cast<QLabel*>(m_ValueWidget)->setPixmap(QPixmap::fromImage( imageReceived ));
}

//void BlockOutletWidget::setResolution(_2Real::StringEnumeration const& res )
//{
//	_2Real::StringEnumeration theEnum = res;
//	m_Width  = atoi(theEnum["width"].c_str());
//	m_Height = atoi(theEnum["height"].c_str());
//}

