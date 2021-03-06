#include "login.h"
#include "ui_login.h"
#include "include/Msg.h"
#include "include/client.h"

#include <QMessageBox>
#include <QToolTip>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include <synchapi.h>
#include <QDebug>
#include <QHostAddress>

QCache<QString, User> Login::userCache;

Login::Login(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Login)
{
	ui->setupUi(this);

	/* 主界面初始化 */
	homeInit();

	/* 读取本地缓存 */
	readLocalCache();

	/* 切换到登录主界面 */
	ui->stackedWidget->setCurrentIndex(0);
}

Login::~Login()
{
	delete ui;
}

/* 登录 */
void Login::on_loginBtn_clicked()
{
	qDebug() << "进入登录槽函数";
	/* 获取用户名和密码内容 */
	QString username = ui->userNameLineEdit->text();
	QString password = ui->pwdLineEdit->text();

	/* 有效性判断 */
	if(username.isEmpty())
	{
		ui->userNameLineEdit->setFocus();
		QToolTip::showText(ui->userNameLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入账号后再登录", this);
		return;
	}
	if(password.isEmpty())
	{
		ui->pwdLineEdit->setFocus();
		QToolTip::showText(ui->pwdLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入密码后再登录", this);
		return;
	}

	/* 用户登录 */
	ui->loginBtn->setText("取    消");
	/* 延时2s，模拟QQ登录 */
	Sleep(2000);
	/* 发送登录验证 */
	User user;
	user.setUsername(username.toStdString());
	user.setPassword(password.toStdString());
	Msg msg;
	msg.setType(MsgType::MSG_LOGIN);
	msg.setSrc(user);
	Client::tcpSendMsg(msg);
	/* 获取验证结果 */
	msg = Client::getMsg();
	/* 查无此人 */
	if(msg.getType() == MsgType::ERRNO_INEXISTENCE)
	{
		ui->loginBtn->setText("登    录");
		QMessageBox::critical(this, "错误：", "账号或密码输入错误", QMessageBox::Ok);
		return;
	}

	/* 登录成功，切换到聊天窗口 */
	/* 加入本地缓存 */
	user = msg.getSrc();
	QSettings settings("E:/qt/202006051029-Chat/Client/cache/userCache.ini", QSettings::IniFormat);
	settings.setValue(QString("login/0/id"), user.getId());
	settings.setValue(QString("login/%1/username").arg(user.getId()), user.getUsername().data());
	settings.setValue(QString("login/%1/password").arg(user.getId()), user.getPassword().data());
	settings.setValue(QString("login/%1/mobile").arg(user.getId()), user.getMobile().data());
	settings.setValue(QString("login/%1/remember").arg(user.getId()), ui->remeberCheckBox->isChecked());
	settings.setValue(QString("login/%1/autoLogin").arg(user.getId()), ui->autoLoginCheckBox->isChecked());
	qDebug() << "缓存写入成功" << settings.value("login/0/id");
	/* 切换到聊天界面 */
	LSChat::getInstance();
	LSChat::showList();
	this->close();
}

/* 切换到注册账号界面 */
void Login::on_toRegisterBtn_clicked()
{
	ui->stackedWidget->setCurrentIndex(1);
}

/* 切换到修改密码界面 */
void Login::on_toAlterBtn_clicked()
{
	ui->stackedWidget->setCurrentIndex(2);
}

/* 切换到找回密码界面 */
void Login::on_toFindBtn_clicked()
{
	ui->stackedWidget->setCurrentIndex(3);
}

/* 返回登录主界面 */
void Login::returnLoginBtn()
{
	ui->stackedWidget->setCurrentIndex(0);
}

/* 注册用户 */
void Login::on_registerBtn_clicked()
{
	/* 获取用户输入内容 */
	QString username = ui->registerUsernamelineEdit->text();
	QString password = ui->registerPwdlineEdit->text();
	QString password2 = ui->registerPwd2lineEdit->text();
	QString mobile = ui->registerMobileLineEdit->text();

	/* 有效性判断 */
	if(username.isEmpty())
	{
		ui->registerUsernamelineEdit->setFocus();
		QToolTip::showText(ui->registerUsernamelineEdit->mapToGlobal(QPoint(0, 0)), "请您输入账号后再注册", this);
		return;
	}
	if(password.isEmpty())
	{
		ui->registerPwdlineEdit->setFocus();
		QToolTip::showText(ui->registerPwdlineEdit->mapToGlobal(QPoint(0, 0)), "请您输入密码后再注册", this);
		return;
	}
	if(password2.isEmpty())
	{
		ui->registerPwd2lineEdit->setFocus();
		QToolTip::showText(ui->registerPwd2lineEdit->mapToGlobal(QPoint(0, 0)), "请您输入确认密码后再注册", this);
		return;
	}
	if(mobile.isEmpty())
	{
		ui->registerMobileLineEdit->setFocus();
		QToolTip::showText(ui->registerMobileLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入手机号后再注册", this);
		return;
	}
	if(password != password2)
	{
		ui->registerPwd2lineEdit->clear();
		ui->registerPwd2lineEdit->setFocus();
		QToolTip::showText(ui->registerPwd2lineEdit->mapToGlobal(QPoint(0, 0)), "确认密码不一致", this);
		return;
	}

	/* 开始注册 */
	User user;
	user.setUsername(username.toStdString());
	user.setPassword(password.toStdString());
	user.setMobile(mobile.toStdString());
	user.setStatus(UserStatus::USER_STAT_OFFLINE);
	Msg msg;
	msg.setType(MsgType::MSG_REGISTER);
	msg.setSrc(user);
	Client::tcpSendMsg(msg);
	msg = Client::getMsg();

	/* 该用户已存在 */
	switch(msg.getType())
	{
	case MsgType::ERRNO_SUCCESS:				/* 注册成功 */
		QMessageBox::information(this, "提示：", "注册成功", QMessageBox::Ok);
		/* 返回登录主界面 */
		ui->stackedWidget->setCurrentIndex(0);
		break;
	case MsgType::ERRNO_ALREADYEXIST:			/* 该账号已存在 */
		QMessageBox::critical(this, "错误：", "该账号已存在", QMessageBox::Ok);
		break;
	case MsgType::ERRNO_MOBILE_ALREADYEXIST:	/* 该手机号已绑定 */
		QMessageBox::critical(this, "错误：", "该手机号已绑定", QMessageBox::Ok);
		break;
	default:
		break;
	}
}

/* 修改密码 */
void Login::on_alterBtn_clicked()
{
	/* 获取用户输入内容 */
	QString username = ui->alterUsernamelineEdit->text();
	QString oldPwd = ui->alterOldPwdlineEdit->text();
	QString mobile = ui->alterMobileLineEdit->text();
	QString newPwd = ui->alterNewPwdlineEdit->text();
	QString newPwd2 = ui->alterNewPwd2lineEdit->text();

	/* 有效性判断 */
	if(username.isEmpty())
	{
		ui->alterUsernamelineEdit->setFocus();
		QToolTip::showText(ui->alterUsernamelineEdit->mapToGlobal(QPoint(0, 0)), "请您输入账号后再修改", this);
		return;
	}
	if(oldPwd.isEmpty())
	{
		ui->alterOldPwdlineEdit->setFocus();
		QToolTip::showText(ui->alterOldPwdlineEdit->mapToGlobal(QPoint(0, 0)), "请您输入旧密码后再修改", this);
		return;
	}
	if(mobile.isEmpty())
	{
		ui->alterMobileLineEdit->setFocus();
		QToolTip::showText(ui->alterMobileLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入手机号后再修改", this);
		return;
	}
	if(newPwd.isEmpty())
	{
		ui->alterNewPwdlineEdit->setFocus();
		QToolTip::showText(ui->alterNewPwdlineEdit->mapToGlobal(QPoint(0, 0)), "请您输入新密码后再修改", this);
		return;
	}
	if(newPwd2.isEmpty())
	{
		ui->alterNewPwd2lineEdit->setFocus();
		QToolTip::showText(ui->alterNewPwd2lineEdit->mapToGlobal(QPoint(0, 0)), "请您输入确认密码后再修改", this);
		return;
	}
	if(newPwd != newPwd2)
	{
		ui->alterNewPwd2lineEdit->clear();
		ui->alterNewPwd2lineEdit->setFocus();
		QToolTip::showText(ui->alterNewPwd2lineEdit->mapToGlobal(QPoint(0, 0)), "确认密码不一致", this);
		return;
	}

	/* 修改密码 */
	User user;
	user.setUsername(username.toStdString());
	user.setPassword(oldPwd.toStdString());
	user.setMobile(mobile.toStdString());
	User dest;
	dest.setPassword(newPwd.toStdString());
	Msg msg;
	msg.setType(MsgType::MSG_ALTER_PWD);
	msg.setSrc(user);
	msg.setDest(dest);
	Client::tcpSendMsg(msg);
	msg = Client::getMsg();
	switch(msg.getType())
	{
	case MsgType::ERRNO_SUCCESS:		/* 修改完成 */
		QMessageBox::information(this, "提示：", "修改成功", QMessageBox::Ok);
		/* 返回登录主界面 */
		ui->stackedWidget->setCurrentIndex(0);
		break;
	case MsgType::ERRNO_INEXISTENCE:	/* 信息错误 */
		QMessageBox::critical(this, "错误：", "信息输入错误", QMessageBox::Ok);
		break;
	default:
		break;
	}
}

/* 找回密码 */
void Login::on_findBtn_clicked()
{
	/* 获取用户输入内容 */
	QString username = ui->findUsernameLineEdit->text();
	QString mobile = ui->findMobileLineEdit->text();
	QString newPwd = ui->findNewPwdLineEdit->text();
	QString newPwd2 = ui->findNewPwd2LineEdit->text();

	/* 有效性判断 */
	if(username.isEmpty())
	{
		ui->findUsernameLineEdit->setFocus();
		QToolTip::showText(ui->findUsernameLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入账号后再找回", this);
		return;
	}
	if(mobile.isEmpty())
	{
		ui->findMobileLineEdit->setFocus();
		QToolTip::showText(ui->findMobileLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入手机号后再找回", this);
		return;
	}
	if(newPwd.isEmpty())
	{
		ui->findNewPwdLineEdit->setFocus();
		QToolTip::showText(ui->findNewPwdLineEdit->mapToGlobal(QPoint(0, 0)), "请您输入新密码后再找回", this);
		return;
	}
	if(newPwd2.isEmpty())
	{
		ui->findNewPwd2LineEdit->setFocus();
		QToolTip::showText(ui->findNewPwd2LineEdit->mapToGlobal(QPoint(0, 0)), "请您输入确认密码后再找回", this);
		return;
	}
	if(newPwd != newPwd2)
	{
		ui->findNewPwd2LineEdit->clear();
		ui->findNewPwd2LineEdit->setFocus();
		QToolTip::showText(ui->findNewPwd2LineEdit->mapToGlobal(QPoint(0, 0)), "确认密码不一致", this);
		return;
	}

	/* 找回密码 */
	User user;
	user.setUsername(username.toStdString());
	user.setMobile(mobile.toStdString());
	User dest;
	dest.setPassword(newPwd.toStdString());
	Msg msg;
	msg.setType(MsgType::MSG_FIND_PWD);
	msg.setSrc(user);
	msg.setDest(dest);
	Client::tcpSendMsg(msg);
	msg = Client::getMsg();
	switch(msg.getType())
	{
	case MsgType::ERRNO_SUCCESS:			/* 找回完成 */
		QMessageBox::information(this, "提示：", "找回成功", QMessageBox::Ok);
		/* 返回登录主界面 */
		ui->stackedWidget->setCurrentIndex(0);
		break;
	case MsgType::ERRNO_INEXISTENCE:		/* 信息错误 */
		QMessageBox::critical(this, "错误：", "信息输入错误", QMessageBox::Ok);
		break;
	default:
		break;
	}
}

/* 鼠标事件重载 - 按下事件 */
void Login::mousePressEvent(QMouseEvent *event)
{
	bPressFlag = true;
	beginDrag = event->pos();
	QWidget::mousePressEvent(event);
}

/* 鼠标事件重载 - 释放事件 */
void Login::mouseReleaseEvent(QMouseEvent *event)
{
	bPressFlag = false;
	QWidget::mouseReleaseEvent(event);
}

/* 鼠标事件重载 - 移动事件 */
void Login::mouseMoveEvent(QMouseEvent *event)
{
	if(bPressFlag)
	{
		QPoint relaPos(QCursor::pos() - beginDrag);
		move(relaPos);
	}
	QWidget::mouseMoveEvent(event);
}

/* 记住密码 */
void Login::on_remeberCheckBox_stateChanged(int arg1)
{
	/* 取消记住密码，同时取消自动登录 */
	if(arg1 == Qt::Unchecked)
	{
		if(ui->autoLoginCheckBox->isChecked())
			ui->autoLoginCheckBox->setChecked(false);
	}
}

/* 自动登录 */
void Login::on_autoLoginCheckBox_stateChanged(int arg1)
{
	/* 自动登录，同时选择记住密码 */
	if(arg1 == Qt::Checked)
	{
		if(!ui->remeberCheckBox->isChecked())
			ui->remeberCheckBox->setChecked(true);
	}
}

/* 输入用户名，自动提示本地已有数据 */
void Login::on_userNameLineEdit_textChanged(const QString &arg1)
{
	qDebug() << arg1;
	/* 获取缓存key列表 */
	QStringList keyList = userCache.keys();
	User *user;
	bool isEmpty = true;

	/* 先清除已经存在的数据，不然的话每次文本变更都会插入数据，最后出现重复数据 */
	userModel->removeRows(0, userModel->rowCount());

	/* 检索数据 */
	foreach(QString key, keyList)
	{
		user = userCache.object(key);
		qDebug() << user->getUsername().data();
		if(arg1 != "" && QString(user->getUsername().data()).indexOf(arg1) == 0)
		{	/* 如果有，自动填写 */
			userModel->insertRow(0);
			userModel->setData(userModel->index(0, 0),
							   user->getUsername().data());
			ui->pwdLineEdit->setText(user->getPassword().data());
			isEmpty = false;
		}
	}

	/* 如果没有响应数据，密码清空 */
	if(isEmpty)
		ui->pwdLineEdit->clear();
}

/* 用户名自动提示：选择 */
void Login::onUserChoosed(const QString &key)
{
	User *user = userCache.object(key);
	ui->userNameLineEdit->setText(user->getUsername().data());
	ui->pwdLineEdit->setText(user->getPassword().data());
}

/* 主界面初始化 */
void Login::homeInit()
{
	/* 主窗口 */
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);                     //隐藏窗口，只显示控件
	setFixedSize(this->size());                                     //禁止拖动窗口大小

	/* 用户名输入框自动提示 */
	userModel = new QStandardItemModel(0, 1, ui->userNameLineEdit);
	userCompleter = new QCompleter(userModel, ui->userNameLineEdit);
	userCompleter->setCompletionMode(QCompleter::InlineCompletion);
	ui->userNameLineEdit->setCompleter(userCompleter);
	connect(userCompleter, SIGNAL(activated(const QString&)), this, SLOT(onUserChoosed(const QString&)));

	/* 连接 */
	connect(ui->registerReturnLoginBtn, SIGNAL(clicked()), this, SLOT(returnLoginBtn()));
	connect(ui->alterReturnLoginBtn, SIGNAL(clicked()), this, SLOT(returnLoginBtn()));
	connect(ui->findReturnLoginBtn, SIGNAL(clicked()), this, SLOT(returnLoginBtn()));
}

/* 读取本地缓存 */
void Login::readLocalCache()
{
	quint32 id = 0;
	User *user;
	QStringList idStrList;
	QStringList keyList;
	QSettings settings("E:/qt/202006051029-Chat/Client/cache/userCache.ini", QSettings::IniFormat);
	bool isRem = false, isAuto = false;
	QString loginIdStr;

	/* 获取login下的所有用户 */
	settings.beginGroup("login");
	idStrList = settings.childGroups();

	/* 遍历每个用户的数据 */
	foreach(QString idStr, idStrList)
	{
		settings.beginGroup(idStr);
		keyList = settings.childKeys();

		/* 遍历该用户的所有数据 */
		foreach(QString key, keyList)
		{
			QString value = settings.value(key).toString();

			/* 上次登录id记录 */
			if(idStr == "0")
				id = value.toUInt();
			/* 其余记录所有用户信息 */
			else
			{	/* 如果已缓存过该用户，则提取该缓存进行修改 */
				if(userCache.contains(idStr))
					user = userCache.object(idStr);
				/* 否则，新建用户并缓存 */
				else user = new User;

				user->setId(idStr.toUInt());
				if(key == "username")
					user->setUsername(value.toStdString());
				else if(key == "password")
					user->setPassword(value.toStdString());
				else if(key == "mobile")
					user->setMobile(value.toStdString());
				else if(key == "remember" && user->getId() == id && value == "true")
				{	/* 记住密码 */
					loginIdStr = idStr;
					isRem = true;
				}
				else if(key == "autoLogin" && value == true)
				{	/* 自动登录 */
					isAuto = true;
				}

				/* 更新缓存 */
				if(!userCache.contains(idStr))
					userCache.insert(idStr, user);
			}
		}
		settings.endGroup();
	}
	settings.endGroup();

	/* 判断是否有 记住密码/自动登录 */
	user = userCache.object(loginIdStr);
	if(isRem)
	{
		ui->userNameLineEdit->setText(user->getUsername().data());
		ui->pwdLineEdit->setText(user->getPassword().data());
		ui->remeberCheckBox->setChecked(true);
	}
	if(isAuto)
	{
		ui->autoLoginCheckBox->setChecked(true);
		on_loginBtn_clicked();
	}
}

