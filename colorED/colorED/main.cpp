#include "flowermodeling.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	flowermodeling w;
	w.show();
	return a.exec();
}
