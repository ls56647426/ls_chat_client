#include "userinfowidget.h"
#include "ui_userinfowidget.h"

#include <QDateTime>

UserInfoWidget::UserInfoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UserInfoWidget)
{
	ui->setupUi(this);
}

UserInfoWidget::~UserInfoWidget()
{
	delete ui;
}

/* 设置用户数据 */
void UserInfoWidget::setData(const User &user)
{
	QDateTime curTime = QDateTime::currentDateTime();
	QDateTime birTime = QDateTime::fromString(user.getBirthday().data(), "yyyy-MM-dd hh:mm:ss");

	this->user = user;

	ui->nicknameLabel->setText(user.getNickname().data());
	ui->signLabel->setText(user.getSign().data());
	ui->usernameLabel2->setText(user.getUsername().data());
	ui->nicknameLabel2->setText(user.getNickname().data());
	ui->mobileLabel2->setText(user.getMobile().data());
	ui->emailLabel2->setText(user.getEmail().data());
	ui->professionLabel2->setText(user.getProfession().data());
	ui->ownLabel2->setText(QString("%1 %2 %3").arg(
							   birTime.daysTo(curTime) / 365).arg(
							   user.getSex().data()).arg(
							   getAstro(birTime.date().month(), birTime.date().day())));
	ui->locationLabel2->setText(user.getLocation().data());
}

/* 编辑个人信息 */
void UserInfoWidget::on_editUserBtn_clicked()
{
	/* 弹出编辑个人信息界面 */
	if(editUser == NULL)
		editUser = new EditUser;
	editUser->show();
}

/* 修改头像 */
void UserInfoWidget::on_changeHeadBtn_clicked()
{

}

/* 根据月份和日期获取星座 */
QString UserInfoWidget::getAstro(int month, int day)
{
	/* 星座名称 */
	QStringList astro;
	astro << "摩羯座" << "水瓶座" << "双鱼座" << "白羊座" << "金牛座" <<
			 "双子座" << "巨蟹座" << "狮子座" << "处女座" << "天枰座" <<
			 "天蝎座" << "射手座" << "摩羯座";
	/* 两个星座分割日 */
	int arr[] = {20, 19, 21, 21, 21, 22, 23, 23, 23, 23, 22, 22};

	/* 所查询的日期在分割日之前，索引-1，否则不变 */
	int index = month;
	if(day < arr[month - 1])
		index--;

	/* 返回索引指向的星座 */
	return astro[index];
}

User UserInfoWidget::getUser() const
{
	return user;
}
