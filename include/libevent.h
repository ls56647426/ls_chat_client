#ifndef LIBEVENT_H
#define LIBEVENT_H

#include <QObject>
#include <QString>

/* third-party lib */
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>

#include "Msg.h"

class Libevent : public QObject
{
	Q_OBJECT
public:
	explicit Libevent(QObject *parent = nullptr);

	QString getServ_ip() const;
	void setServ_ip(const QString &value);

	quint32 getServ_port() const;
	void setServ_port(const quint32 &value);

	void sendMsg(const QString &msgStr);
	void recvMsg(const QString &msgStr);

private slots:
	void run();

private:
	static void read_cb(struct bufferevent *bev, void *ctx);
	static void write_cb(struct bufferevent *bev, void *ctx);
	static void event_cb(struct bufferevent *bev, short what, void *ctx);

private:
	struct event_base *base;
	struct bufferevent *bev;

	QString serv_ip;
	quint32 serv_port;
};

#endif // LIBEVENT_H
