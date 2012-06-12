#ifndef CAMERACAPTURETEST_H
#define CAMERACAPTURETEST_H

#include <QtGui>


class CameraCaptureTest : public QMainWindow
{
	Q_OBJECT

public:
	CameraCaptureTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CameraCaptureTest();

private:
	QWidget			m_ButtonGroup;
	QHBoxLayout		m_ButtonLayout;
	QPushButton*	m_pStartButton;
	QPushButton*	m_pStopButton;
	
};

#endif // CAMERACAPTURETEST_H
