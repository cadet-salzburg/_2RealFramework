#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::System* system, _2Real::BlockIdentifier blockId, std::string strOutletName, QWidget *parent) : QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( strOutletName )) );
	m_ValueLabel = new QLabel(QString::fromStdString( "" ));
	layout->addWidget( m_ValueLabel );
	setLayout( layout );
	system->registerToOutletData( blockId, strOutletName, *this, &BlockOutletWidget::receiveData );
}


void BlockOutletWidget::receiveData(_2Real::OutputData& data)
{
	m_ValueLabel->setText(QString::fromStdString(data.getDataAsString()));
}
