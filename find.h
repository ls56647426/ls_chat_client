#ifndef FIND_H
#define FIND_H

#include <QWidget>
#include <QMouseEvent>

#include "form/searchlineedit.h"
#include "include/Msg.h"

namespace Ui {
class Find;
}

class Find : public QWidget
{
	Q_OBJECT

public:
	explicit Find(QWidget *parent = nullptr);
	~Find();

private:
	Ui::Find *ui;
	SearchLineEdit *searchLineEdit;

	/* 鼠标事件重载 - 所需变量 */
	bool bPressFlag;		/* 鼠标左键是否按下 */
	QPoint beginDrag;		/* 鼠标左键按下的起始坐标 */

protected:
	/* 鼠标事件重载 - 按下事件 */
	void mousePressEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 释放事件 */
	void mouseReleaseEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 移动事件 */
	void mouseMoveEvent(QMouseEvent *event);
private slots:
	void on_findBtn_clicked();
};

#endif // FIND_H
