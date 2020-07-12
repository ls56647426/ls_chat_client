#include "friendlistitem.h"
#include "ui_friendlistitem.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>

FriendListItem::FriendListItem(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FriendListItem)
{
	ui->setupUi(this);
}

FriendListItem::~FriendListItem()
{
	delete ui;
}

void FriendListItem::setData(const User &user)
{
	this->user = user;

	/* 填充头像 */
	QString picStr = ":/Icon/user.png";
//	QPixmap pixmapPic(user.getPic().data());
	QPixmap pixmapPic(picStr);
	int picWidth = ui->picLabel->width();
	int picHeight = ui->picLabel->height();
	/* 饱满填充 */
	QPixmap pixmapPicFit = pixmapPic.scaled(picWidth, picHeight,
											Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	pixmapPicFit = pixmapToRound(pixmapPicFit, 20);
	ui->picLabel->setPixmap(pixmapPicFit);

	/* 填充昵称 */
	ui->nicknameLabel->setText(user.getNickname().data());
	ui->nicknameLabel->setToolTip(QString("%1(%2)").arg(
									  user.getNickname().data()).arg(
									  user.getUsername().data()));

	/* 填充签名 */
	ui->signLabel->setText(user.getSign().data());
	ui->signLabel->setToolTip(QString("个性签名更新；%1").arg(
								  user.getSign().data()));
}

/* 头像转圆形 */
QPixmap FriendListItem::pixmapToRound(const QPixmap &src, int radius)
{
	if(src.isNull()) return QPixmap();

	QSize size(2*radius, 2*radius);
	QBitmap mask(size);
	QPainter painter(&mask);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

	QPixmap image = src.scaled(size);
	image.setMask(mask);
	return image;
}

User FriendListItem::getUser() const
{
	return user;
}
