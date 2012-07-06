#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget( _2Real::app::OutletHandle& outletHandle, QWidget *parent ) :
	QGroupBox( parent ),
	m_OutletHandle( outletHandle ),
	m_2RealImage( 320, 240, _2Real::ImageChannelOrder::RGBA )
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget( new QLabel( QString::fromStdString( m_OutletHandle.getName() ) ) );

	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

	qRegisterMetaType< QImage >( "QImage" );
	connect( this, SIGNAL( sendPixmap( QImage const& ) ), this, SLOT( updatePixmap( QImage const& ) ), Qt::DirectConnection );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// not really necessary, just giving some initial display img to the widget

	ImageT< unsigned char >::iterator it = m_2RealImage.iter();
	while ( it.nextLine() )
	{
		while ( it.nextPixel() )
		{
			it.r() = 0;
			it.g() = 100;
			it.b() = 255;
			it.a() = 255;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_QPixmapAccess.lock();

	m_QImage = QImage( m_2RealImage.getData(), 320, 240, QImage::Format_RGB32 );
	emit sendPixmap( m_QImage );
}

void BlockOutletWidget::paintEvent( QPaintEvent *event )
{
	if ( m_OutletHandle.getTypename().find( "img_uchar" ) == 0 )
	{
		m_QPixmapAccess.lock();

		QPainter painter(this); 
		painter.drawPixmap( 0, 0, 320, 240, m_QPixmap );

		m_QPixmapAccess.unlock();
	}
}

void BlockOutletWidget::receiveData( _2Real::app::AppData const& data )
{
	if( m_OutletHandle.getTypename().find( "img_uchar" ) == 0 )
	{
		m_QPixmapAccess.lock();		// <- locked here, but will be unlocked in update pixmap fnc

		m_AppData = data;
		//m_2RealImage = m_AppData.getData< ImageT< unsigned char > >();
		m_QImage = QImage( m_AppData.getData< ImageT< unsigned char > >().getData(), 320, 240, QImage::Format_RGB32 );
		emit sendPixmap( m_QImage );
	}
}

void BlockOutletWidget::updatePixmap( QImage const& image ) 
{
	m_QPixmap = QPixmap::fromImage( m_QImage );
	update();
	m_QPixmapAccess.unlock();		// <-- was locked from the calling function
}