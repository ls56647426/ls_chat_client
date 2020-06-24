#include "libevent.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <ws2tcpip.h>
#include <JlCompress.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <stdlib.h>

QMutex Libevent::mutex;
Libevent *Libevent::instance = NULL;

Libevent::Libevent(QObject *parent) : QObject(parent)
{
	/* 从ini配置文件中读取配置信息 */
	QSettings settings("E:/qt/202006051029-Chat/Client/config/libevent.ini", QSettings::IniFormat);

	/* 获取服务器配置信息 */
	serv_ip = settings.value("server/ip", "112.124.19.14").toString();
	serv_port = settings.value("server/port", 5664).toUInt();
}

QString Libevent::getServ_ip() const
{
	return serv_ip;
}

void Libevent::setServ_ip(const QString &value)
{
	serv_ip = value;
}

quint32 Libevent::getServ_port() const
{
	return serv_port;
}

void Libevent::setServ_port(const quint32 &value)
{
	serv_port = value;
}

/* 向服务器端发送数据 */
void Libevent::sendMsg(const QByteArray &msgArr)
{
	qDebug() << "开始发送";
	QByteArray destArr;
	char buf[MSGINFO_MAX_LEN];
	/* 获取实例 */
	Libevent &event = Libevent::getInstance();

	/* 获取消息头 */
	if(msgArr.size() < sizeof(MsgHeader))
	{
		qDebug() << "没有消息头：" << msgArr;
		return;
	}
	MsgHeader header;
	memcpy(&header, msgArr.toStdString().data(), sizeof(header));

	/* 包头有错误，取消发送 */
	if(header.originsize <= 0 ||
			header.originsize > MSGINFO_MAX_LEN)
	{
		qDebug() << "包头有错误";
		return;
	}

	/* 包头信息与实际发送数据内容不匹配 */
	if(msgArr.size() != sizeof(header) + header.originsize)
	{
		qDebug() << "包头信息错误：28 + " << header.originsize << "!=" << msgArr.size();
		return;
	}

	/* 需要压缩 */
	qDebug() << "包头校验完成";
	if(header.compressflag == MsgHeaderType::COMPRESSED)
	{
		/* 开始压缩数据 */
		destArr = qCompress(msgArr.toStdString().data() + sizeof(header), header.originsize);
		header.compresssize = destArr.size();
		/* 压缩结果不对 */
		qDebug() << "压缩后大小：" << destArr.size() << destArr;
		if(header.compresssize <= 0 ||
				header.compresssize > MSGINFO_MAX_LEN)
		{
			qDebug() << "压缩结果错误：" << destArr.size() << destArr;
			return;
		}

		/* 发送数据 */
		QByteArray dest((char *)&header, sizeof(header));
		dest += destArr;
		qDebug() << "发送压缩后数据：" << dest;
		memcpy(buf, dest.toStdString().data(), dest.size());
		bufferevent_write(event.bev, buf, dest.size());
	}
	else
	{
		/* 不用压缩，直接发送数据 */
		qDebug() << "不需压缩直接发送数据：" << msgArr;
		memcpy(buf, msgArr.toStdString().data(), msgArr.size());
		bufferevent_write(event.bev, buf, msgArr.size());
	}
	qDebug() << "发送完成";
}

