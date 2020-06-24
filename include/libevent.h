#ifndef LIBEVENT_H
#define LIBEVENT_H

#include <QObject>
#include <QString>

/* third-party lib */
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>

#include <QMutex>

#include "Msg.h"

/* 单例模式 */
class Libevent : public QObject
{
	Q_OBJECT
public:
	explicit Libevent(QObject *parent = nullptr);

	QString getServ_ip() const;
	void setServ_ip(const QString &value);

	quint32 getServ_port() const;
	void setServ_port(const quint32 &value);

	/* 向服务器端发送数据 */
	static void sendMsg(const QByteArray &msgArr);

	/* 读取服务器端发送的数据 */
	static void recvMsg(QString &msgStr);

private slots:
	/* 启动 槽函数 */
	void run();

private:
	/* 读缓冲区回调函数 */
	static void read_cb(struct bufferevent *bev, void *ctx);
	/* 写缓冲区回调函数 */
	static void write_cb(struct bufferevent *bev, void *ctx);
	/* 事件处理缓冲区回调函数 */
	static void event_cb(struct bufferevent *bev, short what, void *ctx);
	/* 单例模式：获取libevent对象 */
	static Libevent& getInstance();

private:
	struct event_base *base;
	struct bufferevent *bev;

	QString serv_ip;
	quint32 serv_port;

	/* 多线程互斥 */
	static QMutex mutex;
	/* 唯一一个实例 */
	static Libevent *instance;
};

#endif // LIBEVENT_H
