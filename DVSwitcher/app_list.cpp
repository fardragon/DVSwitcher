#include "app_list.h"

App_List::App_List(QObject *parent) : QObject(parent)
{
	this->add_app("C:/Programy/Audacity/audacity.exe");
}

void App_List::add_app(const QString &filepath)
{
	m_list.emplace_back(filepath);
}

const std::vector<app> &App_List::get_apps() const
{
	return m_list;
}

std::pair<int, bool> App_List::get_settings(int index)
{
	return m_list.at(index).settings();
}

void App_List::set_vibrance(int index, int new_vibrance)
{
	m_list.at(index).set_vibrance_level(new_vibrance);
}

void App_List::set_active(int index, bool new_active)
{
	m_list.at(index).set_active(new_active);
}

void App_List::delete_app(int index)
{
	m_list.erase(m_list.begin()+index);
}

bool App_List::is_duplicate(const QString& filepath, int &duplicate_index)
{
	duplicate_index = -1;
	for (auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		if (it->filepath() == filepath)
		{
			duplicate_index = std::distance(m_list.begin(),it);
			return true;
		}
	}
	return false;
}




