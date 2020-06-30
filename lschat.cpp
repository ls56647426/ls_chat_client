#include "lschat.h"
#include "ui_lschat.h"

#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QLine>
#include <QKeyEvent>
#include <QDebug>
#include <QToolTip>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>

/* 多线程互斥 */
QMutex LSChat::mutex;
/* 单例模式：懒汉式 */
LSChat *LSChat::instance = NULL;

static const QString listShowTopBtnSelectStyle = "QPushButton{"
												 "	font-size: 12px;"
												 "	color: #3B3B3B;"
												 "	border: none;"
												 "}"
												 "QPushButton:hover{"
												 "	color: #3B3B3B;"
												 "}";

static const QString listShowTopBtnUnSelectStyle = "QPushButton{"
												   "	font-size: 12px;"
												   "	color: #858585;"
												   "	border: none;"
												   "}"
												   "QPushButton:hover{"
												   "	color: #3B3B3B;"
												   "}";

static const QString listShowTopLineSelectStyle = "background-color: #4598FF;"
												  "border: none;";

static const QString listShowTopLineUnSelectStyle = "background-color: #FAFAFA;"
													"border: none;";

LSChat::LSChat(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LSChat)
{
	ui->setupUi(this);

	/* 主界面初始化 */
	homeInit();

	/* 聊天会话界面初始化 */
	sessionPageInit();

	/* 好友/群 列表界面初始化 */
	listPageInit();

	/* 切换到聊天会话界面 */
	ui->globalStackedWidget->setCurrentIndex(0);

	/* 切换到好友列表界面 */
	ui->listStackedWidget->setCurrentIndex(0);
	ui->toFriListBtn->setStyleSheet(listShowTopBtnSelectStyle);
	ui->toFriListLine->setStyleSheet(listShowTopLineSelectStyle);

	show();
}

LSChat::~LSChat()
{
	delete ui;
}

/* 单例模式：双检锁，获取聊天室对象 */
LSChat &LSChat::getInstance()
{
	if (!instance)
	{
		QMutexLocker locker(&mutex);
		if (!instance)
			instance = new LSChat();
	}

	return *instance;
}

/* 显示 好友/群 列表 */
void LSChat::showList()
{
	LSChat &chat = getInstance();

	/* 获取好友列表 */
	Msg msg;
	msg.setType(MsgType::COMMAND_GET_FRIEND);
	msg.setSrc(chat.me);
	Client::tcpSendMsg(msg);
	msg = Client::getMsg();
	chat.friUserList = msg.getInfo().getUserListInfo();
	/* 显示到friTreeWidget上 */
	chat.ui->friTreeWidget->clear();
	for(auto it = chat.friUserList.begin(); it != chat.friUserList.end(); it++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(chat.ui->friTreeWidget);
		item->setText(0, it->getNickname().data());
	}

//	/* 获取群列表 */
//	msg.setType(MsgType::COMMAND_GET_GROUP_USER);
//	Client::sendMsg(msg);
//	msg = Client::getMsg();
//	QList<UserGroupMap> ugmList = ugms.findAllByUser(me);
//	grpList.clear();
//	for(QList<UserGroupMap>::iterator it = ugmList.begin(); it != ugmList.end(); it++)
//	{	/* 获取该群实际数据，并写入当前群列表 */
//		grpList.append(*gs.findGroupById(it->getGid()));
//	}

//	/* 显示到grpTreeWidget上 */
//	ui->grpTreeWidget->clear();
//	for(QList<Group>::iterator it = grpList.begin(); it != grpList.end(); it++)
//	{
//		QTreeWidgetItem *item = new QTreeWidgetItem(ui->grpTreeWidget);
//		item->setText(0, it->getName());
//	}
}

/* 获取当前在线用户信息 */
User LSChat::getMe()
{
	LSChat &chat = getInstance();
	return chat.me;
}

/* 鼠标事件重载 - 按下事件 */
void LSChat::mousePressEvent(QMouseEvent *event)
{
	bPressFlag = true;
	beginDrag = event->pos();
	QWidget::mousePressEvent(event);
}

/* 鼠标事件重载 - 释放事件 */
void LSChat::mouseReleaseEvent(QMouseEvent *event)
{
	bPressFlag = false;
	QWidget::mouseReleaseEvent(event);
}

/* 鼠标事件重载 - 移动事件 */
void LSChat::mouseMoveEvent(QMouseEvent *event)
{
	if(bPressFlag)
	{
		QPoint relaPos(QCursor::pos() - beginDrag);
		move(relaPos);
	}
	QWidget::mouseMoveEvent(event);
}

