#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(){};
	BlockOutletWidget::BlockOutletWidget(_2Real::BlockIdentifier blockId, std::string strOutletName, QWidget *parent=0);

	void receiveData(_2Real::OutputData& data);

private:
	QLabel*		m_ValueLabel;
};