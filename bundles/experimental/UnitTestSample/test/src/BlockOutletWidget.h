#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);

private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_OutletHandle;
};