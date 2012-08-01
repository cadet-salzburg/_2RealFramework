#ifndef QFILEDIALOGWIDGET_H
#define QFILEDIALOGWIDGET_H

#include "_2RealApplication.h"
#include <QFrame>
#include <QLayout>
#include <QLineedit>

class QFileDialogWidget : public QFrame
{
	Q_OBJECT

public slots:
	void openFile();
	void setPath();

public:
	QFileDialogWidget(_2Real::app::InletHandle& inlet, QWidget *parent=0);
	~QFileDialogWidget();

private:
	QLayout*					m_Layout;
	QLineEdit*					m_PathEdit;
	_2Real::app::InletHandle&	m_InletHandle; 
};

#endif // QFILEDIALOGWIDGET_H
