#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QWidget
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::BlockIdentifier blockId, std::string strOutletName, QWidget *parent=0);
	void receiveData(std::list<_2Real::OutputData> data);

private:
	std::string m_strDescription;

};