#include "_2RealApplication.h"
#include "_2RealDatatypes.h"
#include <QtGui>
#include <list>

class BlockUnitTestWidget : public QWidget
{
Q_OBJECT

public:
	BlockUnitTestWidget::BlockUnitTestWidget();
	BlockUnitTestWidget::BlockUnitTestWidget(std::string bundleName, std::string blockName);
	void setup(std::string bundleName, std::string blockName);
	void shutdown();
	void receiveData(std::list< _2Real::app::AppData > const& data);
	
private slots:
	void onStart();
	void onStop();
	void onStopFinished();
	void stopBlock();

private:
	void setupGui();
	QGroupBox* createButtonWidgets();
	QGroupBox* createInletWidgets();
	QGroupBox* createOutletWidgets();
	void test();

	//_2Real
	_2Real::app::BlockHandle    m_CameraBlockHandle;

	// buttons for starting and stoppping (stopping is called asynchronously via the QFuture Mechanism)
	QPushButton*				m_pStartButton;
	QPushButton*				m_pStopButton;
	QFuture<void>				m_Future;
	QFutureWatcher<void>		m_FutureWatcher;
};