#include "libevent.h"

#include <QSettings>
#include <QDebug>
#include <QHostAddress>
#include <ws2tcpip.h>

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

void Libevent::sendMsg(const QString &msgStr)
{
	/* 发送消息 */
	bufferevent_write(bev, msgStr.toStdString().data(),
					  msgStr.size());
}

void Libevent::recvMsg(const QString &msgStr)
{
	bufferevent_read(bev, msgStr.toStdString().data(),
					  MSGINFO_MAX_LEN);
}

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

void Libevent::read_cb(bufferevent *bev, void *ctx)
{
	char buf[BUFSIZ];
	struct evbuffer* output = bufferevent_get_output(bev);

	bufferevent_read(bev, &buf, BUFSIZ);
	qDebug() << "server:" << buf;
	evbuffer_add(output, "this is client to server msg.",
				 sizeof("this is client to server msg."));
}

void Libevent::write_cb(bufferevent *bev, void *ctx)
{
	qDebug() << "数据发送完成";
}

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
