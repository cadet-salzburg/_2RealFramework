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
	// load and try to find in defined directories
	bool bIsLoaded = loadBundle(strBundleName);

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
	if(m_BlockInfos.size()>0)
	{
		m_strBlockName = m_BlockInfos[0].getName();
	}
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
	button->setDisabled(!bIsLoaded);
	layout->addWidget(button,2,1,Qt::AlignTop);
	connect(button, SIGNAL(clicked()), this, SLOT(onCreate()));

	setLayout(layout);
	setWindowTitle(QString::fromStdString(strBundleName)+" Unit Test");
	
	show();
}

bool BundleUnitTestWidget::loadBundle(std::string strFile)
{
	std::vector<std::string> directories;
	
	directories.push_back("../../bin/win/");
	directories.push_back("../../experimental/bin/win/");
	directories.push_back("../../core/bin/win/");
	directories.push_back("../../tutorial/bin/win/");

	QString curDir = QDir::currentPath();

	// probe if bundle exists in the used directories
	int iDirIndex = -1;
	for(int i=0; i<directories.size(); i++)
	{
		QDir bundlesDirectory(QString::fromStdString(directories[i]));
		QStringList list = bundlesDirectory.entryList(QStringList(QString::fromStdString(strFile + "*.*")));

		if(list.size()>0)
		{
			iDirIndex = i;
			break;
		}
	}

	if(iDirIndex<0)
	{
		return false;
	}

	try 
	{
		// load bundles for use in runtime engine
		m_Engine.setBaseDirectory( directories[iDirIndex] );
		m_BundleHandle = m_Engine.loadBundle( strFile );
	
		BundleInfo bundleInfo = m_BundleHandle.getBundleInfo();
		m_BlockInfos = bundleInfo.getExportedBlocks();
		return true;
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << " " << e.what() << endl;
		return false;
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