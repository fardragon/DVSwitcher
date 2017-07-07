#include "single_instance_guard.h"

Single_Instance_Guard::Single_Instance_Guard(QObject *parent) : QObject(parent)
{
	QObject::connect(&m_server,&QLocalServer::newConnection,this,&Single_Instance_Guard::new_connection);
}

Single_Instance_Guard::~Single_Instance_Guard()
{
	m_server.close();
}

void Single_Instance_Guard::listen(const QString &name)
{
	m_server.removeServer(name);
	m_server.listen(name);
}

bool Single_Instance_Guard::has_previous_instance(const QString &name)
{
	QLocalSocket socket;
	socket.connectToServer(name,QLocalSocket::ReadWrite);

	if (socket.waitForConnected())
	{
		socket.disconnectFromServer();
		return true;
	}
	return false;
}

void Single_Instance_Guard::new_connection()
{
	emit new_instance();
	m_socket = m_server.nextPendingConnection();
}


