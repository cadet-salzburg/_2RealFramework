#include "BlockUnitTestWidget.h"
#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;


#ifndef _DEBUG
	#define shared_library_suffix "_32.dll"
#else
	#define shared_library_suffix "_32d.dll"
#endif


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
	m_p2RealSystem = new _2Real::System(blockName+"_UnitTest");
	m_strBlockName = blockName;

	try 
	{
		// load bundles for use in runtime engine
		engine.setBaseDirectory( directory );
		m_BundleId = engine.load( bundleName.append( shared_library_suffix ) );

		// create certain blocks to a runtime system
		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate( 10.0f );
		m_BlockId = m_p2RealSystem->createBlock( m_BundleId, blockName, fpsTrigger );
		
		// set needed setup parameters for block otherwise set to default

		// start 
		m_p2RealSystem->setup(m_BlockId);

		// setup callbacks
		m_p2RealSystem->registerToNewData( m_BlockId, *this, &BlockUnitTestWidget::receiveData );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
	}

	setupGui();

}

void BlockUnitTestWidget::shutdown()
{
	m_p2RealSystem->clear();
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

void BlockUnitTestWidget::receiveData(std::list<OutputData> data)
{
	printf("received");
}

QGroupBox* BlockUnitTestWidget::createButtonWidgets()
{
	m_pStartButton = new QPushButton(tr("Start"));
	m_pStopButton = new QPushButton(tr("Stop"));
	m_pStopButton->setDisabled(true);

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
	return groupBox;
}

QGroupBox* BlockUnitTestWidget::createOutletWidgets()
{
	QGroupBox* groupBox = new QGroupBox("Outlets");
	ParameterDataMap outlets = Engine::instance().getBlockData(m_BundleId, m_strBlockName).getOutlets();
	QVBoxLayout* layout = new QVBoxLayout();

	for(auto it = outlets.begin(); it != outlets.end(); it++)
	{
		layout->addWidget( new BlockOutletWidget(m_BlockId, it->first, this));
	}

	groupBox->setLayout(layout);
	return groupBox;
}

void BlockUnitTestWidget::onStart()
{ 
	m_pStartButton->setDisabled(true);
	m_pStopButton->setDisabled(false);
}

void BlockUnitTestWidget::onStop()
{ 
	m_pStopButton->setDisabled(true);
	m_Future = QtConcurrent::run(this, &BlockUnitTestWidget::test);
	m_FutureWatcher.setFuture(m_Future);
}

void BlockUnitTestWidget::onStopFinished()
{
	m_pStopButton->setDisabled(true);
	m_pStartButton->setDisabled(false);
	printf("finished\n");
}

void BlockUnitTestWidget::test()
{
	int i=0;
	while(i++<10000)
		printf("kaka");
}