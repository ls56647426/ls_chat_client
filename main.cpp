#include "login.h"
#include "edituser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Login login;
	login.show();
//	EditUser e;
//	e.show();

	return a.exec();
}
