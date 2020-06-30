#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QTimer>
#include <QUdpSocket>

#include "Msg.h"
#include "pojo/Friend.h"

/* 单例模式：懒汉式 + 双检锁 */
class Client : public QObject
{
	Q_OBJECT
public:
	/* 单例模式：双检锁，获取客户端对象 */
	static Client &getInstance();
	/* 将一个Msg包装起来并发送给服务器 */
	static void tcpSendMsg(const Msg msg);
	/* 从msgList中取出一个msg，线程安全 */
	static Msg getMsg();
	/* Json转Msg */
	static Msg jsonToMsg(const QByteArray &jsonArr);
	/* udp发送 */
//	static void udpSendMsg(const Msg msg);
	/* 消息记录获取 */
	static Msg getRecord();

private:
	/* 私有化构造函数 */
	Client();
	/* 封装数据包 */
	static QByteArray packet(const QByteArray &arr);
	/* 解析数据包 */
	static QByteArray unpacket(const QByteArray &arr);
	/* 殡葬一条龙 */
	static void died();

private slots:
	/* 自动接收服务器发送的数据并存入列表 */
	void tcpReadMsg();
	/* 他杀 */
	void error_cb();
	/* 被父母抛弃 */
	void disconnect_cb();
	/* 心跳 */
	void heartbeat_timeout_cb();
	/* udp */
//	void udpReadMsg();

private:
	QTcpSocket *tcpClient;
	QUdpSocket *udpClient;
	QString serv_ip;
	quint32 serv_tcp_port;
//	quint32 serv_udp_port;

	/* 收到的有效包，自动转为Msg存入列表中等待操作 */
	QList<Msg> msgList;
	QList<Msg> recordList;
	QTimer *heartTimer;
	quint8 heartbeats;

	/* 多线程互斥 */
	static QMutex mutex;
	static QMutex msgListMutex;
	static QMutex recordListMutex;
	static QWaitCondition waitMsgList;
	/* 单例模式：懒汉式 */
	static Client *instance;
};

#endif // CLIENT_H
