#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "single_instance_guard.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

	QString name = "fardragon.DigitalVibranceSwitcher";
	Single_Instance_Guard guard;
	if (guard.has_previous_instance(name))
	{
		QMessageBox::critical(nullptr,"Error","Digital Vibrance Switcher is already running");
		return 0;
	}
	guard.listen(name);
	return a.exec();
}
