#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_apps_list(parent),
	m_hook_observer(FocusHook::getInstance())
{
    ui->setupUi(this);

	this->setFixedSize(this->size());

	connect(&m_hook_observer,SIGNAL(foreground_changed(QString)),this,SLOT(foreground_change(QString)));

	this->load_apps();
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
		this->ui->apps_listWidget->addItem(it.name());
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
void MainWindow::on_apps_listWidget_currentRowChanged(int currentRow)

{
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
