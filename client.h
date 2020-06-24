#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QThread>

#include "include/libevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
	Q_OBJECT

public:
	Client(QWidget *parent = nullptr);
	~Client();

private slots:
	void on_pushButton_clicked();

signals:
	void sigEventRun();

private:
	Ui::Client *ui;

	QThread *pThread;
	Libevent *event;
};
#endif // CLIENT_H
