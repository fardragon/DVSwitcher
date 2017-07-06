#ifndef APP_LIST_H
#define APP_LIST_H

#include <QObject>
#include <vector>

#include "app.h"

class App_List : public QObject
{
	Q_OBJECT
public:
	explicit App_List(QObject *parent = nullptr);
	void add_app(const QString& filepath);
	const std::vector<app>& get_apps() const;
	std::pair<int,bool> get_settings(int index);
	void set_vibrance(int index,int new_vibrance);
	void set_active(int index, bool new_active);
	void delete_app(int index);
	bool is_duplicate(const QString& filepath,int &duplicate_index);

private:
	std::vector<app> m_list;


signals:

public slots:
};

#endif // APP_LIST_H
