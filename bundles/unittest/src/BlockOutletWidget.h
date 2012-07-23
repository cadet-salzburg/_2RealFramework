#include "_2RealApplication.h"
#include <QtGui>
#include "qmutex.h"
#include <list>
#include "QGlTextureImage.h"
#include "_2RealDatatypes.h"

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);

private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_OutletHandle;
	QImage							m_Img;
	QPixmap							m_Pixmap;
	QHBoxLayout*					m_Layout;

public slots:
	void updateData(_2Real::app::AppData data);

signals:
	void sendData(_2Real::app::AppData data);
};
