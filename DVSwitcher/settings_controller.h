#ifndef SETTINGS_CONTROLLER_H
#define SETTINGS_CONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QCoreApplication>
#include <vector>
#include <tuple>


class Settings_Controller : public QObject
{
	Q_OBJECT
public:
	explicit Settings_Controller(QObject *parent = nullptr);
	~Settings_Controller();
	void load_settings();

	void save_apps(const std::vector<std::tuple<QString,int,bool>> apps_data);

private:
	QSettings* m_settings;
	bool check_start_windows();
	void load_apps();

signals:
	void loaded_def_vibrance(int);
	void loaded_start_minimized(bool);
	void loaded_start_windows(bool);
	void loaded_disable_switcher(bool);
	void loaded_app(std::tuple<QString,int,bool>);


public slots:
	void new_def_vibrance(int new_def_vib);
	void new_start_minimized(bool start_minimized);
	void new_start_windows(bool start_windows);
	void new_disable_switcher(bool disable_switcher);

};

#endif // SETTINGS_CONTROLLER_H
