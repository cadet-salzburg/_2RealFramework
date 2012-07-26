#include "QPointWidget.h"
#include <iostream>

QPointWidget::QPointWidget(_2Real::app::InletHandle& inlet, QWidget *parent)
	: m_InletHandle(inlet), QFrame(parent)
{
	m_Layout = new QHBoxLayout();

	m_XWidget = new QDoubleSpinBox();
	m_YWidget = new QDoubleSpinBox();
	m_ZWidget = new QDoubleSpinBox();
	m_LabelWidget = new QLineEdit();
	m_IdWidget = new QSpinBox();

	m_Layout->addWidget( m_XWidget );
	m_Layout->addWidget( m_YWidget );
	m_Layout->addWidget( m_ZWidget );
	m_Layout->addWidget( m_LabelWidget );
	m_Layout->addWidget( m_IdWidget );
	setLayout( m_Layout );

	connect(m_XWidget, SIGNAL(valueChanged(double)), this, SLOT(setX(double)));
	connect(m_YWidget, SIGNAL(valueChanged(double)), this, SLOT(setY(double)));
	connect(m_ZWidget, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
	connect(m_LabelWidget, SIGNAL(textChanged(const QString &)), this, SLOT(setLabel(const QString &)));
	connect(m_IdWidget, SIGNAL(valueChanged(int)), this, SLOT(setId(int)));
}

QPointWidget::~QPointWidget()
{
}

void QPointWidget::setX(double value)
{
	try
	{
		_2Real::Point point = m_InletHandle.getCurrentInput().getData<_2Real::Point>();
		point.setX(_2Real::Number(value));
		m_InletHandle.setValue<_2Real::Point>( point );
	}
	catch(_2Real::Exception& e)
	{
		std::cout << e.message() << " " << e.what() << std::endl;
	}
}

void QPointWidget::setY(double value)
{
	try
	{
		_2Real::Point point = m_InletHandle.getCurrentInput().getData<_2Real::Point>();
		point.setY(_2Real::Number(value));
		m_InletHandle.setValue<_2Real::Point>( point );
	}
	catch(_2Real::Exception& e)
	{
		std::cout << e.message() << " " << e.what() << std::endl;
	}
}

void QPointWidget::setZ(double value)
{
	try
	{
		_2Real::Point point = m_InletHandle.getCurrentInput().getData<_2Real::Point>();
		point.setZ(_2Real::Number(value));
		m_InletHandle.setValue<_2Real::Point>( point );
	}
	catch(_2Real::Exception& e)
	{
		std::cout << e.message() << " " << e.what() << std::endl;
	}
}

void QPointWidget::setId(int value)
{
	try
	{
		_2Real::Point point = m_InletHandle.getCurrentInput().getData<_2Real::Point>();
		point.setId( value );
		m_InletHandle.setValue<_2Real::Point>( point );
	}
	catch(_2Real::Exception& e)
	{
		std::cout << e.message() << " " << e.what() << std::endl;
	}
}

void QPointWidget::setLabel(const QString & value)
{
	try
	{
		_2Real::Point point = m_InletHandle.getCurrentInput().getData<_2Real::Point>();
		point.setLabel(value.toStdString());
		m_InletHandle.setValue<_2Real::Point>( point );
	}
	catch(_2Real::Exception& e)
	{
		std::cout << e.message() << " " << e.what() << std::endl;
	}
}
