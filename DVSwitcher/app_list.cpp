#include "app_list.h"

App_List::App_List(QObject *parent) : QObject(parent)
{

}

void App_List::add_app(const QString& filepath, int vibrance_level, bool active)
{
	if (m_list.size() < UINT_MAX)
	{
		m_list.emplace_back(filepath,vibrance_level,active);
	}
	else
	{
		QMessageBox::critical(nullptr,"Error","Cannot add more apps");
	}
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

std::vector<std::tuple<QString, int, bool> > App_List::save_data() const
{
	std::vector<std::tuple<QString, int, bool>> temp;
	std::pair<int,bool> temp_settings;
	QString temp_filepath;


	for (const auto &it: m_list)
	{
		temp_settings = it.settings();
		temp_filepath = it.filepath();
		temp.push_back(std::make_tuple(temp_filepath,temp_settings.first,temp_settings.second));
	}
	return temp;
}

void App_List::load_app(std::tuple<QString, int, bool> new_app)
{
	this->add_app(std::get<0>(new_app),std::get<1>(new_app),std::get<2>(new_app));
}

void App_List::check_exec(QString filepath)
{
	if (filepath == "Disabled")
	{
		emit def_vibrance();
		qDebug() << "Default vibrance";
		return;
	}
	for (auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		if (it->filepath() == filepath)
		{
			if (it->active())
			{
				emit new_vibrance(it->vibrance_level());
				qDebug() << "New vibrance: " << it->vibrance_level();
				return;
			}
			else
			{
				emit def_vibrance();
				qDebug() << "Default vibrance";
				return;
			}
		}
	}
	emit def_vibrance();
	qDebug() << "Default vibrance";
}




