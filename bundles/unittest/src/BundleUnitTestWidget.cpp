#include "BundleUnitTestWidget.h"
#include <iostream>
#include <QComboBox>

using namespace _2Real;
using namespace _2Real::app;
using namespace std;

BundleUnitTestWidget::BundleUnitTestWidget() : m_iInstances(1), m_Engine(Engine::instance())
{
}

BundleUnitTestWidget::BundleUnitTestWidget(std::string strBundleName) : m_iInstances(1), m_Engine(Engine::instance())
{
	setup( strBundleName );
}


BundleUnitTestWidget::~BundleUnitTestWidget()
{

}

void BundleUnitTestWidget::setup(std::string strBundleName)
{
	std::string directory = "../experimental/bin/win/";
	try 
	{
		// load bundles for use in runtime engine
		m_Engine.setBaseDirectory( directory );
		m_BundleHandle = m_Engine.loadBundle( strBundleName );
		BundleInfo bundleInfo = m_BundleHandle.getBundleInfo();
		m_BlockInfos = bundleInfo.getExportedBlocks();

		// setup gui
		QGridLayout *layout = new QGridLayout();

		// combobox for selcetion of block to instance
		QLabel *comboLabel = new QLabel("Blocks");
		layout->addWidget(comboLabel,0,0, Qt::AlignTop);
		QComboBox *comboBox = new QComboBox();
		for(int i=0; i < m_BlockInfos.size(); i++)
		{
			if(m_BlockInfos[i].getName().find("contextblock")==string::npos)
				comboBox->addItem(QString::fromStdString(m_BlockInfos[i].getName()));
		}
		layout->addWidget(comboBox,0,1,Qt::AlignTop);
		m_strBlockName = m_BlockInfos[0].getName();
		connect(comboBox, SIGNAL(activated (QString)), this, SLOT(onSetBlockName(QString)));

		// nr of instance to create
		QLabel *spinBoxLabel = new QLabel("Number of instances");
		layout->addWidget(spinBoxLabel,1,0,Qt::AlignTop);
		QSpinBox *spinBox = new QSpinBox();
		spinBox->setValue(1);
		layout->addWidget(spinBox,1,1,Qt::AlignTop);
		connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSetInstanceValue(int)));

		// create button
		QPushButton *button = new QPushButton("create block instances");
		layout->addWidget(button,2,1,Qt::AlignTop);
		connect(button, SIGNAL(clicked()), this, SLOT(onCreate()));

		setLayout(layout);
		setWindowTitle(QString::fromStdString(strBundleName)+" Unit Test");
		show();
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << " " << e.what() << endl;
	}
}

void BundleUnitTestWidget::shutdown()
{
	m_Engine.clearAll();
}

void BundleUnitTestWidget::onCreate()
{
	for(int i=0; i<m_iInstances; i++)
	{
		BlockUnitTestWidget* tmp = new BlockUnitTestWidget(m_BundleHandle, m_strBlockName);
		//m_BlockTestWidgets.push_back(tmp);	// as the object is deletes itself on close we don't need to manage the created instances here, leave it here if we decide otherwise later
	}
}

void BundleUnitTestWidget::onSetInstanceValue(int iValue)
{
	m_iInstances = iValue;
}

void BundleUnitTestWidget::onSetBlockName(QString strBlockName)
{
	m_strBlockName = strBlockName.toStdString();
}