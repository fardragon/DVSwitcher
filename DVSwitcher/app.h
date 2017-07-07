#ifndef APP_H
#define APP_H

#include <QString>
#include <QFileInfo>

class app
{
public:
	app(const QString& filepath, int vibrance_level, bool active);
	QString name() const;
	QString filepath() const;
	std::pair<int,bool> settings() const;
	void set_active(bool active);
	void set_vibrance_level(int vibrance_level);
	bool active() const;
	int vibrance_level() const;

private:
	QString m_name;
	QString m_filepath;

	int m_vibrance_level;
	bool m_active;
};

#endif // APP_H
