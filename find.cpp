#include "find.h"
#include "ui_find.h"

#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QToolTip>
#include <QTableWidget>
#include <QDebug>

#include "form/findcard.h"

Find::Find(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Find)
{
	ui->setupUi(this);

	/* 隐藏窗口，只显示控件 */
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	/* 阴影效果初始化 */
	QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(ui->findWidget);
	shadowEffect->setOffset(0, 0);							/* 阴影偏移 */
	shadowEffect->setColor(Qt::black);						/* 阴影颜色 */
	shadowEffect->setBlurRadius(10);						/* 阴影半径 */

	/* 设置阴影效果 */
	ui->findWidget->setGraphicsEffect(shadowEffect);
	delete shadowEffect;

	searchLineEdit = new SearchLineEdit;
	searchLineEdit->setMinimumHeight(30);
	searchLineEdit->setPlaceholderText("请输入用户名");

	ui->showCardTableWidget->hide();

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(searchLineEdit);
	hlayout->addWidget(ui->findBtn);
	hlayout->setContentsMargins(40, 30, 40, 30);

	ui->searchWidget->setLayout(hlayout);
}

Find::~Find()
{
	delete ui;
}

/* 鼠标事件重载 - 按下事件 */
void Find::mousePressEvent(QMouseEvent *event)
{
	bPressFlag = true;
	beginDrag = event->pos();
	QWidget::mousePressEvent(event);
}

/* 鼠标事件重载 - 释放事件 */
void Find::mouseReleaseEvent(QMouseEvent *event)
{
	bPressFlag = false;
	QWidget::mouseReleaseEvent(event);
}

/* 鼠标事件重载 - 移动事件 */
void Find::mouseMoveEvent(QMouseEvent *event)
{
	if(bPressFlag)
	{
		QPoint relaPos(QCursor::pos() - beginDrag);
		move(relaPos);
	}
	QWidget::mouseMoveEvent(event);
}

/* 查找 */
void Find::on_findBtn_clicked()
{
	/* 获取数据 */
	QString username = searchLineEdit->text();

	/* 有效性校验 */
	if(username.isEmpty())
	{
		searchLineEdit->setFocus();
		QToolTip::showText(searchLineEdit->mapToGlobal(QPoint(0, searchLineEdit->height())),
						   "请输入用户名再点击查找", this);
		return;
	}

	/* 显示showTableWidget */
	if(ui->showCardTableWidget->isHidden())
	{
		resize(626, 446);
		ui->findWidget->resize(620, 440);
		ui->showCardTableWidget->show();
	}

	/* 查找 */
	User user;
	user.setUsername(username.toStdString());
	Msg msg;
	msg.setType(MsgType::COMMAND_FIND_USER);
	msg.setSrc(user);
	Client::tcpSendMsg(msg);
	msg = Client::getMsg();

	/* 找不到就没有数据，暂时没有报错提示 */
	/* 显示 */
	/* 暂时只通过用户名查找，所以只有一个数据 */
	FindCard *findCard = new FindCard;
	findCard->setUser(msg.getSrc());
	findCard->setStyleSheet("FindCard{"
							"	background-color: rgb(247, 247, 247);"
							"}");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(findCard);
	layout->addStretch();

	ui->showCardTableWidget->clear();
	ui->showCardTableWidget->setRowCount(1);
	ui->showCardTableWidget->setColumnCount(1);
	ui->showCardTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->showCardTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->showCardTableWidget->setCellWidget(0, 0, findCard);
}
