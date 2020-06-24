#include "findcard.h"
#include "../addfriend.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCursor>

FindCard::FindCard(QWidget *parent) : QWidget(parent)
{
	/* 头像按钮 */
	iconBtn = new QPushButton;
//	iconBtn->setIcon(QIcon(user.getIcon()));
	iconBtn->setIconSize(QSize(64, 64));
	iconBtn->setMinimumSize(64, 64);
	iconBtn->setMaximumSize(64, 64);
	iconBtn->setCursor(QCursor(Qt::PointingHandCursor));
	iconBtn->setStyleSheet("QPushButton{"
						   "	border: none;"
						   "	border-radius: 32px;"
						   "}");
	connect(iconBtn, SIGNAL(clicked()), this, SLOT(btnsClicked()));

	/* 详细信息按钮 */
	infoBtn = new QPushButton;
	infoBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	infoBtn->setMaximumHeight(16);
	infoBtn->setCursor(QCursor(Qt::PointingHandCursor));
	infoBtn->setStyleSheet("QPushButton{"
						   "	border: none;"
						   "	font-size: 14px;"
						   "}");
	connect(infoBtn, SIGNAL(clicked()), this, SLOT(btnsClicked()));

	/* 加好友按钮 */
	addFriendBtn = new QPushButton;
	addFriendBtn->setText("加好友");
	addFriendBtn->setMinimumSize(50, 20);
	addFriendBtn->setMaximumSize(50, 20);
	addFriendBtn->setStyleSheet("QPushButton{"
								"	font-size: 14px;"
								"	color: rgb(255, 255, 255);"
								"	background-color: #00A3FF;"
								"	border-radius: 2px;"
								"}"
								"QPushButton:hover{"
								"	background-color: #3CC3F5;"
								"}");
	connect(addFriendBtn, SIGNAL(clicked()), this, SLOT(btnsClicked()));

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addWidget(infoBtn);
	vlayout->addWidget(addFriendBtn);

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addStretch();
	hlayout->addWidget(iconBtn);
	hlayout->addLayout(vlayout);
	hlayout->addStretch();
	hlayout->setMargin(10);
	hlayout->setSpacing(8);

	setLayout(hlayout);
	setMinimumSize(200, 95);
	setMaximumSize(200, 95);
}

/* 按钮单击事件组的槽函数 */
void FindCard::btnsClicked()
{
	QPushButton *btn = qobject_cast<QPushButton *>(sender());

	if(btn == iconBtn || btn == infoBtn)
	{	/* 弹出详细信息窗口 */

	}

	if(btn == addFriendBtn)
	{	/* 弹出加好友窗口 */
		AddFriend *addFriend = new AddFriend;
		addFriend->setUser(user);
		addFriend->show();
	}
}

User FindCard::getUser() const
{
	return user;
}

void FindCard::setUser(const User &value)
{
	user = value;

	iconBtn->setText("头像");
	infoBtn->setText(QString("%1(%2)").arg(user.getUsername().data()).arg(user.getId()));
}
