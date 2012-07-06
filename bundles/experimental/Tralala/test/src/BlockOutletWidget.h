#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QGroupBox
{

Q_OBJECT

public:

	BlockOutletWidget::BlockOutletWidget( _2Real::app::OutletHandle &outletHandle, QWidget *parent = nullptr );
	void receiveData(_2Real::app::AppData const& data);

protected:

	void	paintEvent( QPaintEvent *event );

private:

	QImage							m_QImage;
	QPixmap							m_QPixmap;
	QMutex							m_QPixmapAccess;
	std::string						m_Typename;
	_2Real::app::OutletHandle		m_Handle;
	_2Real::app::AppData			m_AppData;
	_2Real::ImageT< unsigned char >	m_2RealImage;

public slots:

	void	updatePixmap( _2Real::app::AppData );

signals:

	void	sendPixmap( _2Real::app::AppData );

};