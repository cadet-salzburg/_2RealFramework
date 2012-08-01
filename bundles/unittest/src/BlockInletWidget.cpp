#include "BlockInletWidget.h"
#include "QPointWidget.h"

#include <iostream>

#ifdef _UNIX
    #include <float.h>  //for DBL_MAX
#endif

using namespace std;
using namespace _2Real;

// sorry for all the codeduplication but QT doesn't support templated signal/slot concept which sucks
BlockInletWidget::BlockInletWidget(_2Real::app::InletHandle& inletHandle, QWidget *parent)
	: QGroupBox(parent),
	  m_InletHandle ( inletHandle )
{
	try
	{
		QHBoxLayout*	layout = new QHBoxLayout();
		layout->addWidget( new QLabel(QString::fromStdString( m_InletHandle.getName())) );

		// set and init input widget according to inlet's type
		if(m_InletHandle.getTypename() == "bool")
		{
			m_ValueWidget = new QCheckBox();
			dynamic_cast<QCheckBox*>(m_ValueWidget)->setChecked(m_InletHandle.getCurrentInput().getData<bool>());
			connect(m_ValueWidget, SIGNAL(stateChanged(int)), this, SLOT(setBoolValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "unsigned char")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(0);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(UCHAR_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<unsigned char>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setUCharValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "char")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(CHAR_MIN);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(CHAR_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<char>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setCharValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "unsigned short")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(0);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(USHRT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<unsigned short>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setUShortValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "short")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(SHRT_MIN);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(SHRT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<short>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setShortValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		if(m_InletHandle.getTypename() == "unsigned int")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMinimum(0);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(UINT_MAX);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<unsigned int>());
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setUIntValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "int")
		{
			m_ValueWidget = new QSpinBox();
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMinimum(INT_MIN);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setMaximum(INT_MAX);
			dynamic_cast<QSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<int>());
			connect(m_ValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setIntValue(int)));
			layout->addWidget( m_ValueWidget );
		}
		if(m_InletHandle.getTypename() == "unsigned long")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMinimum(0);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(ULONG_MAX);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<unsigned long>());
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setULongValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "long")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMinimum(LONG_MIN);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(LONG_MAX);
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<long>());
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setLongValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "float")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<float>());
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(DBL_MAX);
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setFloatValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "double")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue(m_InletHandle.getCurrentInput().getData<double>());
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(DBL_MAX);
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setDoubleValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "number")
		{
			m_ValueWidget = new QDoubleSpinBox();
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setValue((double)m_InletHandle.getCurrentInput().getData<_2Real::Number>());
			dynamic_cast<QDoubleSpinBox*>(m_ValueWidget)->setMaximum(DBL_MAX);
			connect(m_ValueWidget, SIGNAL(valueChanged(double)), this, SLOT(setNumberValue(double)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "point")
		{
			m_ValueWidget = new QPointWidget(m_InletHandle, this);
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename() == "string")
		{
			m_ValueWidget = new QLineEdit();
			dynamic_cast<QLineEdit*>(m_ValueWidget)->setText(QString::fromStdString(m_InletHandle.getCurrentInput().getData<string>()));
			connect(m_ValueWidget, SIGNAL(textChanged(const QString &)), this, SLOT(setStringValue(const QString &)));
			layout->addWidget( m_ValueWidget );
		}
		else if(m_InletHandle.getTypename().find("vector")!=string::npos || m_InletHandle.getTypename().find("list")!=string::npos)
		{
			// those types aren't supported as they are not easy to enter via a widget and no one wants to enter them I guess
			if(m_InletHandle.getTypename().find("image")!=string::npos || m_InletHandle.getTypename().find("point")!=string::npos ||
				m_InletHandle.getTypename().find("skeleton")!=string::npos || m_InletHandle.getTypename().find("quaternion")!=string::npos)
				return;

			// numeric list and vector types
			m_ValueWidget = new QTextEdit();
			dynamic_cast<QTextEdit*>(m_ValueWidget)->setText(QString::fromStdString(m_InletHandle.getCurrentInput().getDataAsString()));
			connect(m_ValueWidget, SIGNAL(textChanged()), this, SLOT(setNumericVectorListValue()));
			layout->addWidget( m_ValueWidget );
		}

		setLayout( layout );
	}
	catch(Exception& e)
	{
		cout << e.message() << e.what() << std::endl;
	}
}


// Slots
void BlockInletWidget::setBoolValue(int value)
{
	try
	{
		m_InletHandle.setValue<bool>((bool)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setUCharValue(int value)
{
	try
	{
		m_InletHandle.setValue<unsigned char>((unsigned char)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setCharValue(int value)
{
	try
	{
		m_InletHandle.setValue<char>((char)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setUShortValue(int value)
{
	try
	{
		m_InletHandle.setValue<unsigned short>((unsigned short)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setShortValue(int value)
{
	try
	{
		m_InletHandle.setValue<short>((short)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setUIntValue(double value)
{
	try
	{
		m_InletHandle.setValue<unsigned int>((unsigned int)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setIntValue(int value)
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

void BlockInletWidget::setULongValue(double value)
{
	try
	{
		m_InletHandle.setValue<unsigned long>((unsigned long)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setLongValue(double value)
{
	try
	{
		m_InletHandle.setValue<long>((long)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setFloatValue(double value)
{
	try
	{
		m_InletHandle.setValue<float>((float)value);
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setDoubleValue(double value)
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

void BlockInletWidget::setNumberValue(double value)
{
	try
	{
		m_InletHandle.setValue<_2Real::Number>(Number(value));
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setStringValue(const QString & value)
{
	try
	{
		m_InletHandle.setValue<string>(value.toStdString());
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}

void BlockInletWidget::setNumericVectorListValue()
{
	try
	{
		if(m_InletHandle.getTypename() == "unsigned char vector")
		{
			QString str = dynamic_cast<QTextEdit*>(m_ValueWidget)->toPlainText();
			// interpret string with numbers seperated by spaces or ',' as elements of a vector
			std::vector<unsigned char> values;
			QStringList list = str.split(",", QString::SkipEmptyParts);
			for (int i = 0; i < list.count(); i++) 
			{
				values.push_back((unsigned char)list[i].toInt());
			}

			m_InletHandle.setValue<std::vector<unsigned char>>(values);
		}
	}
	catch(_2Real::Exception& e)
	{
		cout << e.message() << " " << e.what() << std::endl;
	}
}