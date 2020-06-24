#ifndef LSCHAT_H
#define LSCHAT_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSplitter>
#include <QPushButton>
#include <QToolBar>

#include "find.h"
#include "pojo/User.h"
#include "pojo/Group.h"
#include "pojo/Friend.h"

namespace Ui {
class LSChat;
}

class LSChat : public QWidget
{
	Q_OBJECT

public:
	explicit LSChat(QWidget *parent = nullptr);
	~LSChat();

	void showList();

private slots:
	/* 接收登录窗口传来的数据，并显示本窗口 */
	void run(User user);
	/* 发送消息 */
	void on_sendMsgBtn_clicked();
	/* 界面切换按钮 */
	void changePageBtn();
	/* 动作组事件 */
	void actionsTriggered();
	/* 菜单按钮事件 */
	void toolBtnClicked();

private:
	/* 主界面初始化 */
	void homeInit();
	/* 聊天会话界面初始化 */
	void sessionPageInit();
	/* 好友/群 列表界面初始化 */
	void listPageInit();

private:
	Ui::LSChat *ui;
	User me;
	QList<User> friUserList;
	QList<Group> grpList;

	/* 要弹出的窗口 */
	Find *findWidget;

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
