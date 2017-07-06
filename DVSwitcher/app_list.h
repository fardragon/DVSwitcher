#ifndef APP_LIST_H
#define APP_LIST_H

#include <QObject>

class App_List : public QObject
{
	Q_OBJECT
public:
	explicit App_List(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APP_LIST_H