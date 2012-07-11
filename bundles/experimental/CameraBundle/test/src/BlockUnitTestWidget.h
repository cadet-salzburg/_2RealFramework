#include "_2RealApplication.h"
#include "_2RealDatatypes.h"
#include <QtGui>
#include <list>

class BlockUnitTestWidget : public QWidget
{
Q_OBJECT

public:
	BlockUnitTestWidget::BlockUnitTestWidget();
	BlockUnitTestWidget::BlockUnitTestWidget(_2Real::app::BundleHandle bundleHandle, std::string blockName);
	void setup(_2Real::app::BundleHandle bundleHandle, std::string blockName);
	void shutdown();
	void receiveData(std::list< _2Real::app::AppData > const& data);
	
private slots:
	void onStart();
	void onStop();
	void onSingleStep();
	void onStopFinished();
	void stopBlock();
	void setFpsValue(double value);

private:
	void setupGui();
	QGroupBox* createButtonWidgets();
	QGroupBox* createInletWidgets();
	QGroupBox* createOutletWidgets();

	//_2Real
	_2Real::app::BlockHandle    m_CameraBlockHandle;

	// buttons for starting and stoppping (stopping is called asynchronously via the QFuture Mechanism)
	QPushButton*				m_pStartButton;
	QPushButton*				m_pStopButton;
	QPushButton*                m_pSingleStepButton;
	QDoubleSpinBox*				m_pFpsSpinBox;
	QFuture<void>				m_Future;
	QFutureWatcher<void>		m_FutureWatcher;
	double						m_fDefaultFps;
};