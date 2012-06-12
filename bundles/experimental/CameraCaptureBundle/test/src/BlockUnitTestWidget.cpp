#include "BlockUnitTestWidget.h"

using _2Real::OutputData;

BlockUnitTestWidget::BlockUnitTestWidget()
{
	m_pStartButton = new QPushButton(tr("&Start"));
	connect(m_pStartButton, SIGNAL(clicked()), this, SLOT(start()));

	m_pStopButton = new QPushButton(tr("Stop"));

		
	m_ButtonLayout.setAlignment(Qt::AlignBottom);
	m_ButtonLayout.addWidget(m_pStartButton);
    m_ButtonLayout.addWidget(m_pStopButton);
	
	m_ButtonGroup.setLayout(&m_ButtonLayout);
	
	m_ButtonGroup.show();

}

void BlockUnitTestWidget::receiveData(std::list<OutputData> data)
{
	printf("received\n");
}

void BlockUnitTestWidget::start()
{ 
	//QtConcurrent::run()
	printf("clicked\n");
}