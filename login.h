#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QLineEdit>
#include <QCache>

#include "pojo/User.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
	Q_OBJECT

public:
	Login(QWidget *parent = nullptr);
	~Login();

private slots:
	/* 登录 */
	void on_loginBtn_clicked();
	/* 选择注册账号 */
	void on_toRegisterBtn_clicked();
	/* 选择修改密码 */
	void on_toAlterBtn_clicked();
	/* 选择找回密码 */
	void on_toFindBtn_clicked();
	/* 返回登录主界面 */
	void returnLoginBtn();
	/* 注册用户 */
	void on_registerBtn_clicked();
	/* 修改密码 */
	void on_alterBtn_clicked();
	/* 找回密码 */
	void on_findBtn_clicked();
	/* 记住密码 */
	void on_remeberCheckBox_stateChanged(int arg1);
	/* 自动登录 */
	void on_autoLoginCheckBox_stateChanged(int arg1);
	/* 用户名自动提示 */
	void on_userNameLineEdit_textChanged(const QString &arg1);

signals:
	/* 显示龙少聊天室窗口 */
	void showLSChat(User user);

private:
	/* 主界面初始化 */
	void homeInit();
	/* 读取本地缓存 */
	void readLocalCache();
	/* 连接服务器 */
	void libeventInit();

private:
	Ui::Login *ui;
	User me;

	/* 鼠标事件重载 - 所需变量 */
	bool bPressFlag;
	QPoint beginDrag;

	static QCache<QString, User> userCache;

protected:
	/* 鼠标事件重载 - 按下事件 */
	void mousePressEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 释放事件 */
	void mouseReleaseEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 移动事件 */
	void mouseMoveEvent(QMouseEvent *event);
};
#endif // LOGIN_H
