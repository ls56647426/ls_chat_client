#include "login.h"
#include "lschat.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Login login;
	LSChat client;

	a.connect(&login, SIGNAL(showLSChat(User)), &client, SLOT(run(User)));

	login.show();

	return a.exec();
}
