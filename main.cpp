#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	/* 显示登录界面 */
	Login w;
	w.show();

	return a.exec();
}
