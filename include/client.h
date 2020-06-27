#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QList>
#include <QTimer>

#include "Msg.h"

/* 单例模式：懒汉式 + 双检锁 */
class Client : public QObject
{
	Q_OBJECT
public:
	/* 单例模式：双检锁，获取libevent对象 */
	static Client &getInstance();
	/* 将一个Msg包装起来并发送给服务器 */
	static void sendMsg(const Msg msg);
	/* 从msgList中取出一个msg，线程安全 */
	static Msg getMsg();

private:
	/* 私有化构造函数 */
	Client();
	/* 封装数据包 */
	static QByteArray packet(const QByteArray &arr);
	/* 解析数据包 */
	static QByteArray unpacket(const QByteArray &arr);

private slots:
	/* 自动接收服务器发送的数据并存入列表 */
	static void readMsg();
	/* 接收error信号 */
	static void error_cb();

private:
	QTcpSocket *tcpClient;
	QString serv_ip;
	quint32 serv_port;

	/* 收到的有效包，自动转为Msg存入列表中等待操作 */
	QList<Msg> msgList;
	QTimer *heartTimer;
	quint8 heartbeats;

	/* 多线程互斥 */
	static QMutex mutex;
	static QMutex msgListMutex;
	/* 单例模式：懒汉式 */
	static Client *instance;
};

#endif // CLIENT_H
