#include "BlockUnitTestWidget.h"

#include "BlockInletWidget.h"
#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::app;


BlockUnitTestWidget::BlockUnitTestWidget()
{
}

BlockUnitTestWidget::BlockUnitTestWidget(std::string bundleName, string blockName)
{
	setup(bundleName, blockName);
}

void BlockUnitTestWidget::setup(std::string bundleName, std::string blockName)
{
	string directory = "../experimental/bin/win/";
	Engine &engine = Engine::instance();

	try 
	{
		// load bundles for use in runtime engine
		engine.setBaseDirectory( directory );
		BundleHandle bundleHandle = engine.loadBundle( "CameraCaptureBundle" );
		BundleInfo const& bundleData = bundleHandle.getBundleInfo();
		
		m_CameraBlockHandle = bundleHandle.createBlockInstance( "CameraCaptureBlock" );
		BlockInfo const& blockData = m_CameraBlockHandle.getBlockInfo();
		m_CameraBlockHandle.setUpdateRate( 60.0 );
		
		// setup
		m_CameraBlockHandle.setup();
		// start
		m_CameraBlockHandle.start();
		// setup callbacks
		m_CameraBlockHandle.registerToNewData( *this, &BlockUnitTestWidget::receiveData );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
	}
	
	setupGui();

}

void BlockUnitTestWidget::shutdown()
{
	Engine::instance().clearAll();
}

void BlockUnitTestWidget::setupGui()
{
	QGridLayout *grid = new QGridLayout;	// all things added with new as long as they belong to a parent are deleted by the parent so no need to care for that

	grid->addWidget(createInletWidgets());
	grid->addWidget(createOutletWidgets());
	grid->addWidget(createButtonWidgets());

	setLayout(grid);
	show();
}

void BlockUnitTestWidget::receiveData(std::list< _2Real::app::AppData > const& data)
{
}

QGroupBox* BlockUnitTestWidget::createButtonWidgets()
{
	m_pStartButton = new QPushButton(tr("Start"));
	m_pStopButton = new QPushButton(tr("Stop"));
	m_pStartButton->setDisabled(true);

	// connect signals
	connect(m_pStartButton, SIGNAL(clicked()), this, SLOT(onStart()));
	connect(m_pStopButton, SIGNAL(clicked()), this, SLOT(onStop()));
	connect(&m_FutureWatcher, SIGNAL(finished()), this, SLOT(onStopFinished()));

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setAlignment(Qt::AlignBottom);
	layout->addWidget(m_pStartButton);
    layout->addWidget(m_pStopButton);
	
	QGroupBox *groupBox = new QGroupBox("Controls");
	groupBox->setLayout(layout);

	return groupBox;
}

QGroupBox* BlockUnitTestWidget::createInletWidgets()
{
	QGroupBox *groupBox = new QGroupBox("Inlets");
	BlockInfo::ParameterInfos inlets = m_CameraBlockHandle.getBlockInfo().getInlets();
	QVBoxLayout* layout = new QVBoxLayout();

	for(auto it = inlets.begin(); it != inlets.end(); it++)
	{
		BlockInletWidget* tmp = new BlockInletWidget(  m_CameraBlockHandle.getInletHandle(it->getName()), this );
		layout->addWidget(tmp);
	}

	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox* BlockUnitTestWidget::createOutletWidgets()
{
	QGroupBox* groupBox = new QGroupBox("Outlets");
	BlockInfo::ParameterInfos outlets = m_CameraBlockHandle.getBlockInfo().getOutlets();
	QVBoxLayout* layout = new QVBoxLayout();

	for(auto it = outlets.begin(); it != outlets.end(); it++)
	{
		BlockOutletWidget* tmp = new BlockOutletWidget( m_CameraBlockHandle.getOutletHandle(it->getName()), this );
		layout->addWidget(tmp);
	}

	groupBox->setLayout(layout);
	return groupBox;
}

void BlockUnitTestWidget::onStart()
{ 
	m_pStartButton->setDisabled(true);
	m_pStopButton->setDisabled(false);
	m_CameraBlockHandle.start();
}

void BlockUnitTestWidget::onStop()
{ 
	m_pStopButton->setDisabled(true);
	m_Future = QtConcurrent::run(this, &BlockUnitTestWidget::stopBlock);
	m_FutureWatcher.setFuture(m_Future);
}

void BlockUnitTestWidget::onStopFinished()
{
	m_pStopButton->setDisabled(true);
	m_pStartButton->setDisabled(false);
}

void BlockUnitTestWidget::stopBlock()
{
	m_CameraBlockHandle.stop();
}