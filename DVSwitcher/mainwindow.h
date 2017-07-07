#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMenu>

#include <windows.h>

#include <QDebug>




#include "app_list.h"
#include "focushook.h"
#include "settings_controller.h"


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
	void init_tray();


    Ui::MainWindow *ui;
	App_List m_apps_list;
	QSystemTrayIcon m_tray;
	Settings_Controller m_settings;

private slots:

	void on_active_checkBox_clicked(bool checked);
	void on_vibrance_Slider_valueChanged(int value);
	void on_delete_Button_clicked();
	void on_apps_listWidget_currentRowChanged(int currentRow);
	void on_add_Button_clicked();
	void on_def_vibrance_Slider_valueChanged(int value);
	void foreground_change(QString new_foreground_process);
	void closeEvent(QCloseEvent *event);
	void changeEvent(QEvent *event);
	void systemTray(QSystemTrayIcon::ActivationReason reason);
	void bring_forward();


signals:

};

#endif // MAINWINDOW_H
