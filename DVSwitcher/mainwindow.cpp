#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_apps_list(parent)
{
    ui->setupUi(this);

	this->setFixedSize(this->size());

	QObject::connect(&FocusHook::getInstance(),&FocusHook::foreground_changed,this,&MainWindow::foreground_change);
	QObject::connect(this->ui->def_vibrance_Slider,&QSlider::valueChanged,&this->m_settings,&Settings_Controller::new_def_vibrance);
	QObject::connect(&m_settings,&Settings_Controller::loaded_def_vibrance,this->ui->def_vibrance_Slider,&QSlider::setValue);
	QObject::connect(this->ui->start_min_checkBox,&QCheckBox::toggled,&m_settings,&Settings_Controller::new_start_minimized);
	QObject::connect(&m_settings,&Settings_Controller::loaded_start_minimized,this->ui->start_min_checkBox,&QCheckBox::setChecked);
	QObject::connect(this->ui->autostart_checkBox,&QCheckBox::toggled,&m_settings,&Settings_Controller::new_start_windows);
	QObject::connect(&m_settings,&Settings_Controller::loaded_start_windows,this->ui->autostart_checkBox,&QCheckBox::setChecked);
	QObject::connect(&m_settings,&Settings_Controller::loaded_app,&m_apps_list,&App_List::load_app);
	QObject::connect(this->ui->disable_Checkbox,&QCheckBox::toggled,&m_settings,&Settings_Controller::new_disable_switcher);
	QObject::connect(this->ui->disable_Checkbox,&QCheckBox::toggled,&FocusHook::getInstance(),&FocusHook::toggle_active);
	QObject::connect(&m_settings,&Settings_Controller::loaded_disable_switcher,this->ui->disable_Checkbox,&QCheckBox::setChecked);
	QObject::connect(&FocusHook::getInstance(),&FocusHook::foreground_changed,&m_apps_list,&App_List::check_exec);

	m_settings.load_settings();

	this->init_tray();
	this->load_apps();
	if (!this->ui->start_min_checkBox->isChecked())this->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::load_apps()
{
	auto list = m_apps_list.get_apps();


	for (const auto& it : list)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(it.name());
		item->setToolTip(it.filepath());
		this->ui->apps_listWidget->addItem(item);
	}
	if (this->ui->apps_listWidget->count() > 0 )
	{
		this->ui->apps_listWidget->setCurrentRow(0);
		this->ui->vibrance_Slider->show();
		this->ui->active_checkBox->show();
		this->ui->vibrance_text->show();
		this->ui->vibrance_label->show();
	}
	else
	{
		this->ui->vibrance_Slider->hide();
		this->ui->active_checkBox->hide();
		this->ui->vibrance_text->hide();
		this->ui->vibrance_label->hide();
	}

}

void MainWindow::unload_apps()
{
	this->ui->apps_listWidget->clear();
}

void MainWindow::init_tray()
{
	QPixmap pixmap(":/DVSwitcher/Resources/Icons/Icon.bmp");
	QIcon icon(pixmap);
	auto menu = new QMenu;
	QObject::connect(&m_tray,&QSystemTrayIcon::activated,this,&MainWindow::systemTray);
	menu->addAction("Open",this,&MainWindow::bring_forward);

	auto toggle = new QAction;
	toggle->setText("Disable");
	toggle->setCheckable(true);
	menu->addAction(toggle);
	QObject::connect(toggle,&QAction::toggled,this->ui->disable_Checkbox,&QCheckBox::setChecked);
	toggle->setChecked(this->ui->disable_Checkbox->isChecked());
	menu->addAction("Quit",this,&MainWindow::close);

	m_tray.setContextMenu(menu);
	m_tray.setIcon(icon);
	m_tray.setToolTip("Digital Vibrance Switcher");
	m_tray.show();
}

void MainWindow::bring_forward()
{
	if (!this->isVisible())
	{
		this->showNormal();
		HWND handle = reinterpret_cast<HWND>(this->winId());
		BringWindowToTop(handle);
		SetForegroundWindow(handle);
		SetFocus(handle);
	}
	else
	{
		HWND handle = reinterpret_cast<HWND>(this->winId());
		BringWindowToTop(handle);
		SetForegroundWindow(handle);
		SetFocus(handle);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	auto save_data = m_apps_list.save_data();
	m_settings.save_apps(save_data);
	event->accept();
}

void MainWindow::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange )
	{
		if (isMinimized())
		{
			this->hide();
		}
	}
}

void MainWindow::systemTray(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		this->bring_forward();
		break;
	case QSystemTrayIcon::DoubleClick:

		this->hide();
		break;
	default:

		break;
	}

}

void MainWindow::on_active_checkBox_clicked(bool checked)
{
	int index = this->ui->apps_listWidget->currentRow();
	if (index != -1) m_apps_list.set_active(index,checked);
}

void MainWindow::on_vibrance_Slider_valueChanged(int value)
{
	int index = this->ui->apps_listWidget->currentRow();
	if (index != -1) m_apps_list.set_vibrance(index,value);
	QString text = QString::number(value) + "%";
	this->ui->vibrance_label->setText(text);
}

void MainWindow::on_delete_Button_clicked()
{
	int index = this->ui->apps_listWidget->currentRow();
	if (index != -1) m_apps_list.delete_app(index);
	this->unload_apps();
	this->load_apps();
}

void MainWindow::on_apps_listWidget_currentRowChanged(int currentRow){
	if (currentRow > -1)
	{
		auto data = m_apps_list.get_settings(currentRow);
		this->ui->vibrance_Slider->setValue(data.first);
		this->ui->active_checkBox->setChecked(data.second);
	}
	else
	{
		this->ui->vibrance_Slider->setValue(0);
		this->ui->active_checkBox->setChecked(false);
	}
}

void MainWindow::on_add_Button_clicked()
{
	QString file = QFileDialog::getOpenFileName(this,"Select executable to add", QDir::homePath(), "Executable (*.exe)");
	if (file.size()>0)
	{
		auto exec = QFileInfo(file);
		int duplicate_pos;
		if (exec.suffix() != "exe")
		{
			QMessageBox::warning(this,"Error","Selected file is not an executable!");
		}
		else if (!exec.exists())
		{
			QMessageBox::warning(this,"Error","Selected file does not exist!");
		}
		else if (m_apps_list.is_duplicate(file,duplicate_pos))
		{
			QMessageBox::warning(nullptr,"Duplicate","This application is already on list!");
			this->ui->apps_listWidget->setCurrentRow(duplicate_pos);
		}
		else
		{
			m_apps_list.add_app(file);
			this->unload_apps();
			this->load_apps();
			this->ui->apps_listWidget->setCurrentRow(this->ui->apps_listWidget->count()-1);
		}
	}
}

void MainWindow::on_def_vibrance_Slider_valueChanged(int value)
{
	QString text = QString::number(value) + "%";
	this->ui->def_vibrance_level->setText(text);
}

void MainWindow::foreground_change(QString new_foreground_process)
{
	auto file= QFileInfo(new_foreground_process);
	this->ui->executable_label->setText(file.fileName());
}

