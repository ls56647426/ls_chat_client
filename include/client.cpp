#include "client.h"

#include <QDebug>
#include <QSettings>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <zlib.h>

QMutex Client::mutex;
QMutex Client::msgListMutex;
QMutex Client::recordListMutex;
QWaitCondition Client::waitMsgList;
/* 单例模式：懒汉式 */
Client *Client::instance = NULL;

Client::Client()
{
	qDebug() << "进入Client构造函数。";
	/* 获取父母DNA信息 */
	QSettings settings("E:/qt/202006051029-Chat/Client/config/server.ini", QSettings::IniFormat);
	serv_ip = settings.value("server/ip", "112.124.19.14").toString();
	serv_tcp_port = settings.value("server/tcp_port", 5664).toUInt();
//	serv_udp_port = settings.value("server/udp_port", 7426).toUInt();

	/* 怀胎十月 */
	tcpClient = new QTcpSocket(this);
	connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpReadMsg()));
	tcpClient->connectToHost(QHostAddress(serv_ip), serv_tcp_port);

	/* 他杀 */
	connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_cb()));

	/* 被父母抛弃 */
	connect(tcpClient, SIGNAL(disconnected()), this, SLOT(disconnect_cb()));

	/* 出生，开始心跳，心跳间隔时间3s */
	heartbeats = 0;
	heartTimer = new QTimer(this);
	connect(heartTimer, SIGNAL(timeout()), this, SLOT(heartbeat_timeout_cb()));
	heartTimer->start(3000);

//	/* udp */
//	udpClient = new QUdpSocket(this);
//	udpClient->bind();
//	connect(udpClient, SIGNAL(readyRead()), this, SLOT(udpReadMsg()));
}

/* 将一个Msg包装起来并发送给服务器 */
void Client::tcpSendMsg(const Msg msg)
{
	string msgStr = msg.toString();
	QByteArray msgArr(msgStr.data(), msgStr.size()), destArr;

	/* 获取唯一实例 */
	Client &client = Client::getInstance();

	/* 封装数据包 */
	destArr = packet(msgArr);

	/* 发送消息 */
	client.tcpClient->write(destArr);
}

/* 从msgList中取出一个msg，线程安全 */
Msg Client::getMsg()
{
	Msg msg;

	/* 获取唯一实例 */
	Client &client = Client::getInstance();

	/* 等待服务器端发送数据并接收完成 */
	while(!client.msgList.size())
		/* 这一步是tcp异步处理的精髓 */
		client.tcpClient->waitForReadyRead(1000);

	/* 取出msg */
	QMutexLocker locker(&msgListMutex);
	msg = client.msgList.front();
	client.msgList.pop_front();

	return msg;
}

/* 单例模式：双检锁，获取客户端对象 */
Client &Client::getInstance()
{
	if (!instance)
	{
		QMutexLocker locker(&mutex);
		if (!instance)
			instance = new Client();
	}

	return *instance;
}

/* 封装数据包 */
QByteArray Client::packet(const QByteArray &arr)
{
	QByteArray destArr, res;

	/* 包装消息头 */
	MsgHeader header;
	header.compressflag = MsgHeaderType::COMPRESSED;
	header.originsize = arr.size();
	header.compresssize = compressBound(header.originsize);

	/* 压缩，插入消息头 */
	if(header.compressflag == MsgHeaderType::COMPRESSED)
	{
		/* 开始压缩数据 */
		Bytef *buf = new Bytef[header.compresssize];
		compress(buf, (uLongf*)&header.compresssize, (Bytef*)arr.data(), header.originsize);
		destArr = QByteArray((char*)buf, header.compresssize);

		/* 压缩校验 */
		if(header.compresssize <= 0 ||
				header.compresssize > MSGINFO_MAX_LEN)
		{
			qDebug() << "压缩结果错误";
			exit(1);
		}
		res = QByteArray((char *)&header, sizeof(header)) + destArr;
	}
	else res = QByteArray((char *)&header, sizeof(header)) + arr;

	return res;
}

