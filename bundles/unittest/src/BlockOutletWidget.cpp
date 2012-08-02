#include "BlockOutletWidget.h"
#include "QGlSkeletonWidget.h"
#include "QGlTextureImage.h"
#include <iostream>

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent) 
	: m_OutletHandle(imageHandle),
	  QGroupBox(parent)
{
	try
	{
		m_Layout = new QHBoxLayout();
		m_Layout->addWidget( new QLabel(QString::fromStdString( m_OutletHandle.getName() )) );

		if(m_OutletHandle.getTypename().find("skeleton")!=string::npos )
		{
			m_ValueWidget = new QGlSkeletonWidget();
			dynamic_cast<QGlSkeletonWidget*>(m_ValueWidget)->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
			dynamic_cast<QGlSkeletonWidget*>(m_ValueWidget)->setMinimumSize(80, 60);
		}
		else if(m_OutletHandle.getTypename() == "number image")
		{
			m_ValueWidget = new QGlTextureImage();
			dynamic_cast<QGlTextureImage*>(m_ValueWidget)->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
			dynamic_cast<QGlTextureImage*>(m_ValueWidget)->setMinimumSize(80, 60);
		}
		else if(m_OutletHandle.getTypename().find("vector")!=string::npos || m_OutletHandle.getTypename().find("list")!=string::npos)
		{
			m_ValueWidget = new QTextBrowser();
		}
		else
		{
			m_ValueWidget = new QLabel();
		}

		m_Layout->addWidget( m_ValueWidget );
		setLayout( m_Layout );

		// register data callback for _2Real Framework
		m_OutletHandle.registerToNewData( *this, &BlockOutletWidget::receiveData );

		// note that the moc is quite pedantic with regards to namespaces:
		// you must use the full symbol name here & in the function implementations
		// data between threads has to be sent around through signal and slots in QT !!!!
		qRegisterMetaType< _2Real::app::AppData >( "_2Real::app::AppData" );
		connect( this, SIGNAL( sendData( _2Real::app::AppData ) ), this, SLOT( updateData( _2Real::app::AppData ) ) );
	}
	catch(Exception& e)
	{
		cout << e.message() << e.what() << std::endl;
	}
}

void BlockOutletWidget::receiveData(_2Real::app::AppData const& data)
{
	//keep the data around as long as it and it's underlying pointers are needed, it is internally a share_ptr !!!!!
	emit sendData( data );			// since a copy of the incoming data is emitted
									// it does not matter how much time passes 
									// between this and 'updateData':
									// the data will for sure be alive when the pixmap is updated
}

void BlockOutletWidget::updateData(_2Real::app::AppData data) 
{
	try
	{
		if( m_OutletHandle.getTypename() == "number image" )
		{
			Image const& img = data.getData< Image >();
			dynamic_cast<QGlTextureImage*>(m_ValueWidget)->updateTexture( img.getWidth(), img.getHeight(), img.getNumberOfChannels(), img.getImageType(), img.getData() );
		}
		else if(m_OutletHandle.getTypename() == "skeleton")
		{
			Skeleton const& skeleton = data.getData< Skeleton >();
			dynamic_cast<QGlSkeletonWidget*>(m_ValueWidget)->updateSkeleton( skeleton );
		}
		else if(m_OutletHandle.getTypename() == "skeleton vector")
		{
			std::vector<Skeleton> const& skeletons = data.getData< std::vector<Skeleton >>();
			dynamic_cast<QGlSkeletonWidget*>(m_ValueWidget)->updateSkeletons( skeletons );
		}
		else if(m_OutletHandle.getTypename().find("vector")!=string::npos || m_OutletHandle.getTypename().find("list")!=string::npos)
		{
			dynamic_cast<QTextBrowser*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
		}
		else if( m_OutletHandle.getTypename() == "short" || m_OutletHandle.getTypename() == "unsigned short" ||			// handle numeric types all the same and display the number as string '1234'
			m_OutletHandle.getTypename() == "int" || m_OutletHandle.getTypename() == "unsigned int" || 
			m_OutletHandle.getTypename() == "long" || m_OutletHandle.getTypename() == "unsigned long" ||
			m_OutletHandle.getTypename() == "float" || m_OutletHandle.getTypename() == "double" || m_OutletHandle.getTypename() == "number" ||
			m_OutletHandle.getTypename() == "point" || m_OutletHandle.getTypename() == "string")
		{
			dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString(data.getDataAsString()));
		}
		else if( m_OutletHandle.getTypename() == "char")
		{
			stringstream strTmp;
			strTmp << data.getDataAsString() << " : " << data.getData<char>();
			dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString( strTmp.str()));
		}
		else if( m_OutletHandle.getTypename() == "unsigned char")
		{
			stringstream strTmp;
			strTmp << data.getDataAsString() << " : " << data.getData<unsigned char>();
			dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString( strTmp.str()));
		}
		else if( m_OutletHandle.getTypename() == "bool" )
		{
			stringstream strTmp;
			bool bTmp =  data.getData<bool>();
			if(bTmp)
			{
				strTmp << bTmp << " : true";
			}
			else
			{
				strTmp << bTmp << " : false";
			}
			dynamic_cast<QLabel*>(m_ValueWidget)->setText(QString::fromStdString( strTmp.str()));
		}


		update();
	}
	catch(Exception& e)
	{
		cout << e.message() << e.what() << std::endl;
	}
}