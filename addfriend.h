#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include <QString>

#include "pojo/User.h"

/**
 * @brief The AddFriendEnum enum
 * SendInfo：编辑发送的消息
 * SetInfo：设置对方信息(备注，分组)
 * End：编辑结束，开始发送请求
 */
enum AddFriendEnum
{
	SendInfo = 0/*, SetInfo*/, End
};

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
	Q_OBJECT

public:
	explicit AddFriend(QWidget *parent = nullptr);
	~AddFriend();

	User getUser() const;
	void setUser(const User &value);

private slots:
	void on_nextBtn_clicked();

private:
	Ui::AddFriend *ui;

	/* 当前处于哪一步 */
	quint8 status;

	/* 发送的验证消息 */
	QString checkInfo;

	/* 要添加的对象 */
	User user;
};

#endif // ADDFRIEND_H