/* 解析数据包 */
QByteArray Client::unpacket(const QByteArray &arr)
{
	MsgHeader header;
	QByteArray destArr;

	/* 获取包头 */
	memcpy(&header, arr.data(), sizeof(header));

	/* 压缩过，需要解压后再提取内容 */
	if(header.compressflag == MsgHeaderType::COMPRESSED)
	{
		/* 包头有错误，立即关闭连接 */
		if(header.originsize <= 0 ||
				header.originsize > MSGINFO_MAX_LEN ||
				header.compresssize <= 0 ||
				header.compresssize > MSGINFO_MAX_LEN)
		{
			qDebug() << "包头错误：" << header.originsize << header.compresssize;
			return destArr;
		}

		/* 收到的数据不够一个完整的数据包 */
		if((uint32_t)arr.size() < sizeof(header) + header.compresssize)
			return destArr;

		/* 去掉包头，解压数据 */
		Bytef *buf = new Bytef[header.originsize];
		int ret = uncompress(buf, (uLongf*)&header.originsize,
							 (Bytef*)arr.data() + sizeof(header), header.compresssize);
		if(ret != Z_OK)
		{
			qDebug() << "解压数据出错：" << ret;
			return destArr;
		}
		destArr = QByteArray((char *)buf, header.originsize);
	}
	/* 没压缩过，直接提取 */
	else
	{
		/* 包头有错误，立即关闭连接 */
		if(header.originsize <= 0 ||
				header.originsize > MSGINFO_MAX_LEN)
		{
			qDebug() << "包头错误：" << header.originsize;
			return destArr;
		}

		/* 收到的数据不够一个完整的数据包 */
		if((uint32_t)arr.size() < sizeof(header) + header.originsize)
			return destArr;

		/* 去掉包头，不需要解压 */
		destArr = QByteArray(arr.data() + sizeof(header), header.originsize);
	}

	return destArr;
}

/* 殡葬一条龙 */
void Client::died()
{
	/* 每个人都是唯一的 */
	Client &client = Client::getInstance();

	/* 掏心 */
	client.heartTimer->stop();
	/* 切块 */
	client.tcpClient->close();
	/* 火葬 */
	delete instance;
	/* 奏乐 */
	qDebug() << "tcpCLient 你 死 的 好 惨 呐 /(ㄒoㄒ)/~~";
	/* 招魂 */
	/* ... */
}

/* Json转Msg */
Msg Client::jsonToMsg(const QByteArray &jsonArr)
{
	Msg msg;
	QJsonParseError jsonError;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonArr, &jsonError));
	if(jsonError.error != QJsonParseError::NoError)
	{
		qDebug() << "json error：" << jsonArr;
		return msg;
	}

	QJsonObject msgObj = jsonDoc.object();
	msg.setType(msgObj["type"].toInt());
	/* 心跳包不往下读了 */
	if(msg.getType() == MsgType::MSG_HEARTBEAT)
		return msg;

	Group grp;
	QJsonObject subObj = msgObj["group"].toObject();
	grp.setId(subObj["id"].toInt());
	grp.setName(subObj["name"].toString().toStdString());
	msg.setGroup(grp);

	User src;
	subObj = msgObj["src"].toObject();
	src.setId(subObj["id"].toInt());
	src.setUsername(subObj["username"].toString().toStdString());
	src.setPassword(subObj["password"].toString().toStdString());
	src.setPic(subObj["pic"].toString().toStdString());
	src.setNickname(subObj["nickname"].toString().toStdString());
	src.setSign(subObj["sign"].toString().toStdString());
	src.setSex(subObj["sex"].toString().toStdString());
	src.setBirthday(subObj["birthday"].toString().toStdString());
	src.setLocation(subObj["location"].toString().toStdString());
	src.setProfession(subObj["profession"].toString().toStdString());
	src.setMobile(subObj["mobile"].toString().toStdString());
	src.setEmail(subObj["email"].toString().toStdString());
	src.setStatus(subObj["status"].toInt());
	src.setDate(subObj["date"].toString().toStdString());
	msg.setSrc(src);

	User dest;
	subObj = msgObj["dest"].toObject();
	dest.setId(subObj["id"].toInt());
	dest.setUsername(subObj["username"].toString().toStdString());
	dest.setPassword(subObj["password"].toString().toStdString());
	dest.setPic(subObj["pic"].toString().toStdString());
	dest.setNickname(subObj["nickname"].toString().toStdString());
	dest.setSign(subObj["sign"].toString().toStdString());
	dest.setSex(subObj["sex"].toString().toStdString());
	dest.setBirthday(subObj["birthday"].toString().toStdString());
	dest.setLocation(subObj["location"].toString().toStdString());
	dest.setProfession(subObj["profession"].toString().toStdString());
	dest.setMobile(subObj["mobile"].toString().toStdString());
	dest.setEmail(subObj["email"].toString().toStdString());
	dest.setStatus(subObj["status"].toInt());
	dest.setDate(subObj["date"].toString().toStdString());
	msg.setDest(dest);

	MsgInfo info;

	subObj = msgObj["info"].toObject();
	info.setInfo(subObj["info"].toString().toStdString());

	list<User> list;
	QJsonArray userArr = subObj["userListInfo"].toArray();
	for(int i = 0; i < userArr.size(); i++)
	{
		QJsonObject userObj = userArr.at(i).toObject()["user"].toObject();
		User user;
		user.setId(userObj["id"].toInt());
		user.setUsername(userObj["username"].toString().toStdString());
		user.setPassword(userObj["password"].toString().toStdString());
		user.setPic(userObj["pic"].toString().toStdString());
		user.setNickname(userObj["nickname"].toString().toStdString());
		user.setSign(userObj["sign"].toString().toStdString());
		user.setSex(userObj["sex"].toString().toStdString());
		user.setBirthday(userObj["birthday"].toString().toStdString());
		user.setLocation(userObj["location"].toString().toStdString());
		user.setProfession(userObj["profession"].toString().toStdString());
		user.setMobile(userObj["mobile"].toString().toStdString());
		user.setEmail(userObj["email"].toString().toStdString());
		user.setStatus(userObj["status"].toInt());
		user.setDate(userObj["date"].toString().toStdString());
		list.push_back(user);
	}

	info.setUserListInfo(list);

	msg.setInfo(info);

	return msg;
}

