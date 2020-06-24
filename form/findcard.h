#ifndef FINDCARD_H
#define FINDCARD_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

#include "pojo/User.h"

class FindCard : public QWidget
{
	Q_OBJECT
public:
	explicit FindCard(QWidget *parent = nullptr);

	User getUser() const;
	void setUser(const User &value);

private slots:
	/* 按钮单击事件组的槽函数 */
	void btnsClicked();

private:
	User user;

	QPushButton *iconBtn;
	QPushButton *infoBtn;
	QPushButton *addFriendBtn;
};

#endif // FINDCARD_H
