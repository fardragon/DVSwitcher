#include "app.h"

app::app(const QString& filepath, int vibrance_level, bool active) :
	m_filepath(filepath),m_vibrance_level(vibrance_level),m_active(active)
{
	QFileInfo exec = QFileInfo(filepath);
	m_name=exec.fileName();

}

QString app::name() const
{
	return m_name;
}

std::pair<int, bool> app::settings() const
{
	return std::make_pair(m_vibrance_level,m_active);
}

void app::set_active(bool active)
{
	m_active = active;
}

void app::set_vibrance_level(int vibrance_level)
{
	m_vibrance_level = vibrance_level;
}

bool app::active() const
{
	return m_active;
}

int app::vibrance_level() const
{
	return m_vibrance_level;
}

QString app::filepath() const
{
	return m_filepath;
}