/* 消息记录获取 */
Msg Client::getRecord()
{
	Msg msg;

	/* 获取唯一实例 */
	Client &client = Client::getInstance();

	/* 等待服务器端发送数据并接收完成 */
	while(!client.recordList.size())
		/* 这一步是tcp异步处理的精髓 */
		client.tcpClient->waitForReadyRead(1000);

	/* 取出msg */
	QMutexLocker locker(&msgListMutex);
	msg = client.recordList.front();
	client.recordList.pop_front();

	return msg;
}

/* udp发送 */
//void Client::udpSendMsg(const Msg msg)
//{
//	string msgStr = msg.toString();
//	QByteArray msgArr(msgStr.data(), msgStr.size()), destArr;

//	/* 获取唯一实例 */
//	Client &client = Client::getInstance();

//	/* 封装数据包 */
//	destArr = packet(msgArr);

//	/* 发送消息 */
//	qDebug() << "udpSendMsg:" << destArr;
//	client.udpClient->writeDatagram(destArr, QHostAddress(client.serv_ip), client.serv_udp_port);
//}

/* 自动接收服务器发送的数据并存入列表 */
void Client::tcpReadMsg()
{
	QByteArray arr, destArr;
	Msg msg;

	/* 获取唯一实例 */
	Client &client = Client::getInstance();

	/* 读取服务器发送的数据 */
	arr = client.tcpClient->readAll();

	/* 解包 */
	destArr = unpacket(arr);

	/* Json转Msg */
	msg = jsonToMsg(destArr);

	/* 心跳包，不入列表 */
	if(msg.getType() == MsgType::MSG_HEARTBEAT)
	{
		client.heartbeats = 0;
		return;
	}

	/* 消息包，存入消息记录表 */
	if(msg.getType() == MsgType::MSG_SEND_MSG)
	{
		QMutexLocker locker(&recordListMutex);
		client.recordList.push_back(msg);
		return;
	}

	/* 存入Msg队列 */
	QMutexLocker locker(&msgListMutex);
	client.msgList.push_back(msg);
}

/* 他杀 */
void Client::error_cb()
{
	/* 获取唯一实例 */
	Client &client = Client::getInstance();

	qDebug() << client.tcpClient->errorString();
	client.tcpClient->close();

	QMutexLocker locker(&mutex);
	delete instance;
	instance = new Client;
}

/* 被父母抛弃 */
void Client::disconnect_cb()
{
	/* 心灰意冷，自我了断 */
	died();
}

/* 心跳 */
void Client::heartbeat_timeout_cb()
{
	/* 每个人都是唯一的 */
	Client &client = Client::getInstance();

	/* 毫无生命迹象 */
	if(client.heartbeats == 5)
		died();

	/* 心跳ing */
	client.heartbeats++;
	Msg msg;
	msg.setType(MsgType::MSG_HEARTBEAT);
	tcpSendMsg(msg);
}

///* udp */
//void Client::udpReadMsg()
//{
//	QByteArray destArr;
//	Msg msg;

//	/* 每个人都是唯一的 */
//	Client &client = Client::getInstance();

//	/* 读取服务器发送的数据 */
//	qint64 size = client.udpClient->pendingDatagramSize();
//	QByteArray arr(size, 0);
//	client.udpClient->readDatagram(arr.data(), size);

//	/* 解包 */
//	destArr = unpacket(arr);

//	/* Json转Msg */
//	msg = jsonToMsg(destArr);

//	/* 判断是否为心跳包，如果是，不存入列表 */
//	if(msg.getType() == MsgType::MSG_HEARTBEAT)
//	{
//		client.heartbeats = 0;
//		return;
//	}

//	/* 存入Msg队列 */
//	QMutexLocker locker(&msgListMutex);
//	client.msgList.push_back(msg);
//}

