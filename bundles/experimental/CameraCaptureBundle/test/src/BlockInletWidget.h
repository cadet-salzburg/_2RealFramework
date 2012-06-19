#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockInletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockInletWidget::BlockInletWidget(){};
	BlockInletWidget::BlockInletWidget(_2Real::BlockIdentifier blockId, std::string strOutletName, QWidget *parent=0);

private:
	QDoubleSpinBox * m_ValueWidget;
};