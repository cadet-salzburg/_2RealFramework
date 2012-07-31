#include "_2RealApplication.h"
#include <QtGui>
#include <list>

class BlockInletWidget : public QGroupBox
{
Q_OBJECT

public slots:
	void setBoolValue(int value);
	void setUCharValue(int value);
	void setCharValue(int value);
	void setUShortValue(int value);
	void setShortValue(int value);
	void setUIntValue(double value);
	void setIntValue(int value);
	void setULongValue(double value);
	void setLongValue(double value);
	void setFloatValue(double value);
	void setDoubleValue(double value);
	void setNumberValue(double value);
	void setStringValue(const QString & text);
	void setNumericVectorListValue();
	
public:
	BlockInletWidget(_2Real::app::InletHandle& inletHandle, QWidget *parent=0);

private:

	QWidget*				 m_ValueWidget;
	_2Real::app::InletHandle m_InletHandle;
};
