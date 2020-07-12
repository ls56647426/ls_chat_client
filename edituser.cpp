#include "edituser.h"
#include "ui_edituser.h"

#include <QGraphicsDropShadowEffect>

EditUser::EditUser(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EditUser)
{
	ui->setupUi(this);

	/* 主窗口 */
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);                     //隐藏窗口，只显示控件
	setFixedSize(this->size());                                     //禁止拖动窗口大小

	/* 阴影效果初始化 */
	QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(ui->editUserWidget);
	shadowEffect->setOffset(0, 0);							/* 阴影偏移 */
	shadowEffect->setColor(Qt::black);						/* 阴影颜色 */
	shadowEffect->setBlurRadius(10);						/* 阴影半径 */

	/* 设置阴影效果 */
	ui->editUserWidget->setGraphicsEffect(shadowEffect);
//	delete shadowEffect;

	/* connect */
	connect(ui->closeBtn1, SIGNAL(clicked(bool)), this, SLOT(closeSlot()));
	connect(ui->closeBtn2, SIGNAL(clicked(bool)), this, SLOT(closeSlot()));
}

EditUser::~EditUser()
{
	delete ui;
}

/* 关闭：关闭的时候，判断内容有没有发生变化，提示是否保存 */
void EditUser::closeSlot()
{
	close();
}

/* 鼠标事件重载 - 按下事件 */
void EditUser::mousePressEvent(QMouseEvent *event)
{
	bPressFlag = true;
	beginDrag = event->pos();
	QWidget::mousePressEvent(event);
}

/* 鼠标事件重载 - 释放事件 */
void EditUser::mouseReleaseEvent(QMouseEvent *event)
{
	bPressFlag = false;
	QWidget::mouseReleaseEvent(event);
}

/* 鼠标事件重载 - 移动事件 */
void EditUser::mouseMoveEvent(QMouseEvent *event)
{
	if(bPressFlag)
	{
		QPoint relaPos(QCursor::pos() - beginDrag);
		move(relaPos);
	}
	QWidget::mouseMoveEvent(event);
}
