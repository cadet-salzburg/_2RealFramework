#include "BlockInletWidget.h"

using namespace std;
using namespace _2Real;

BlockInletWidget::BlockInletWidget(_2Real::System* system, _2Real::BlockIdentifier blockId, std::string strInletName, QWidget *parent) : QGroupBox(parent), m_BlockId(blockId), m_strInletName(strInletName)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( strInletName )) );
	
	m_ValueWidget = new QDoubleSpinBox ();
	m_ValueWidget->setSingleStep(1);
	
	connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
	layout->addWidget( m_ValueWidget );

	setLayout( layout );
}

void BlockInletWidget::setValue(double value)
{
	try
	{
		m_pSystem->setValue<double>(m_BlockId, m_strInletName, value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << e.what() << std::endl; 
	}
}