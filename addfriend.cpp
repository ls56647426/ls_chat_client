#include "addfriend.h"
#include "ui_addfriend.h"

#include <QGraphicsDropShadowEffect>

AddFriend::AddFriend(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AddFriend)
{
	ui->setupUi(this);

	/* 隐藏窗口，只显示控件 */
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	/* 阴影效果初始化 */
	QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(ui->addFriendWidget);
	shadowEffect->setOffset(0, 0);							/* 阴影偏移 */
	shadowEffect->setColor(Qt::black);						/* 阴影颜色 */
	shadowEffect->setBlurRadius(10);						/* 阴影半径 */

	/* 设置阴影效果 */
	ui->addFriendWidget->setGraphicsEffect(shadowEffect);
	delete shadowEffect;

	/* 初始化在编辑验证信息界面 */
	status = AddFriendEnum::SendInfo;
	ui->stackedWidget->setCurrentIndex(0);
}

AddFriend::~AddFriend()
{
	delete ui;
}

/* 下一步 */
void AddFriend::on_nextBtn_clicked()
{
	switch (status)
	{
	case AddFriendEnum::SendInfo:		/* 发送验证消息 */
		/* 获取要验证消息内容 */
		checkInfo = ui->infoTextEdit->placeholderText();

		/* 编辑完成，开始发送加好友请求 */
		/* ... */

		/* 显示结果 */
		status = AddFriendEnum::End;
		ui->nextBtn->hide();
		ui->stackedWidget->setCurrentIndex(status);
		break;
/*	case AddFriendEnum::SetInfo:
		break;*/
	default:
		break;
	}
}

User AddFriend::getUser() const
{
	return user;
}

void AddFriend::setUser(const User &value)
{
	user = value;

	/* 显示信息 */
	ui->usernameBtn->setText(QString(user.getUsername().data()));
}
