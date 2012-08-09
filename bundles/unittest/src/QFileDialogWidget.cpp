#include "QFileDialogWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFiledialog>

QFileDialogWidget::QFileDialogWidget(_2Real::app::InletHandle& inlet, QWidget *parent)
	: m_InletHandle(inlet), QFrame(parent)
{
	m_Layout = new QHBoxLayout();

	m_PathEdit = new QLineEdit();
	QPushButton* button = new QPushButton();
	button->setText("...");
	m_Layout->addWidget( m_PathEdit );
	m_Layout->addWidget( button );
	setLayout( m_Layout );

	connect(m_PathEdit, SIGNAL(returnPressed()), this, SLOT(setPath()));
	connect(button, SIGNAL(clicked()), this, SLOT(openFile()));
}

QFileDialogWidget::~QFileDialogWidget()
{
}

void QFileDialogWidget::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
	m_PathEdit->setText(fileName);
	_2Real::FilePath path(fileName.toStdString());
	m_InletHandle.setValue<_2Real::FilePath>(path);
}

void QFileDialogWidget::setPath()
{
	QString fileName = m_PathEdit->text();
	_2Real::FilePath path(fileName.toStdString());
	m_InletHandle.setValue<_2Real::FilePath>(path);
}