/* 尺寸变化事件重载 */
void LSChat::resizeEvent(QResizeEvent *event)
{
	/* 暂时好像用不上了 */
	move((QApplication::desktop()->width() - width()) / 2,
		 (QApplication::desktop()->height() - height()) / 2);
	QWidget::resizeEvent(event);
}

/* 事件过滤器 */
bool LSChat::eventFilter(QObject *target, QEvent *event)
{
	/* 聊天消息编辑框事件 */
	if(target == ui->chatTextEdit)
	{
		/* 按键事件 */
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent *k = static_cast<QKeyEvent *>(event);
			/* 主键盘回车是Qt::Key_Return - 小键盘回车是Qt::Key_Enter */
			if(k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter)
			{
				/*
				 * Ctrl + Enter || Shift + Enter || Ctrl + Shift + Enter是 换行 功能
				 * Qt::KeypadModifier 是 小键盘
				 */
				if(k->modifiers() == Qt::ControlModifier ||
						k->modifiers() == Qt::ShiftModifier ||
						k->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) ||
						k->modifiers() == (Qt::ControlModifier | Qt::KeypadModifier) ||
						k->modifiers() == (Qt::ShiftModifier | Qt::KeypadModifier) ||
						k->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier |
										   Qt::KeypadModifier))
				{
					if(!ctrlEnter)
					{	/* insert的确不会额外加换行，但是听说内容多了会比append慢 */
						ui->chatTextEdit->textCursor().movePosition(QTextCursor::End);
						ui->chatTextEdit->textCursor().insertText("\n");
						return true;
					}
					else
					{	/* 执行发送消息按钮 */
						on_sendMsgBtn_clicked();
						return true;
					}
				}
				else if(k->modifiers() == Qt::NoModifier ||
						k->modifiers() == Qt::KeypadModifier)
				{
					if(!ctrlEnter)
					{	/* 执行发送消息按钮 */
						on_sendMsgBtn_clicked();
						return true;
					}
					else
					{	/* insert的确不会额外加换行，但是听说内容多了会比append慢 */
						ui->chatTextEdit->textCursor().movePosition(QTextCursor::End);
						ui->chatTextEdit->textCursor().insertText("\n");
						return true;
					}
				}
			}
		}
	}

	return QWidget::eventFilter(target, event);
}

/* 发送消息 */
void LSChat::on_sendMsgBtn_clicked()
{
	LSChat &chat = getInstance();

	/* 获取数据 */
	QString text = ui->chatTextEdit->toPlainText();

	/* 有效性校验 */
	if(text.isEmpty())
	{
		QToolTip::showText(ui->sendMsgBtn->mapToGlobal(QPoint(-120, -40)), "发送内容不能为空，请重新输入", this);
		return;
	}

	MsgInfo info;
	info.setInfo(text.toStdString());
	Msg msg;
	msg.setType(MsgType::MSG_SEND_MSG);
	msg.setSrc(chat.me);
	msg.setInfo(info);
//	Client::udpSendMsg(msg);
//	Client::tcpSendMsg(msg);

	/* 发送完清空输入框 */
	ui->chatTextEdit->clear();
}

/* 主界面初始化 */
void LSChat::homeInit()
{
	/* 隐藏窗口，只显示控件 */
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	/* 阴影效果初始化 */
	QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(ui->LSChatWidget);
	shadowEffect->setOffset(0, 0);							/* 阴影偏移 */
	shadowEffect->setColor(Qt::black);						/* 阴影颜色 */
	shadowEffect->setBlurRadius(10);						/* 阴影半径 */

	/* 设置阴影效果 */
	ui->LSChatWidget->setGraphicsEffect(shadowEffect);
	delete shadowEffect;

	/* 获取登录的用户信息 */
	QSettings settings("E:/qt/202006051029-Chat/Client/cache/userCache.ini", QSettings::IniFormat);
	me.setId(settings.value("login/0/id").toUInt());
	me.setUsername(settings.value(QString("login/%1/username").arg(me.getId())).toString().toStdString());
	me.setPassword(settings.value(QString("login/%1/password").arg(me.getId())).toString().toStdString());
	me.setMobile(settings.value(QString("login/%1/mobile").arg(me.getId())).toString().toStdString());
	me.setStatus(UserStatus::USER_STAT_ONLINE);
}

