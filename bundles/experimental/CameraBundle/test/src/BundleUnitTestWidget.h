#ifndef BUNDLEUNITTESTWIDGET_H
#define BUNDLEUNITTESTWIDGET_H

#include <QObject>
#include "_2RealApplication.h"
#include "BlockUnitTestWidget.h"

class BundleUnitTestWidget : public QWidget
{
	Q_OBJECT

public:
	BundleUnitTestWidget::BundleUnitTestWidget();
	BundleUnitTestWidget::BundleUnitTestWidget(std::string strBundleName);
	~BundleUnitTestWidget();

	void setup(std::string strBundleName);
	void shutdown();

private:
	std::vector<BlockUnitTestWidget*>		m_BlockTestWidgets;
	_2Real::app::Engine&					m_Engine;
	_2Real::app::BundleHandle				m_BundleHandle;
	_2Real::app::BundleInfo::BlockInfos     m_BlockInfos;
	int										m_iInstances;
	std::string								m_strBlockName;

private slots:
	void onCreate();
	void onSetInstanceValue(int iValue);
	void onSetBlockName(QString stBlockName);
};

#endif // BUNDLEUNITTESTWIDGET_H
