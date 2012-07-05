#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockInletWidget : public QGroupBox
{
Q_OBJECT

public slots:
	void setValue(double value);

public:
	BlockInletWidget::BlockInletWidget(_2Real::app::InletHandle& inletHandle, QWidget *parent=0);

private:
	QDoubleSpinBox*					m_ValueWidget;
	_2Real::app::InletHandle		m_InletHandle;
};