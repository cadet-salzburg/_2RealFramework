#include "_2RealApplication.h"
#include <QtGui>
#include <list>
#include "qmutex.h"
#include "_2RealDatatypes.h"

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);
	//void receiveResolution(_2Real::app::AppData const& data);

public slots:
		void setImage( unsigned char* img );
		//void setResolution( _2Real::StringEnumeration const& res );
signals:
		void sendImage( unsigned char* img );
		//void sendResolution( _2Real::StringEnumeration const& res );
private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_ImageHandle;
	_2Real::app::OutletHandle		m_ResolutionHandle;
	_2Real::app::AppData			m_ImageData;
	_2Real::app::AppData			m_ResolutionData;
	int								m_Width;
	int								m_Height;
};