#ifndef APP_H
#define APP_H

#include <QString>
#include <QFileInfo>

class app
{
public:
	app(const QString& filepath, int vibrance_level = 50, bool active = true);

	QString name() const;
	QString filepath() const;
	std::pair<int,bool> settings();
	void set_active(bool active);
	void set_vibrance_level(int vibrance_level);

private:
	QString m_name;
	QString m_filepath;

	int m_vibrance_level;
	bool m_active;
};

#endif // APP_H
