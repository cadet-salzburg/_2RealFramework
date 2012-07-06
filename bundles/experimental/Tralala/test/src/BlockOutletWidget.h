#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QGroupBox
{

Q_OBJECT

public:

	BlockOutletWidget::BlockOutletWidget (_2Real::app::OutletHandle &outletHandle, QWidget *parent = nullptr );
	void receiveData(_2Real::app::AppData const& data);

protected:

	void	paintEvent( QPaintEvent *event );

private:

	//QWidget*						m_ValueWidget;
	QImage							m_QImage;
	QPixmap							m_QPixmap;
	QMutex							m_QPixmapAccess;
	//QMutex						m_QStringAccess;

	_2Real::app::OutletHandle		m_OutletHandle;
	_2Real::app::AppData			m_AppData;
	_2Real::ImageT< unsigned char >	m_2RealImage;


public slots:

	void updatePixmap(const QImage &img);
	//void updateString(const QString& str);

signals:

	void sendPixmap(const QImage &img);
	//void sendString(const QString &str);

};