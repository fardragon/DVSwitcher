#include "settings_controller.h"

Settings_Controller::Settings_Controller(QObject *parent) : QObject(parent)
{
	m_settings = new QSettings("fardragon","DVSwitcher");
}

Settings_Controller::~Settings_Controller()
{
	delete m_settings;
}

void Settings_Controller::load_settings()
{
	if (m_settings->contains("def_vibrance"))
	{
		emit loaded_def_vibrance(m_settings->value("def_vibrance").toInt());
	}

	if (m_settings->contains("start_minimized"))
	{
		emit loaded_start_minimized(m_settings->value("start_minimized").toBool());
	}
	if (m_settings->contains("disable_switcher"))
	{
		emit loaded_disable_switcher(m_settings->value("disable_switcher").toBool());
	}
	emit loaded_start_windows(this->check_start_windows());
	this->load_apps();

}

void Settings_Controller::save_apps(const std::vector<std::tuple<QString, int, bool> > apps_data)
{
	m_settings->beginWriteArray("apps_data",apps_data.size());
	for (uint i = 0; i < apps_data.size(); ++i)
	{
		m_settings->setArrayIndex(i);
		m_settings->setValue("filepath",std::get<0>(apps_data.at(i)));
		m_settings->setValue("dig_vibrance",std::get<1>(apps_data.at(i)));
		m_settings->setValue("active",std::get<2>(apps_data.at(i)));
	}
	m_settings->endArray();
}

bool Settings_Controller::check_start_windows()
{
	QSettings registry("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
	if (registry.contains("DVSwitcher_autostart")) return true;
	return false;
}

void Settings_Controller::load_apps()
{
	uint size = m_settings->beginReadArray("apps_data");
	if (size == 0)
	{
		m_settings->endArray();
		return;
	}
	QString temp_filepath;
	int temp_vibrance;
	bool temp_active;

	for (uint i=0; i<size; ++i)
	{
		m_settings->setArrayIndex(i);
		temp_filepath= m_settings->value("filepath").toString();
		temp_vibrance = m_settings->value("dig_vibrance").toInt();
		temp_active = m_settings->value("active").toBool();
		emit loaded_app(std::make_tuple(temp_filepath,temp_vibrance,temp_active));
	}
	m_settings->endArray();
}

void Settings_Controller::new_def_vibrance(int new_def_vib)
{
	m_settings->setValue("def_vibrance",new_def_vib);
}

void Settings_Controller::new_start_minimized(bool start_minimized)
{
	m_settings->setValue("start_minimized",start_minimized);
}

void Settings_Controller::new_disable_switcher(bool disable_switcher)
{
	m_settings->setValue("disable_switcher",disable_switcher);
}

void Settings_Controller::new_start_windows(bool start_windows)
{
	QSettings registry("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);

	if (start_windows)
	{
		registry.setValue("DVSwitcher_autostart", QCoreApplication::applicationFilePath().replace('/', '\\'));
	}
	else
	{
		registry.remove("DVSwitcher_autostart");
	}
}



