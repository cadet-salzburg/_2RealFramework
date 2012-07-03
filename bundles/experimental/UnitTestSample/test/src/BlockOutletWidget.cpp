#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent) : m_OutletHandle(outletHandle),  QGroupBox(parent)
{
	QHBoxLayout*	layout = new QHBoxLayout();
	layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

	if(m_OutletHandle.getTypename().find("img")==0)
	{
		m_ValueWidget = new QLabel("");
	}
	else
	{
		m_ValueWidget = new QLabel(QString::fromStdString( "" ));
	}
	
	layout->addWidget( m_ValueWidget );
	setLayout( layout );
	m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );
}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	if(m_OutletHandle.getTypename().find("img_uchar")==0)
	{
	//	unsigned char* ptr = data.getData<ImageT<unsigned char>>().getData();
		
	//	dynamic_cast<QLabel*>(m_ValueWidget)->setPixmap(QPixmap::fromImage(QImage( 
	}
	else if( m_OutletHandle.getTypename() == "double" )
	{
		dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
	}
}
