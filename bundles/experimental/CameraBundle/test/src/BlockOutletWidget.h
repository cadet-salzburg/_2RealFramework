#include "_2RealApplication.h"
#include <QtGui>
#include <list>
#include "qmutex.h"
#include "_2RealDatatypes.h"

class BlockOutletWidget : public QGroupBox
{
Q_OBJECT

public:
	BlockOutletWidget::BlockOutletWidget(_2Real::app::OutletHandle& imageHandle, QWidget *parent=0);

	void receiveData(_2Real::app::AppData const& data);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	QWidget*						m_ValueWidget;
	_2Real::app::OutletHandle		m_OutletHandle;
	_2Real::app::AppData			m_AppData;
	QImage							m_Img;
	QPixmap							m_Pixmap;
	QMutex							m_QPixmapAccess;
	int								m_Width;
	int								m_Height;
	QHBoxLayout*					m_Layout;

public slots:
	void updatePixmap(const QImage &img);
	void updateString(const QString& str);

signals:
	void sendPixmap(const QImage &img);
	void sendString(const QString &str);
};