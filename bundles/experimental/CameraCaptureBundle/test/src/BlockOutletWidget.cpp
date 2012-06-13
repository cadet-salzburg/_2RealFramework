#include "BlockOutletWidget.h"

using namespace std;
using namespace _2Real;

BlockOutletWidget::BlockOutletWidget(_2Real::BlockIdentifier blockId, std::string strOutletName, QWidget *parent) : QWidget(parent)
{
	QLabel* outletDescription = new QLabel(QString::fromStdString( strOutletName ));
}
