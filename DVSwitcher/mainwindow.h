#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <shlwapi.h>

#include "app_list.h"
#include "focushook.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	void load_apps();
	void unload_apps();



    Ui::MainWindow *ui;
	App_List m_apps_list;
	FocusHook &m_hook_observer;

private slots:

	void on_active_checkBox_clicked(bool checked);
	void on_vibrance_Slider_valueChanged(int value);
	void on_delete_Button_clicked();
	void on_apps_listWidget_currentRowChanged(int currentRow);
	void on_add_Button_clicked();
	void on_def_vibrance_Slider_valueChanged(int value);
	void foreground_change(QString new_foreground_process);

signals:

};

#endif // MAINWINDOW_H
