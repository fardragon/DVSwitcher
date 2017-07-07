#ifndef APP_LIST_H
#define APP_LIST_H

#include <QObject>
#include <QMessageBox>
#include <vector>
#include <limits>
#include <tuple>

#include <QDebug>

#include "app.h"

class App_List : public QObject
{
	Q_OBJECT
public:
	explicit App_List(QObject *parent = nullptr);
	void add_app(const QString& filepath, int vibrance_level = 50, bool active = true);
	const std::vector<app>& get_apps() const;
	std::pair<int,bool> get_settings(int index);
	void set_vibrance(int index,int new_vibrance);
	void set_active(int index, bool new_active);
	void delete_app(int index);
	bool is_duplicate(const QString& filepath,int &duplicate_index);
	std::vector<std::tuple<QString,int,bool>> save_data() const;

private:
	std::vector<app> m_list;


signals:
	void def_vibrance();
	void new_vibrance(int);

public slots:
	void load_app(std::tuple<QString,int,bool> new_app);
	void check_exec(QString filepath);

};

#endif // APP_LIST_H
