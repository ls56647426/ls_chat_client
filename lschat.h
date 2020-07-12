#ifndef LSCHAT_H
#define LSCHAT_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSplitter>
#include <QPushButton>
#include <QToolBar>
#include <QMutex>
#include <QTreeWidgetItem>

#include "find.h"
#include "pojo/User.h"
#include "pojo/Group.h"
#include "pojo/Friend.h"
#include "include/client.h"
#include "form/userinfowidget.h"

namespace Ui {
class LSChat;
}

/* 单例模式 */
class LSChat : public QWidget
{
	Q_OBJECT

public:
	/* 单例模式：双检锁，获取聊天室对象 */
	static LSChat &getInstance();
	/* 获取当前在线用户信息 */
	static User getMe();
	/* 显示 好友/群 列表 */
	static void showList();

private slots:
	/* 发送消息 */
	void on_sendMsgBtn_clicked();
	/* 界面切换按钮 */
	void changePageBtn();
	/* 动作组事件 */
	void actionsTriggered();
	/* 菜单按钮事件 */
	void toolBtnClicked();
	/* 我的设置 */
	void on_myBtn_clicked();
	/* 单击展示完整用户信息 */
	void on_friTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
	explicit LSChat(QWidget *parent = nullptr);
	~LSChat();
	/* 主界面初始化 */
	void homeInit();
	/* 聊天会话界面初始化 */
	void sessionPageInit();
	/* 好友/群 列表界面初始化 */
	void listPageInit();

private:
	Ui::LSChat *ui;
	Group group;
	User me, you;
	list<User> friUserList;
	list<Group> grpList;

	/* 要弹出的窗口 */
	Find *findWidget;
	UserInfoWidget *userInfoWidget;

	/* 界面布局 */
	/* 聊天会话界面 */
	QSplitter *sessionSplitter;
	QSplitter *chatShowSplitter;
	QSplitter *chatEditSplitter;
	QAction *enterSendMsgAction;
	QAction *ctrlEnterSendMsgAction;
	/* 好友/群 列表界面 */
	QSplitter *listSplitter;
	QSplitter *listShowSplitter;
	QAction *addListAction;
	QAction *createListAction;

	/* 鼠标事件重载 - 所需变量 */
	bool bPressFlag;		/* 鼠标左键是否按下 */
	QPoint beginDrag;		/* 鼠标左键按下的起始坐标 */

	/* 发送消息方式(Enter - Ctrl+Enter) */
	bool ctrlEnter;

	/* 多线程互斥 */
	static QMutex mutex;
	/* 单例模式：懒汉式 */
	static LSChat *instance;

protected:
	/* 鼠标事件重载 - 按下事件 */
	void mousePressEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 释放事件 */
	void mouseReleaseEvent(QMouseEvent *event);
	/* 鼠标事件重载 - 移动事件 */
	void mouseMoveEvent(QMouseEvent *event);
	/* 尺寸变化事件重载 */
	void resizeEvent(QResizeEvent *event);
	/* 事件过滤器 */
	bool eventFilter(QObject *target, QEvent *event);
};

#endif // LSCHAT_H
