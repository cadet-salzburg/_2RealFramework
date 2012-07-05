#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& outletHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_OutletHandle;
	Poco::Mutex						m_Mutex;
	_2Real::app::AppData			m_AppData;	
	QImage							m_Img;
	QPixmap							m_Pixmap;

public slots:
	void updatePixmap(const QImage &img);

signals:
	void sendPixmap(const QImage &img);
};