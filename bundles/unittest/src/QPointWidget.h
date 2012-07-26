#ifndef QPOINTWIDGET_H
#define QPOINTWIDGET_H

#include "_2RealApplication.h"

#include <QFrame>
#include <QSpinbox>
#include <QDoubleSpinbox>
#include <QLineEdit>
#include <QHBoxLayout>


class QPointWidget : public QFrame
{
	Q_OBJECT

public slots:
	void setX(double value);
	void setY(double value);
	void setZ(double value);
	void setId(int value);
	void setLabel(const QString & value);

public:
	QPointWidget(_2Real::app::InletHandle& inlet, QWidget *parent=0);
	~QPointWidget();

private:
	QLayout*			m_Layout;
	QDoubleSpinBox*		m_XWidget;
	QDoubleSpinBox*		m_YWidget;
	QDoubleSpinBox*		m_ZWidget;
	QLineEdit*			m_LabelWidget;
	QSpinBox*			m_IdWidget;

	_2Real::app::InletHandle&	 m_InletHandle; 
};

#endif // QPOINTWIDGET_H