/* 聊天会话界面初始化 */
void LSChat::sessionPageInit()
{
	/* 添加按钮菜单 */
	enterSendMsgAction = new QAction("按Enter键发送消息");
	connect(enterSendMsgAction, SIGNAL(triggered()), this, SLOT(actionsTriggered()));

	ctrlEnterSendMsgAction = new QAction("按Ctrl+Enter键发送消息");
	connect(ctrlEnterSendMsgAction, SIGNAL(triggered()), this, SLOT(actionsTriggered()));

	QMenu *menu = new QMenu;
	menu->addAction(enterSendMsgAction);
	menu->addAction(ctrlEnterSendMsgAction);

	ui->sendMsgTipBtn->setMenu(menu);

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->setMargin(4);
	hlayout->setSpacing(1);
	hlayout->addWidget(ui->sendMsgLabel);
	hlayout->addWidget(ui->sendMsgTipBtn);

	ui->sendMsgBtn->setLayout(hlayout);

	/* 按钮关联 */
	connect(ui->toSessionBtn, SIGNAL(clicked()), this, SLOT(changePageBtn()));
	connect(ui->toListBtn, SIGNAL(clicked()), this, SLOT(changePageBtn()));
	connect(ui->toYunBtn, SIGNAL(clicked()), this, SLOT(changePageBtn()));
	connect(ui->toFriListBtn, SIGNAL(clicked()), this, SLOT(changePageBtn()));
	connect(ui->toGrpListBtn, SIGNAL(clicked()), this, SLOT(changePageBtn()));

	/* 添加工具栏 - chatShow的工具栏 */
	QToolBar *chatShowToolBar = new QToolBar;
	chatShowToolBar->setMinimumSize(400, 38);
	chatShowToolBar->setMaximumHeight(38);
	chatShowToolBar->setStyleSheet("QToolBar{"
									   "	background-color: white;"
									   "}");
	chatShowToolBar->addWidget(ui->chatUsernameBtn);
	chatShowToolBar->addWidget(ui->chatShowTopWidget);

	/* 添加工具栏 - chatTextEdit的工具栏 */
	QToolBar *chatTextEditToolBar = new QToolBar;
	chatTextEditToolBar->setMinimumSize(400, 40);
	chatTextEditToolBar->setMaximumHeight(40);
	chatTextEditToolBar->setStyleSheet("QToolBar{"
									   "	background-color: blue;"
									   "}");
	chatTextEditToolBar->addWidget(ui->chatEditFontComboBox);
	chatTextEditToolBar->addWidget(ui->chatEditComboBox);

	/* 添加工具栏 - sendMsgBtn所在的工具栏 */
	QToolBar *sendMsgToolBar = new QToolBar;
	sendMsgToolBar->setMinimumSize(400, 40);
	sendMsgToolBar->setMaximumHeight(40);
	sendMsgToolBar->setStyleSheet("QToolBar{"
								  "	background-color: white;"
								  "}");
	sendMsgToolBar->addWidget(ui->chatSendWidget);
	sendMsgToolBar->addWidget(ui->sendMsgBtn);

	/* 创建布局器 */
	sessionSplitter = new QSplitter(Qt::Horizontal, ui->sessionPage);
	chatShowSplitter = new QSplitter(Qt::Vertical);
	chatEditSplitter = new QSplitter(Qt::Vertical);

	sessionSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sessionSplitter->setMinimumSize(650, 540);
	sessionSplitter->setHandleWidth(0);
	sessionSplitter->setChildrenCollapsible(false);
	sessionSplitter->addWidget(ui->chatItemShowListWidget);
	sessionSplitter->addWidget(chatShowSplitter);
	sessionSplitter->setStretchFactor(1, 1);

	chatShowSplitter->setMinimumSize(400, 540);
	chatShowSplitter->setHandleWidth(1);
	chatShowSplitter->setChildrenCollapsible(false);
	chatShowSplitter->addWidget(chatShowToolBar);
	chatShowSplitter->addWidget(chatEditSplitter);
	chatShowSplitter->addWidget(sendMsgToolBar);
	chatShowSplitter->setStretchFactor(1, 1);

	chatEditSplitter->setMinimumSize(400, 460);
	chatEditSplitter->setHandleWidth(1);
	chatEditSplitter->setChildrenCollapsible(false);
	chatEditSplitter->addWidget(ui->chatShowListWidget);
	chatEditSplitter->addWidget(chatTextEditToolBar);
	chatEditSplitter->addWidget(ui->chatTextEdit);
	chatEditSplitter->setStretchFactor(0, 1);

	/* 初始化按Enter键发送消息 */
	ctrlEnter = false;
	enterSendMsgAction->setEnabled(false);
	ctrlEnterSendMsgAction->setEnabled(true);
	ui->sendMsgBtn->setToolTip("按Enter键发送消息，按Ctrl+Enter键换行");

	/* 加载事件过滤器 */
	ui->chatTextEdit->installEventFilter(this);
}

