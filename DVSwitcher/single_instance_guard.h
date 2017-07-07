#ifndef SINGLE_INSTANCE_GUARD_H
#define SINGLE_INSTANCE_GUARD_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

class Single_Instance_Guard : public QObject
{
	Q_OBJECT
public:
	explicit Single_Instance_Guard(QObject *parent = nullptr);
	~Single_Instance_Guard();

	void listen(const QString &name);
	bool has_previous_instance(const QString &name);

private:

	QLocalSocket *m_socket;
	QLocalServer m_server;


signals:

	void new_instance();

public slots:

	void new_connection();

};

#endif // SINGLE_INSTANCE_GUARD_H
