#include "CameraCaptureTest.h"

CameraCaptureTest::CameraCaptureTest(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	m_pStartButton = new QPushButton(tr("&Start"));
	m_pStopButton = new QPushButton(tr("Stop"));

		
	m_ButtonLayout.setAlignment(Qt::AlignBottom);
	m_ButtonLayout.addWidget(m_pStartButton);
    m_ButtonLayout.addWidget(m_pStopButton);
	
	m_ButtonGroup.setLayout(&m_ButtonLayout);
	
	setCentralWidget(&m_ButtonGroup);
	m_ButtonGroup.show();

}

CameraCaptureTest::~CameraCaptureTest()
{
}

