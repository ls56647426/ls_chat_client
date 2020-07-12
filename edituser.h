#ifndef EDITUSER_H
#define EDITUSER_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class EditUser;
}

class EditUser : public QWidget
{
	Q_OBJECT

public:
	explicit EditUser(QWidget *parent = nullptr);
	~EditUser();

private slots:
	/* 关闭：关闭的时候，判断内容有没有发生变化，提示是否保存 */
	void closeSlot();

private:
	Ui::EditUser *ui;

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
};

#endif // EDITUSER_H
