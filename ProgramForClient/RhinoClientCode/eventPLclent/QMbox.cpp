#include "QMbox.h"
#include "Web/WJMainWindow.h"

QMbox::QMbox(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Qt::WindowFlags flag = (Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setWindowFlags(flag);
	setAttribute(Qt::WA_TranslucentBackground, true);
}
QMbox::~QMbox()
{

}

void QMbox::hideBtuno()
{
	ui.pushButton_NO->hide();
}
void QMbox::setcontent(QString content)
{
	ui.label_content->setText(content);
}
void QMbox::SetTextOKBtn(QString OKbutton)
{
	ui.pushButton_OK->setText(OKbutton);
}
void QMbox::SetTextNOBtn(QString NObutton)
{
	ui.pushButton_NO->setText(NObutton);
}
void QMbox::slots_OK()
{
	hide();
}
void QMbox::slots_NO()
{
	//WJMainWindow()->setEvent();
	hide();
}

