#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

BlockOutletWidget::BlockOutletWidget( _2Real::app::OutletHandle& outletHandle, QWidget *parent ) :
	QGroupBox( parent ),
	m_Handle( outletHandle ),
	m_2RealImage( 320, 240, _2Real::ImageChannelOrder::RGBA ),
	m_Typename( outletHandle.getTypename() )
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget( new QLabel( QString::fromStdString( m_Handle.getName() ) ) );

	// note that the moc is quite pedantic with regards to namespaces:
	// you must use the full symbol name here & in the function implementations
	qRegisterMetaType< _2Real::app::AppData >( "_2Real::app::AppData" );
	connect( this, SIGNAL( sendPixmap( _2Real::app::AppData ) ), this, SLOT( updatePixmap( _2Real::app::AppData ) ) );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// not really necessary, just giving some initial display img to the widget
	if ( m_Typename == "img_uchar" )
	{
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

		m_QImage = QImage( m_2RealImage.getData(), 320, 240, QImage::Format_RGB32 );
		m_QPixmap = QPixmap::fromImage( m_QImage );
		update();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_Handle.registerToNewData( *this, &BlockOutletWidget::receiveData );
}

void BlockOutletWidget::paintEvent( QPaintEvent *event )
{
	if ( m_Typename == "img_uchar" )
	{
		m_QPixmapAccess.lock();

		QPainter painter(this); 
		painter.drawPixmap( 0, 0, 320, 240, m_QPixmap );

		m_QPixmapAccess.unlock();
	}
}

void BlockOutletWidget::receiveData( AppData const& data )
{
	emit sendPixmap( data );		// since a copy of the incoming data is emitted
									// it does not matter how much time passes 
									// between this and 'updatePixmap':
									// the data will for sure be alive when the pixmap is updated
}

void BlockOutletWidget::updatePixmap( _2Real::app::AppData data ) 
{
	if ( m_Typename == "img_uchar" )
	{
		m_QPixmapAccess.lock();

		m_AppData = data;			// keeps current image data alive
		m_QImage = QImage( data.getData< ImageT< unsigned char > >().getData(), 320, 240, QImage::Format_RGB32 );
		m_QPixmap = QPixmap::fromImage( m_QImage );
		update();

		m_QPixmapAccess.unlock();
	}
}