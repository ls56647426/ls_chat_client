#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include "edituser.h"
#include "pojo/User.h"

#include <QWidget>

namespace Ui {
class UserInfoWidget;
}

class UserInfoWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UserInfoWidget(QWidget *parent = nullptr);
	~UserInfoWidget();

	/* 设置用户数据 */
	void setData(const User &user);

	User getUser() const;

private slots:
	/* 编辑个人信息 */
	void on_editUserBtn_clicked();
	/* 修改头像 */
	void on_changeHeadBtn_clicked();

private:
	/* 根据月份和日期获取星座 */
	QString getAstro(int month, int day);

private:
	Ui::UserInfoWidget *ui;

	EditUser *editUser;
	User user;
};

#endif // USERINFOWIDGET_H
