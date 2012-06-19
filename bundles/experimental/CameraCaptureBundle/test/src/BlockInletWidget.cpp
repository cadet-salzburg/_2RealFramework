#include "BlockInletWidget.h"

using namespace std;
using namespace _2Real;

BlockInletWidget::BlockInletWidget(_2Real::BlockIdentifier blockId, std::string strInletName, QWidget *parent) : QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( strInletName )) );
	
	m_ValueWidget = new QDoubleSpinBox ();
	m_ValueWidget->setSingleStep(1);
	layout->addWidget( m_ValueWidget );
	
	setLayout( layout );
}