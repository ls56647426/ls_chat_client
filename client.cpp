#include "client.h"
#include "ui_client.h"

#include <QDebug>

Client::Client(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Client)
{
	ui->setupUi(this);

	event = new Libevent;
	pThread = new QThread(this);
	event->moveToThread(pThread);
	connect(this, SIGNAL(sigEventRun()), event, SLOT(run()));
}

Client::~Client()
{
	delete ui;
}

void Client::on_pushButton_clicked()
{
	if(pThread->isRunning())
		return;
	qDebug() << "开始链接服务器端。";
	pThread->start();
	emit sigEventRun();
}
