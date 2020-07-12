#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QWidget>

#include "../pojo/User.h"

namespace Ui {
class FriendListItem;
}

class FriendListItem : public QWidget
{
	Q_OBJECT

public:
	explicit FriendListItem(QWidget *parent = nullptr);
	~FriendListItem();

	void setData(const User &user);

	User getUser() const;

private:
	static QPixmap pixmapToRound(const QPixmap &src, int radius);

private:
	Ui::FriendListItem *ui;

	User user;
};

#endif // FRIENDLISTITEM_H
