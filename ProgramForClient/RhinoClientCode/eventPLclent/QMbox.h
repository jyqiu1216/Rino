#ifndef QMBOX_H
#define QMBOX_H

#include <QDialog>
#include "ui_QMbox.h"

class QMbox : public QDialog
{
	Q_OBJECT

public:
	QMbox(QWidget *parent = 0);
	~QMbox();
	// 设置显示内容
	void setcontent(QString content);
	// 设置OK按钮内容
	void SetTextOKBtn(QString OKbutton);
	// 设置NO按钮内容
	void SetTextNOBtn(QString NObutton);
	// 隐藏NO按钮
	void hideBtuno();
signals:
	void signal_NOOK(QString);
private slots:
	void slots_OK();
	void slots_NO();

private:
	Ui::QMbox ui;
	bool isToConfirmOrCancel;
};

#endif // QMBOX_H