/* 好友/群 列表界面初始化 */
void LSChat::listPageInit()
{
	/* 创建 [加好友/加群] - [创建群聊] 菜单 */
	addListAction = new QAction("加好友/加群");
	connect(addListAction, SIGNAL(triggered()), this, SLOT(actionsTriggered()));

	createListAction = new QAction("创建群聊");
	connect(createListAction, SIGNAL(triggered()), this, SLOT(actionsTriggered()));

	QMenu *menu = new QMenu;
	menu->addAction(addListAction);
	menu->addAction(createListAction);

	ui->listAddBtn1->setMenu(menu);
	ui->listAddBtn2->setMenu(menu);

	/* 创建布局器 */
	listSplitter = new QSplitter(Qt::Horizontal, ui->listPage);
	listShowSplitter = new QSplitter(Qt::Vertical);

	listSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	listSplitter->setMinimumSize(650, 540);
	listSplitter->setHandleWidth(0);
	listSplitter->setChildrenCollapsible(false);
	listSplitter->addWidget(listShowSplitter);
	listSplitter->addWidget(ui->infoGroupBox);
	listShowSplitter->setStretchFactor(1, 1);

	listShowSplitter->setMinimumSize(250, 540);
	listShowSplitter->setMaximumWidth(250);
	listShowSplitter->setHandleWidth(1);
	listShowSplitter->setChildrenCollapsible(false);
	listShowSplitter->addWidget(ui->listShowTopwidget);
	listShowSplitter->addWidget(ui->listStackedWidget);
	listShowSplitter->setStretchFactor(1, 1);

	/* 加载 好友/群 列表 */
//	showList();
}

/* 界面切换按钮 */
void LSChat::changePageBtn()
{
	/* 获取当前触发事件的按钮 */
	QPushButton *btn = qobject_cast<QPushButton *>(sender());

	/* 处理相应事件 */
	if(btn == ui->toSessionBtn)
	{	/* 切换到会话界面 */
		ui->globalStackedWidget->setCurrentIndex(0);
	}

	if(btn == ui->toListBtn)
	{	/* 切换到 好友/群 列表 */
		ui->globalStackedWidget->setCurrentIndex(1);
	}

	if(btn == ui->toYunBtn)
	{	/* 切换到云文件 */
		ui->globalStackedWidget->setCurrentIndex(2);
	}

	if(btn == ui->toFriListBtn)
	{	/* 切换到 好友列表 */
		ui->listStackedWidget->setCurrentIndex(0);
		ui->toFriListBtn->setStyleSheet(listShowTopBtnSelectStyle);
		ui->toFriListLine->setStyleSheet(listShowTopLineSelectStyle);
		ui->toGrpListBtn->setStyleSheet(listShowTopBtnUnSelectStyle);
		ui->toGrpListLine->setStyleSheet(listShowTopLineUnSelectStyle);
	}

	if(btn == ui->toGrpListBtn)
	{	/* 切换到 群列表 */
		ui->listStackedWidget->setCurrentIndex(1);
		ui->toGrpListBtn->setStyleSheet(listShowTopBtnSelectStyle);
		ui->toGrpListLine->setStyleSheet(listShowTopLineSelectStyle);
		ui->toFriListBtn->setStyleSheet(listShowTopBtnUnSelectStyle);
		ui->toFriListLine->setStyleSheet(listShowTopLineUnSelectStyle);
	}
}

/* 动作组事件 */
void LSChat::actionsTriggered()
{
	QAction *action = qobject_cast<QAction *>(sender());

	if(action == enterSendMsgAction)
	{	/* 按Enter键发送消息 */
		ctrlEnter = false;
		enterSendMsgAction->setEnabled(false);
		ctrlEnterSendMsgAction->setEnabled(true);
		ui->sendMsgBtn->setToolTip("按Enter键发送消息，按Ctrl+Enter键换行");
	}

	if(action == ctrlEnterSendMsgAction)
	{	/* 按Ctrl+Enter键发送消息 */
		ctrlEnter = true;
		ctrlEnterSendMsgAction->setEnabled(false);
		enterSendMsgAction->setEnabled(true);
		ui->sendMsgBtn->setToolTip("按Ctrl+Enter键发送消息，按Enter键换行");
	}

	if(action == addListAction)
	{	/* 加好友/加群 */
		findWidget = new Find;
//		findWidget->setParent(this);
		findWidget->show();
	}

	if(action == createListAction)
	{	/* 创建群聊 */

	}
}

/* 菜单按钮事件 */
void LSChat::toolBtnClicked()
{
	QPushButton *btn = qobject_cast<QPushButton *>(sender());

	qDebug() << btn;
}

/* 我的设置 */
void LSChat::on_myBtn_clicked()
{

}
