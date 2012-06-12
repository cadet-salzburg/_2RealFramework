#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockUnitTestWidget : public QWidget
{
Q_OBJECT

public:
	BlockUnitTestWidget::BlockUnitTestWidget();
	void receiveData(std::list<_2Real::OutputData> data);

private slots:
	void start();

private:
	QWidget			m_ButtonGroup;
	QHBoxLayout		m_ButtonLayout;
	QPushButton*	m_pStartButton;
	QPushButton*	m_pStopButton;

};