/* 读取服务器端发送的数据 */
void Libevent::recvMsg(QString &msgStr)
{
	char buf[MSGINFO_MAX_LEN];
	QByteArray destArr;
	/* 获取实例 */
	Libevent &event = Libevent::getInstance();

	/* 获取包头 */
	MsgHeader header;
	struct evbuffer *input = bufferevent_get_input(event.bev);
	evbuffer_copyout(input, &header, sizeof(MsgHeader));

	/* 解析包头 */
	if(header.compressflag == MsgHeaderType::COMPRESSED)
	{
		/* 包头有错误，立即关闭连接 */
		if(header.originsize <= 0 ||
				header.originsize > MSGINFO_MAX_LEN ||
				header.compresssize <= 0 ||
				header.compresssize > MSGINFO_MAX_LEN)
		{
			qDebug() << "包头错误：" << header.originsize << header.compresssize;
			return;
		}

		/* 收到的数据不够一个完整的数据包 */
		if(evbuffer_get_length(input) < sizeof(header) + header.compresssize)
			return;

		evbuffer_drain(input, sizeof(header));
		evbuffer_remove(input, buf, header.compresssize);

		destArr = qUncompress((uchar *)buf, header.compresssize);
		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(destArr, &jsonError));
		if(jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "json error：" << destArr;
			return;
		}

		QJsonObject rootObj = jsonDoc.object();
		QStringList keys = rootObj.keys();
		for(int i = 0; i < keys.size(); i++)
		{
			qDebug() << "key" << i << " is:" << keys.at(i);
		}
	}
	else
	{
		/* 包头有错误，立即关闭连接 */
		if(header.originsize <= 0 ||
				header.originsize > MSGINFO_MAX_LEN)
		{
			qDebug() << "包头错误：" << header.originsize;
			return;
		}

		/* 收到的数据不够一个完整的数据包 */
		if(evbuffer_get_length(input) < sizeof(header) + header.originsize)
			return;

		evbuffer_drain(input, sizeof(header));
		evbuffer_remove(input, buf, header.originsize);

		destArr = qUncompress((uchar *)buf, header.originsize);
		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(destArr, &jsonError));
		if(jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "json error：" << destArr;
			return;
		}

		QJsonObject rootObj = jsonDoc.object();
		QStringList keys = rootObj.keys();
		for(int i = 0; i < keys.size(); i++)
		{
			qDebug() << "key" << i << " is:" << keys.at(i);
		}
	}
	msgStr = destArr;
}

/* 启动 槽函数 */
void Libevent::run()
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	//创建base
	base = event_base_new();

	int fd = socket(AF_INET, SOCK_STREAM, 0);

	//创建bufferevent,并将通信的fd放到bufferevent中
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	/* 初始化服务器端地址结构 */
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip.toStdString().data());
	serv_addr.sin_port = htons(serv_port);

	//连接服务器
	bufferevent_socket_connect(bev, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	/* 监听、添加终端输入事件 */
	/* TODO: 改为获取ui界面操作 */
/*	struct event *ev_cmd = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST,
									 cmd_msg_cb, bev);
	event_add(ev_cmd, NULL);*/

	/* 注册普通的信号事件 */
	/*	struct event *esc_signal_event = evsignal_new(base, SIGINT, signal_cb, base);
		if(!signal_event || event_add(signal_event, NULL) < 0)
		{
			fprintf(stderr, "信号事件注册失败!\n");
			exit(1);
		}*/

	//设置回调函数
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	event_base_dispatch(base);

//	event_free(ev_cmd);
	event_base_free(base);
}

/* 读缓冲区回调函数 */
void Libevent::read_cb(bufferevent *bev, void *ctx)
{
	char buf[BUFSIZ];
	struct evbuffer* output = bufferevent_get_output(bev);

	bufferevent_read(bev, &buf, BUFSIZ);
	qDebug() << "server:" << buf;
	evbuffer_add(output, "this is client to server msg.",
				 sizeof("this is client to server msg."));
}

/* 写缓冲区回调函数 */
void Libevent::write_cb(bufferevent *bev, void *ctx)
{
	qDebug() << "数据发送完成";
}

/* 事件处理缓冲区回调函数 */
void Libevent::event_cb(bufferevent *bev, short what, void *ctx)
{
	if (what & BEV_EVENT_EOF) //遇到文件结束指示
	{
		qDebug() << "连接关闭。";
	}
	else if (what & BEV_EVENT_ERROR) //操作发生错误
	{
		qDebug() << "连接发生错误！";
	}
	else if (what & BEV_EVENT_TIMEOUT) //超时
	{
		qDebug() << "与服务器连接超时。。。";
	}
	else if (what & BEV_EVENT_CONNECTED) //连接已经完成
	{
		/* 提示主页信息 */
//		showInitMenu();
//		curMenu = MENU_INIT;
		qDebug() << "连接完成。";
		return;
	}
	/* None of the other events can happen here, since we haven't enabled
		 * timeouts */

	/* close套接字，free读写缓冲区 */
	bufferevent_free(bev);
}

/* 单例模式：获取libevent对象 */
Libevent &Libevent::getInstance()
{
	if (!instance)
	{
		QMutexLocker locker(&mutex);
		if (!instance)
			instance = new Libevent();
	}

	return *instance;
}
