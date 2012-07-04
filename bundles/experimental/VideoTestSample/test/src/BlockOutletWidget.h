#include "_2RealApplication.h"
#include <QtGui>
#include <list>
#include <QMutex>

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);
	void paintEvent(QPaintEvent *pe)
	{
		std::cout << "Yes!!" << std::endl;
	};
public slots:
		void setImage( unsigned char* img );

signals:
		void sendImage( unsigned char* img );
private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_OutletHandle;
	QMutex							m_Mutex;
	_2Real::app::AppData			m_Data;
};