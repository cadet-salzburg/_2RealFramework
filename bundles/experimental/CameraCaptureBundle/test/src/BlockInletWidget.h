#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockInletWidget : public QGroupBox
{
Q_OBJECT

public slots:
	void setValue(double value);

public:
	BlockInletWidget::BlockInletWidget(){};
	BlockInletWidget::BlockInletWidget(_2Real::System* system, _2Real::BlockIdentifier blockId, std::string strInletName, QWidget *parent=0);

private:
	QDoubleSpinBox* m_ValueWidget;
	_2Real::System* m_pSystem;
	_2Real::BlockIdentifier m_BlockId;
	std::string m_strInletName;

};