#include "BlockInletWidget.h"

#include <iostream>

using namespace std;
using namespace _2Real;

BlockInletWidget::BlockInletWidget(_2Real::app::InletHandle& inletHandle, QWidget *parent) 
	: QGroupBox(parent),
	  m_InletHandle ( inletHandle )
{
	try
	{
		QHBoxLayout*	layout = new QHBoxLayout();
		layout->addWidget( new QLabel(QString::fromStdString( m_InletHandle.getName())) );
	
		// set and init input widget according to inlet's type
		if(m_InletHandle.getTypename() == "unsigned int")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(-UINT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(UINT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<unsigned int>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
		}
		else if(m_InletHandle.getTypename() == "int")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(INT_MIN);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(INT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<int>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
		}
		else if(m_InletHandle.getTypename() == "double")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<double>());
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(DBL_MAX);
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
		}
		
		layout->addWidget( m_ValueWidget );
		setLayout( layout );
	}
	catch(Exception& e)
	{
		cout << e.message() << e.what() << std::endl;
	}
}

void BlockInletWidget::setValue(double value)
{
	try
	{
		m_InletHandle.setValue<double>(value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl; 
	}
}

void BlockInletWidget::setValue(int value)
{
	try
	{
		m_InletHandle.setValue<int>(value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl; 
	}
}