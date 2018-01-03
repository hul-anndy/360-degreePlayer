#include "panoplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PanoPlayer w;
	w.show();
	return a.exec();
}
