#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent) 
	: m_OutletHandle(imageHandle),
	  QGroupBox(parent)
{
	m_Layout = new QHBoxLayout();
	m_Layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	m_ValueWidget = new QLabel(this);
	m_Layout->addWidget( m_ValueWidget );
	setLayout( m_Layout );

	// register data callback for _2Real Framework
	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

	// note that the moc is quite pedantic with regards to namespaces:
	// you must use the full symbol name here & in the function implementations
	// data between threads has to be sent around through signal and slots in QT !!!!
	qRegisterMetaType< _2Real::app::AppData >( "_2Real::app::AppData" );
	connect( this, SIGNAL( sendData( _2Real::app::AppData ) ), this, SLOT( updateData( _2Real::app::AppData ) ) );
}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	//keep the data around as long as it and it's underlying pointers are needed, it is internally a share_ptr !!!!!
	emit sendData( data );			// since a copy of the incoming data is emitted
									// it does not matter how much time passes 
									// between this and 'updateData':
									// the data will for sure be alive when the pixmap is updated
}

void BlockOutletWidget::updateData(_2Real::app::AppData data) 
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{	
		m_Width  = data.getData<ImageT<unsigned char>>().getWidth();
		m_Height = data.getData<ImageT<unsigned char>>().getHeight();
		unsigned char* ptr = data.getData<ImageT<unsigned char>>().getData();
		m_Img = QImage( ptr, m_Width,m_Height, QImage::Format_RGB888);
		m_Pixmap = QPixmap::fromImage(m_Img);

		dynamic_cast<QLabel*>(m_ValueWidget)->setPixmap(m_Pixmap);
	}
	else if( m_OutletHandle.getTypename() == "double" )
	{
		dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
	}

	update();
}